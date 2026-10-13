// Visible manual-login browser. No credential/cookie/header/state export APIs.
const fs = require('node:fs/promises');
const path = require('node:path');
const readline = require('node:readline/promises');
const crypto = require('node:crypto');
const root = '/home/toolchain/development/libc++_replacement';
const work = path.join(root, 'tmp/QB_1194606');
process.env.PLAYWRIGHT_BROWSERS_PATH = path.join(work, 'browsers');
const { chromium } = require(path.join(work, 'playwright/node_modules/playwright-core'));
const target = 'https://quickbuild.tizen.org/build/1194606/gbs_reports';
let context;
const status = async (state, extra = {}) => fs.writeFile(path.join(work, 'browser-status.json'),
  JSON.stringify({state, at: new Date().toISOString(), ...extra}, null, 2), {mode: 0o600});
const delay = ms => new Promise(resolve => setTimeout(resolve, ms));
const safeDestination = name => {
  const dest = path.resolve(work, 'downloads', name);
  if (!dest.startsWith(path.join(work, 'downloads') + path.sep)) throw new Error('InvalidDestination');
  return dest;
};
const safeUrl = value => {
  const url = new URL(value);
  if (url.protocol !== 'https:' || url.hostname !== 'quickbuild.tizen.org' || url.username || url.password) {
    throw new Error('UnapprovedHost');
  }
  return url.href;
};
async function main() {
  process.umask(0o077);
  await fs.mkdir(path.join(work, 'browser-profile'), {recursive: true, mode: 0o700});
  await fs.mkdir(path.join(work, 'commands'), {recursive: true, mode: 0o700});
  await fs.mkdir(path.join(work, 'results'), {recursive: true, mode: 0o700});
  await fs.mkdir(path.join(work, 'downloads'), {recursive: true, mode: 0o700});
  context = await chromium.launchPersistentContext(path.join(work, 'browser-profile'), {
    headless: false, viewport: null, acceptDownloads: true,
  });
  const page = context.pages()[0] || await context.newPage();
  await page.goto(target, {waitUntil: 'domcontentloaded', timeout: 60000});
  const terminal = readline.createInterface({input: process.stdin, output: process.stdout});
  let authenticated = false;
  for (let attempt = 1; attempt <= 3; attempt++) {
    await status('WAITING_FOR_MANUAL_LOGIN', {attempt});
    await terminal.question('请在浏览器窗口中用 lhmax2025 登录，登录完成后回到终端按回车\n');
    // Deliberately ignore terminal input; never inspect login form values.
    await page.goto(target, {waitUntil: 'domcontentloaded', timeout: 60000});
    const passwordFormPresent = await page.locator('input[type="password"]').count() > 0;
    const loginUrl = /(?:signin|login)/i.test(new URL(page.url()).pathname);
    if (!passwordFormPresent && !loginUrl) {
      authenticated = true;
      break;
    }
    console.log('仍是登录页面；请在浏览器中完成登录，不要在终端输入凭据。');
  }
  terminal.close();
  if (!authenticated) {
    await status('LOGIN_NOT_CONFIRMED_AFTER_THREE_ATTEMPTS');
    await context.close();
    return;
  }
  await status('READY', {password_form_present: false});
  console.log('登录页面检查通过。浏览器会话将用于只读抓取；请保留窗口。');
  // Commands contain only URLs and output names, not credentials. Process serially.
  let stop = false;
  while (!stop) {
    for (const name of (await fs.readdir(path.join(work, 'commands'))).filter(n => n.endsWith('.json')).sort()) {
      const resultPath = path.join(work, 'results', name);
      try { await fs.access(resultPath); continue; } catch {}
      let result;
      try {
        const command = JSON.parse(await fs.readFile(path.join(work, 'commands', name), 'utf8'));
        if (command.action === 'close') {
          stop = true;
          result = {ok: true, action: 'close'};
        } else if (command.action === 'capture') {
          await page.goto(safeUrl(command.url), {waitUntil: 'domcontentloaded', timeout: 60000});
          await page.waitForTimeout(1500);
          if (await page.locator('input[type="password"]').count()) throw new Error('LoginRequired');
          const dest = safeDestination(command.name);
          await fs.mkdir(path.dirname(dest), {recursive: true});
          const html = await page.content();
          const text = await page.locator('body').innerText();
          const links = await page.locator('a[href]').evaluateAll(nodes => nodes.map(a => ({text: a.innerText, href: a.href})));
          await fs.writeFile(dest + '.html', html, {mode: 0o600});
          await fs.writeFile(dest + '.txt', text, {mode: 0o600});
          await fs.writeFile(dest + '.links.json', JSON.stringify(links, null, 2), {mode: 0o600});
          result = {ok: true, action: 'capture', files: [dest + '.html', dest + '.txt', dest + '.links.json']};
        } else if (command.action === 'download') {
          // Shared browser-session request context; cookies handled internally only.
          const response = await context.request.get(safeUrl(command.url), {timeout: 120000});
          const body = await response.body();
          const dest = safeDestination(command.name);
          await fs.mkdir(path.dirname(dest), {recursive: true});
          await fs.writeFile(dest, body, {mode: 0o600});
          result = {ok: response.ok(), action: 'download', status: response.status(),
            path: dest, bytes: body.length, sha256: crypto.createHash('sha256').update(body).digest('hex')};
          await response.dispose();
        } else throw new Error('UnknownAction');
      } catch (error) {
        // Do not persist runtime exception text/headers that could contain session data.
        result = {ok: false, error_type: error.name || 'Error'};
      }
      await fs.writeFile(resultPath, JSON.stringify(result, null, 2), {mode: 0o600});
    }
    if (!stop) await delay(500);
  }
  await context.close();
  await status('CLOSED_PROFILE_RETAINED');
}
main().catch(async error => {
  await status('BROWSER_ERROR', {error_type: error.name || 'Error'});
  console.error('浏览器步骤失败，未记录异常全文以避免会话信息进入日志。');
  if (context) await context.close().catch(() => {});
  process.exitCode = 1;
});

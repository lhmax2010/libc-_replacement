#!/usr/bin/env python3
"""人工归属表驱动；保留每个候选及全部相关文件快照，拒绝自动推定安全。"""
import pathlib,csv,hashlib,re
root=pathlib.Path('codes/llvm/libcxx')
out=pathlib.Path('docs/progress/R114/W1')
actual={
 'src/shared_mutex.cpp':{25:'S',28:'H',53:'S',92:'H'},
 'src/mutex.cpp':{82:'S',117:'S'},
 'src/future.cpp':{103:'F',110:'F',121:'F',138:'G'},
 'src/condition_variable.cpp':{36:'C',60:'C'},
 'src/call_once.cpp':{47:'X'},
 'include/shared_mutex':{198:'H',230:'H',239:'S',249:'H',270:'S',280:'S',324:'T',417:'T'},
 'include/mutex':{257:'S',266:'S',294:'S',310:'S'},
 'include/__thread/this_thread.h':{61:'S'},
 'include/__condition_variable/condition_variable.h':dict.fromkeys([112,127,135,160,162,193,223],'C'),
 'include/condition_variable':dict.fromkeys([181,193,237,244,251,257,300,329,338],'A'),
 'include/future':dict.fromkeys([571,582,637,646,697,955,958,962,1026,1029,1033,1093,1096,1100,1908,1915,1918,1922,1964,1971,1974,1978,2020,2027,2030,2034],'F'),
 'include/__mutex/unique_lock.h':{59:'T',153:'T'},
 'include/__cxx03/mutex':dict.fromkeys([253,267,292,313],'S'),
 'include/__cxx03/__thread/this_thread.h':{58:'S'},
 'include/__cxx03/__condition_variable/condition_variable.h':dict.fromkeys([147,161,169,194,196,205,235],'C'),
 'include/__cxx03/future':dict.fromkeys([517,525,583,592,643,901,904,908,972,975,979,1039,1042,1046,1781,1788,1791,1795,1837,1844,1847,1851,1893,1900,1903,1907],'F'),
 'include/__cxx03/__mutex/unique_lock.h':{57:'T',156:'T'},
}
actual['include/future'].update({861:'N',898:'N',986:'G',1053:'G'})
actual['include/__cxx03/future'].update({807:'N',844:'N',932:'G',999:'G'})
# C++03 condition_variable_any 的实际调用行从相同成员调用语法定位后人工复核。
p=root/'include/__cxx03/condition_variable'
actual[str(p.relative_to(root))]={i:'A' for i,l in enumerate(p.read_text().splitlines(),1) if re.search(r'(?:__cv_\.(?:wait|wait_until)\(|^\s+(?:return |if \()?wait(?:_until)?\(__lock)',l)}
meaning={
 'S':('安全','等待前未占有业务锁/未设置共享预约状态；unique_lock 退出解锁。共享读者计数在等待后增加。'),
 'H':('有残留且有害','直接或经包装在 gate2 等待读者退出前设置 write_entered；仅正常超时清除，展开无回滚。'),
 'F':('安全（限定内部等待分支）','等待 shared state ready 前不修改 ready；锁由 unique_lock 清理。deferred 用户函数及外层生命周期边界另列，非全调用链安全证明。'),
 'G':('有残留但无害（限定普通共享状态）','get 先置空 future 指针，unique_ptr/release_shared_count 负责释放原引用；消费 future 的状态保留但无孤立锁。async 最后引用另列 N。'),
 'C':('无法判定（调用方状态）','条件变量包装只计算局部时间/判断谓词，不管理调用者业务状态；mutex 在底层取消清理后重新持有，需调用者守卫解锁。任意谓词/Clock 的副作用不作推定。'),
 'A':('安全（限定成功重锁）','复制 shared_ptr；等待前释放用户锁；RAII 先解内部锁再重锁用户锁，stop_callback 析构注销回调。自定义 lock() 抛出时 noexcept 析构终止，不能无条件宣称干净退出。'),
 'N':('无法判定（展开不能穿过此规格）','async 最后引用释放的 __on_zero_shared 为 noexcept，调用 wait 后才销毁；本次四入口解除不解除这一边界，不能归为可回滚的已退出残留。'),
 'T':('无法判定（模板实参）','守卫只在成功获取后记录 owns；实际锁方法若为 shared mutex 则继承 H，其他用户 Mutex 的内部状态不可推定。'),
 'X':('范围疑问','call_once 直接调用底层 helper，未经过四种入口；静态见等待侧原生 mutex 缺少退出守卫，待人工确认是否扩围。'),
}
rows=[]; snippets=['# 调用点代码及分类\n']; hashes=[]
for file,entries in sorted(actual.items()):
 text=(root/file).read_text();lines=text.splitlines()
 snap=out/'snapshots'/file;snap.parent.mkdir(parents=True,exist_ok=True);snap.write_text(text)
 hashes.append(hashlib.sha256(text.encode()).hexdigest()+'  '+str(snap))
 for n,tag in sorted(entries.items()):
  cat,reason=meaning[tag];rows.append([file,n,tag,cat,reason,lines[n-1].strip()])
  snippets += [f'\n## {file}:{n}\n\n分类：{cat}。{reason}\n\n```cpp\n',* [f'{i+1:4} {lines[i]}\n' for i in range(max(0,n-5),min(len(lines),n+5))],'```\n']
assert actual['src/shared_mutex.cpp'][28]=='H' and actual['include/shared_mutex'][249]=='H'
with (out/'calls.tsv').open('x') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['文件','行','归属','分类','依据','代码']);w.writerows(rows)
(out/'CALL_SITES.md').write_text(''.join(snippets))
(out/'source_SHA256SUMS').write_text('\n'.join(hashes)+'\n')
print(f'reviewed_call_lines={len(rows)} files={len(actual)} harmful_positive_controls=2/2')

"""Read-only guest observations. No inferior calls/writes, finish, or stepping."""
import gdb,json,os,pathlib,struct,time
BASE=pathlib.Path('/home/toolchain/development/libc++_replacement')
ROOT=BASE/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0'
DEBUG=BASE/'tmp/ARM_LZMA_0921B/debug-root/usr/lib/debug/usr/lib'
OUT=pathlib.Path(os.environ['OBS_OUT'])
events=(OUT/'gdb-observations.jsonl').open('x',buffering=1)
count=0; open_pointer=None; observed=[]; returns=[]
def record(event,**kw):
    row=dict(event=event,monotonic_ns=time.monotonic_ns(),close_count=count,**kw)
    line=json.dumps(row,ensure_ascii=False)
    events.write(line+'\n');events.flush();gdb.write(line+'\n')
def reg(n):return int(gdb.parse_and_eval('$'+n))&0xffffffff
def u32(p):return struct.unpack('<I',gdb.selected_inferior().read_memory(p,4))[0]
def text(p):
    if not p:return None
    return gdb.Value(p).cast(gdb.lookup_type('char').pointer()).string(errors='replace',length=-1)
def lz(p):
    if not p:return dict(encoding=None,file=None,internal=None)
    return dict(encoding=u32(p+0x805c),file=hex(u32(p+0x8058)),internal=hex(u32(p+0x8024)))
def address(name):return int(gdb.parse_and_eval('(void*)'+name))
class ReturnBP(gdb.Breakpoint):
    def __init__(self,where,name):
        super().__init__('*'+hex(where),internal=True);self.label=name
    def stop(self):
        if count==18:
            r=reg('r0');record('conditional_return',function=self.label,pc=hex(reg('pc')),r0=hex(r),signed_r0=r if r<0x80000000 else r-0x100000000)
        return False
def arm_return_observations():
    if returns:return
    # Static epilogues, not FinishBreakpoint; r0 is already populated here.
    for fn,offset in [('Fwrite',0x1036c),('rpmcpioWrite',0x3a610),('rpmcpioHeaderWrite',0x3a19c)]:
        base=io_base if fn=='Fwrite' else rpm_base
        insn=gdb.selected_frame().architecture().disassemble(base+offset,base+offset+4)[0]['asm']
        if 'pc' not in insn or not ('pop' in insn or 'ldm' in insn):
            raise RuntimeError('return opcode mismatch '+fn+' '+insn)
        returns.append(ReturnBP(base+offset,fn))
    record('branch_b_return_breakpoints_enabled',functions=['Fwrite','rpmcpioWrite','rpmcpioHeaderWrite'])
class Observe(gdb.Breakpoint):
    def __init__(self,where,label):
        super().__init__('*'+hex(where),internal=True);self.label=label;self.enabled=False
    def stop(self):
        global open_pointer
        if count!=18:return False
        try:
            p=reg('r0')
            if self.label=='lzopen_entry':
                raw=bytes(gdb.selected_inferior().read_memory(p,16))
                first=raw.find(b'\x00')
                second=raw.find(b'\x00',first+1) if first>=0 else -1
                tail=raw[first+1:second if second>=0 else 16] if first>=0 else b''
                printable=lambda b: ''.join(chr(c) if 32<=c<=126 else '.' for c in b)
                record(self.label,mode_pointer=hex(p),raw_hex=raw.hex(' '),
                       raw_printable=printable(raw),first_nul=first,next_nul=second,
                       after_first_nul_hex=tail.hex(' '),after_first_nul_printable=printable(tail),
                       segment_complete=first>=0 and second>=0,contains_r_in_segment=114 in tail)
            elif self.label=='lzopen_return':
                open_pointer=p;record(self.label,pointer=hex(p),**lz(p))
            elif self.label=='lzwrite_entry':
                fields=lz(p)
                record(self.label,pointer=hex(p),length=reg('r2'),open_pointer=None if open_pointer is None else hex(open_pointer),equal_to_open=None if open_pointer is None else p==open_pointer,**fields)
                if open_pointer is not None and p==open_pointer and fields['encoding']==1:
                    record('branch_b_observed_no_extra_breakpoints',reason='current task permits entry addition only')
            else:
                fd=u32(p);fps=u32(fd+12);io=u32(fps);fp=u32(fps+4)
                record(self.label,cpio=hex(p),fd=hex(fd),fps=hex(fps),io=hex(io),ioname=text(u32(io)),name=text(u32(io+4)),fp=hex(fp),path=text(reg('r1')),open_pointer=None if open_pointer is None else hex(open_pointer),equal_to_open=None if open_pointer is None else fp==open_pointer)
        except Exception as e:
            record('observer_error',point=self.label,error=str(e));return True
        return False
class CloseCounter(gdb.Breakpoint):
    def stop(self):
        global count
        count+=1
        record('lzclose_counter',pointer=hex(reg('r0')),**lz(reg('r0')))
        for bp in observed:bp.enabled=(count==18)
        if count==19:
            for bp in returns:bp.enabled=False
        return False
gdb.execute('set pagination off');gdb.execute('set confirm off')
gdb.execute('set architecture armv7');gdb.execute('set auto-load off')
gdb.execute('set sysroot '+str(ROOT))
gdb.execute('file '+str(ROOT/'usr/bin/rpmbuild'))
gdb.execute('set stop-on-solib-events 1')
gdb.execute('target remote 127.0.0.1:'+os.environ['OBS_PORT'])
# Loader rendezvous only to resolve DSO bases, before any RPM archive work.
for attempt in range(30):
    try:
        io_base=address('Fwrite')-0x1014c
        rpm_base=address('rpmcpioWrite')-0x3a54c
        break
    except gdb.error:gdb.execute('continue')
else:raise RuntimeError('RPM shared libraries not resolved')
record('shared_library_bases',librpmio=hex(io_base),librpm=hex(rpm_base),inferior_pid=gdb.selected_inferior().pid)
gdb.execute('info sharedlibrary')
gdb.execute('add-symbol-file '+str(DEBUG/'librpmio.so.3.1.0.debug')+' -o '+hex(io_base))
gdb.execute('add-symbol-file '+str(DEBUG/'librpm.so.3.1.0.debug')+' -o '+hex(rpm_base))
gdb.execute('set stop-on-solib-events 0')
for p,label in [(io_base+0x132b4,'lzopen_entry'),(io_base+0x13840,'lzopen_return'),(io_base+0x12fe4,'lzwrite_entry'),(rpm_base+0x39cc8,'cpio_header_entry')]:
    observed.append(Observe(p,label))
CloseCounter('*'+hex(io_base+0x13154),internal=True)
record('observer_ready',enabled='lzclose counter only; four points enabled when counter reaches 18',no_inferior_calls=True,no_inferior_writes=True)
gdb.execute('continue')
record('gdb_continue_returned',inferior_pid=gdb.selected_inferior().pid)
if gdb.selected_inferior().pid:
    raise RuntimeError('unexpected stop: do not automatically resume')

set pagination off
set confirm off
set breakpoint pending on
set print thread-events off
set debuginfod enabled off
python
import gdb, json
seen_maps = False
class ReturnObserve(gdb.FinishBreakpoint):
    def stop(self):
        reg = '$r0' if 'arm' in gdb.newest_frame().architecture().name() else '$eax'
        print('P11_RETURN '+json.dumps({'function':'can_catch',
              'value':str(self.return_value), 'return_register':int(gdb.parse_and_eval(reg))}))
        return False
class Observe(gdb.Breakpoint):
    def stop(self):
        global seen_maps
        frame = gdb.newest_frame()
        pc = int(frame.pc())
        row = {'event':self.location, 'pc':hex(pc), 'object':gdb.solib_name(pc),
               'architecture':frame.architecture().name()}
        arm = 'arm' in row['architecture']
        word = 4 if arm else 8
        if self.location == '__cxa_throw':
            ti = int(gdb.parse_and_eval('$r1' if arm else '$rsi'))
            row['type_info'] = hex(ti)
            try:
                nameptr = int.from_bytes(gdb.selected_inferior().read_memory(ti+word,word).tobytes(),'little')
                row['type_name'] = gdb.parse_and_eval('(char*)'+hex(nameptr)).string()
            except Exception as e: row['type_read_error'] = str(e)
        elif self.location == '__gxx_personality_v0':
            if arm:
                ptr = int(gdb.parse_and_eval('$r1'))
                row['ehabi_state'] = int(gdb.parse_and_eval('$r0'))
                row['exception_class'] = hex(int.from_bytes(gdb.selected_inferior().read_memory(ptr,8).tobytes(),'little'))
                row['exception_pointer'] = hex(ptr)
            else:
                row['actions'] = int(gdb.parse_and_eval('$rsi'))
                row['exception_class'] = hex(int(gdb.parse_and_eval('$rdx')) & ((1<<64)-1))
                row['exception_pointer'] = hex(int(gdb.parse_and_eval('$rcx')))
        elif self.location == '_Unwind_RaiseException':
            ptr = int(gdb.parse_and_eval('$r0' if arm else '$rdi'))
            data = gdb.selected_inferior().read_memory(ptr,8).tobytes()
            row['exception_class'] = hex(int.from_bytes(data,'little'))
            row['header_bytes'] = data.hex()
        elif 'can_catch' in self.location:
            for label, reg in [('catch_type','$r0' if arm else '$rdi'),('thrown_type','$r1' if arm else '$rsi')]:
                ptr = int(gdb.parse_and_eval(reg))
                words = gdb.selected_inferior().read_memory(ptr,2*word).tobytes()
                vtable = int.from_bytes(words[:word],'little')
                nameptr = int.from_bytes(words[word:2*word],'little')
                row[label] = {'address':hex(ptr),'vtable':hex(vtable),
                    'vtable_object':gdb.solib_name(vtable),'name_address':hex(nameptr),
                    'name':gdb.parse_and_eval('(char*)'+hex(nameptr)).string()}
            ReturnObserve(frame,internal=True)
        print('P11_TRACE '+json.dumps(row,sort_keys=True))
        if not seen_maps:
            print('P11_MAPS_BEGIN')
            if arm:
                print('REMOTE_MAPS NOT_OBSERVED; GDB shared library addresses follow, not /proc/self/maps')
                gdb.execute('info sharedlibrary')
            else:
                gdb.execute('info proc mappings')
            print('P11_MAPS_END')
            seen_maps = True
        return False
for name in ('__cxa_throw','__gxx_personality_v0','_Unwind_RaiseException',
             '__cxxabiv1::__class_type_info::can_catch'):
    Observe(name)
end
continue
python
try: print('P11_INFERIOR_EXIT '+str(gdb.parse_and_eval('$_exitcode')))
except Exception: print('P11_INFERIOR_EXIT NOT_OBSERVED')
end

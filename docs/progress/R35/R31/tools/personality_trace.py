import gdb


def register_value(name):
    return int(gdb.parse_and_eval(name))


class PersonalityReturn(gdb.FinishBreakpoint):
    def __init__(self, frame, actions, sequence):
        super().__init__(frame, internal=True)
        self.actions = actions
        self.sequence = sequence

    def stop(self):
        try:
            if self.return_value is None:
                value = register_value("$rax")
            else:
                value = int(self.return_value)
        except Exception as error:
            value = "NOT_AVAILABLE:%s" % error
        gdb.write(
            "R31_PERSONALITY_RETURN sequence=%d actions=%d force=%d return=%s\n"
            % (self.sequence, self.actions, bool(self.actions & 8), value)
        )
        return False


class PersonalityEntry(gdb.Breakpoint):
    sequence = 0

    def stop(self):
        PersonalityEntry.sequence += 1
        sequence = PersonalityEntry.sequence
        try:
            actions = register_value("$rsi")
            exception_class = register_value("$rdx")
        except Exception as error:
            gdb.write(
                "R31_PERSONALITY_ENTRY sequence=%d REGISTER_READ_FAILED=%s\n"
                % (sequence, error)
            )
            return True
        gdb.write(
            "R31_PERSONALITY_ENTRY sequence=%d actions=%d force=%d exception_class=0x%x pc=%s\n"
            % (sequence, actions, bool(actions & 8), exception_class, gdb.newest_frame().pc())
        )
        if actions & 8:
            gdb.write("R31_FORCE_UNWIND_BACKTRACE_BEGIN\n")
            gdb.write(gdb.execute("bt 10", to_string=True))
            gdb.write("R31_FORCE_UNWIND_BACKTRACE_END\n")
        try:
            PersonalityReturn(gdb.newest_frame(), actions, sequence)
        except Exception as error:
            gdb.write(
                "R31_PERSONALITY_RETURN_BREAKPOINT_FAILED sequence=%d error=%s\n"
                % (sequence, error)
            )
            return True
        return False


PersonalityEntry("__gxx_personality_v0")

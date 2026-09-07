// 不依赖 C 运行库的 aarch64 探针：输出自身映射，再返回精确成功标记。
.section .rodata
maps_path:
  .asciz "/proc/self/maps"
pass_text:
  .ascii "RESULT=PASS maps_dump_complete=1\n"
.equ pass_len, . - pass_text

.section .bss
.balign 16
buffer:
  .skip 16384

.section .text
.global _start
.type _start, %function
_start:
  mov x0, #-100
  adr x1, maps_path
  mov x2, #0
  mov x3, #0
  mov x8, #56
  svc #0
  cmp x0, #0
  b.lt fail
  mov x19, x0

read_loop:
  mov x0, x19
  adr x1, buffer
  mov x2, #16384
  mov x8, #63
  svc #0
  cmp x0, #0
  b.le close_file
  mov x2, x0
  mov x0, #1
  adr x1, buffer
  mov x8, #64
  svc #0
  b read_loop

close_file:
  mov x0, x19
  mov x8, #57
  svc #0
  mov x0, #1
  adr x1, pass_text
  mov x2, #pass_len
  mov x8, #64
  svc #0
  mov x0, #0
  mov x8, #93
  svc #0

fail:
  mov x0, #2
  mov x8, #93
  svc #0
.size _start, . - _start

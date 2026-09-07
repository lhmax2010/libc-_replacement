	.arch armv7-a
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"cantunwind_worker.cpp"
	.text
	.align	2
	.global	cantunwind_worker
	.syntax unified
	.arm
	.type	cantunwind_worker, %function
cantunwind_worker:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	.save {fp, lr}
	.setfp fp, sp, #4
	add	fp, sp, #4
	.pad #8
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	movw	r3, #:lower16:worker_entered
	movt	r3, #:upper16:worker_entered
	mov	r2, #1
	dmb	ish
	str	r2, [r3]
.L2:
	bl	pthread_testcancel
	b	.L2
	.fnend
	.size	cantunwind_worker, .-cantunwind_worker
	.ident	"GCC: (Tizen GCC 14.2.0 20240801 1.15) 14.2.0"
	.section	.note.GNU-stack,"",%progbits

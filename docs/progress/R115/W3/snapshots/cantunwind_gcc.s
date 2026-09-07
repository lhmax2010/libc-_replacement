	.cpu arm10tdmi
	.arch armv5t
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"cantunwind_worker.cpp"
	.text
	.align	2
	.global	cantunwind_worker
	.syntax unified
	.arm
	.type	cantunwind_worker, %function
cantunwind_worker:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, fp, lr}
	add	fp, sp, #12
	sub	sp, sp, #8
	str	r0, [fp, #-16]
	ldr	r4, .L3
	mov	r5, #1
	bl	__sync_synchronize
	str	r5, [r4]
.L2:
	bl	pthread_testcancel
	b	.L2
.L4:
	.align	2
.L3:
	.word	worker_entered
	.size	cantunwind_worker, .-cantunwind_worker
	.global	__sync_synchronize
	.ident	"GCC: (Tizen GCC 14.2.0 20240801 1.15) 14.2.0"
	.section	.note.GNU-stack,"",%progbits

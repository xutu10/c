	.file	"fgets_keyboard.c"
	.section	.rodata
.LC0:
	.string	"input your age:"
.LC1:
	.string	"input your name:"
.LC2:
	.string	"age:%s, name:%s/n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movq	stdin(%rip), %rdx
	leaq	-48(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	fgets
	movq	stdin(%rip), %rax
	movq	%rax, %rdi
	call	fflush
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movq	stdin(%rip), %rax
	leaq	-48(%rbp), %rdx
	leaq	3(%rdx), %rcx
	movq	%rax, %rdx
	movl	$9, %esi
	movq	%rcx, %rdi
	call	fgets
	leaq	-48(%rbp), %rax
	leaq	3(%rax), %rdx
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L3
	call	__stack_chk_fail
.L3:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits

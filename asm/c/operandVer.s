	.file	"operand.c"
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
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$1, -16(%rbp)
	movl	$2, -12(%rbp)
	movl	$3, -8(%rbp)
	movl	$0, -4(%rbp)
	movl	-4(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jle	.L2
	movl	-12(%rbp), %eax
	cltd
	idivl	-16(%rbp)
	movl	%eax, -4(%rbp)
	jmp	.L3
.L2:
	movl	-4(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jge	.L4
	movl	-12(%rbp), %eax
	cltd
	idivl	-16(%rbp)
	movl	%eax, -4(%rbp)
	jmp	.L3
.L4:
	movl	-4(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jne	.L5
	movl	-12(%rbp), %eax
	cltd
	idivl	-16(%rbp)
	movl	%eax, -4(%rbp)
	jmp	.L3
.L5:
	movl	-4(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jl	.L6
	movl	-12(%rbp), %eax
	cltd
	idivl	-16(%rbp)
	movl	%eax, -4(%rbp)
	jmp	.L3
.L6:
	movl	-4(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jg	.L3
	movl	-12(%rbp), %eax
	cltd
	idivl	-16(%rbp)
	movl	%eax, -4(%rbp)
.L3:
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits

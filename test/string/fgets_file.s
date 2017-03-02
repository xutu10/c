	.file	"fgets_file.c"
	.section	.rodata
.LC0:
	.string	"r"
.LC1:
	.string	"cykbd"
.LC2:
	.string	"can't open the file"
.LC3:
	.string	"length of the file :%d \n"
.LC4:
	.string	"%s"
.LC5:
	.string	"w"
.LC6:
	.string	"des"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	$0, -24(%rbp)
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	call	fopen
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	jne	.L2
	movl	$.LC2, %edi
	call	perror
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movl	$-1, %eax
	jmp	.L3
.L2:
	movq	-24(%rbp), %rax
	movl	$2, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	fseek
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	ftell
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %eax
	addl	$1, %eax
	movl	%eax, %esi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	movl	-28(%rbp), %eax
	cltq
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -16(%rbp)
	movl	-28(%rbp), %eax
	cltq
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	rewind
	jmp	.L4
.L5:
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcat
.L4:
	movl	-28(%rbp), %eax
	leal	1(%rax), %ecx
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	fgets
	testq	%rax, %rax
	jne	.L5
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC4, %edi
	movl	$0, %eax
	call	printf
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movl	$.LC5, %esi
	movl	$.LC6, %edi
	call	fopen
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	fputs
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movl	$0, %eax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits

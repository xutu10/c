	.file	"operand.c"
# GNU C (Ubuntu 4.8.4-2ubuntu1~14.04.3) version 4.8.4 (x86_64-linux-gnu)
#	compiled by GNU C version 4.8.4, GMP version 5.1.3, MPFR version 3.1.2-p3, MPC version 1.0.1
# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -imultiarch x86_64-linux-gnu operand.c -mtune=generic
# -march=x86-64 -fverbose-asm -fstack-protector -Wformat -Wformat-security
# options enabled:  -faggressive-loop-optimizations
# -fasynchronous-unwind-tables -fauto-inc-dec -fbranch-count-reg -fcommon
# -fdelete-null-pointer-checks -fdwarf2-cfi-asm -fearly-inlining
# -feliminate-unused-debug-types -ffunction-cse -fgcse-lm -fgnu-runtime
# -fgnu-unique -fident -finline-atomics -fira-hoist-pressure
# -fira-share-save-slots -fira-share-spill-slots -fivopts
# -fkeep-static-consts -fleading-underscore -fmath-errno
# -fmerge-debug-strings -fmove-loop-invariants -fpeephole
# -fprefetch-loop-arrays -freg-struct-return
# -fsched-critical-path-heuristic -fsched-dep-count-heuristic
# -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
# -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
# -fsched-stalled-insns-dep -fshow-column -fsigned-zeros
# -fsplit-ivs-in-unroller -fstack-protector -fstrict-volatile-bitfields
# -fsync-libcalls -ftrapping-math -ftree-coalesce-vars -ftree-cselim
# -ftree-forwprop -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
# -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop -ftree-pta
# -ftree-reassoc -ftree-scev-cprop -ftree-slp-vectorize
# -ftree-vect-loop-version -funit-at-a-time -funwind-tables -fverbose-asm
# -fzero-initialized-in-bss -m128bit-long-double -m64 -m80387
# -maccumulate-outgoing-args -malign-stringops -mfancy-math-387
# -mfp-ret-in-387 -mfxsr -mglibc -mieee-fp -mlong-double-80 -mmmx -mno-sse4
# -mpush-args -mred-zone -msse -msse2 -mtls-direct-seg-refs

	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)	# argc, argc
	movq	%rsi, -32(%rbp)	# argv, argv
	movl	$0, -16(%rbp)	#, a
	movl	$2, -12(%rbp)	#, b
	movl	$3, -8(%rbp)	#, c
	movl	$1, -4(%rbp)	#, d
	movl	-8(%rbp), %eax	# c, tmp67
	movl	-12(%rbp), %edx	# b, tmp68
	addl	%edx, %eax	# tmp68, tmp66
	movl	%eax, -16(%rbp)	# tmp66, a
	movl	-12(%rbp), %eax	# b, tmp70
	imull	-8(%rbp), %eax	# c, tmp69
	movl	%eax, -16(%rbp)	# tmp69, a
	movl	-12(%rbp), %eax	# b, tmp75
	movl	-8(%rbp), %edx	# c, tmp76
	subl	%eax, %edx	# tmp75, tmp74
	movl	%edx, %eax	# tmp74, tmp74
	movl	%eax, -16(%rbp)	# tmp74, a
	movl	-12(%rbp), %eax	# b, tmp77
	imull	-8(%rbp), %eax	# c, D.1735
	movl	-4(%rbp), %edx	# d, tmp81
	subl	%eax, %edx	# D.1735, tmp80
	movl	%edx, %eax	# tmp80, tmp80
	movl	%eax, -16(%rbp)	# tmp80, a
	movl	-4(%rbp), %eax	# d, tmp82
	imull	-12(%rbp), %eax	# b, D.1735
	imull	-8(%rbp), %eax	# c, tmp83
	movl	%eax, -16(%rbp)	# tmp83, a
	movl	$0, %eax	#, D.1735
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits

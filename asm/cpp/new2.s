	.file	"new.cpp"
# GNU C++ (Ubuntu 4.8.4-2ubuntu1~14.04.3) version 4.8.4 (x86_64-linux-gnu)
#	compiled by GNU C version 4.8.4, GMP version 5.1.3, MPFR version 3.1.2-p3, MPC version 1.0.1
# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -imultiarch x86_64-linux-gnu -D_GNU_SOURCE new.cpp
# -mtune=generic -march=x86-64 -fverbose-asm -fstack-protector -Wformat
# -Wformat-security
# options enabled:  -faggressive-loop-optimizations
# -fasynchronous-unwind-tables -fauto-inc-dec -fbranch-count-reg -fcommon
# -fdelete-null-pointer-checks -fdwarf2-cfi-asm -fearly-inlining
# -feliminate-unused-debug-types -fexceptions -ffunction-cse -fgcse-lm
# -fgnu-runtime -fgnu-unique -fident -finline-atomics -fira-hoist-pressure
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

	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.text._ZN6OutputC2Eiii,"axG",@progbits,_ZN6OutputC5Eiii,comdat
	.align 2
	.weak	_ZN6OutputC2Eiii
	.type	_ZN6OutputC2Eiii, @function
_ZN6OutputC2Eiii:
.LFB972:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)	# this, this
	movl	%esi, -12(%rbp)	# a, a
	movl	%edx, -16(%rbp)	# b, b
	movl	%ecx, -20(%rbp)	# c, c
	movq	-8(%rbp), %rax	# this, tmp59
	movl	-12(%rbp), %edx	# a, tmp60
	movl	%edx, (%rax)	# tmp60, this_1(D)->value
	movq	-8(%rbp), %rax	# this, tmp61
	movl	-16(%rbp), %edx	# b, tmp62
	movl	%edx, 4(%rax)	# tmp62, this_1(D)->count
	movq	-8(%rbp), %rax	# this, tmp63
	movl	-20(%rbp), %edx	# c, tmp64
	movl	%edx, 8(%rax)	# tmp64, this_1(D)->a
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE972:
	.size	_ZN6OutputC2Eiii, .-_ZN6OutputC2Eiii
	.weak	_ZN6OutputC1Eiii
	.set	_ZN6OutputC1Eiii,_ZN6OutputC2Eiii
	.section	.text._ZN6Output7displayEv,"axG",@progbits,_ZN6Output7displayEv,comdat
	.align 2
	.weak	_ZN6Output7displayEv
	.type	_ZN6Output7displayEv, @function
_ZN6Output7displayEv:
.LFB974:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%rbx	#
	subq	$24, %rsp	#,
	.cfi_offset 3, -24
	movq	%rdi, -24(%rbp)	# this, this
	movq	-24(%rbp), %rax	# this, tmp63
	movl	4(%rax), %ebx	# this_1(D)->count, D.20954
	movq	-24(%rbp), %rax	# this, tmp64
	movl	(%rax), %eax	# this_1(D)->value, D.20954
	movl	%eax, %esi	# D.20954,
	movl	$_ZSt4cout, %edi	#,
	call	_ZNSolsEi	#
	movl	%ebx, %esi	# D.20954,
	movq	%rax, %rdi	# D.20955,
	call	_ZNSolsEi	#
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi	#,
	movq	%rax, %rdi	# D.20955,
	call	_ZNSolsEPFRSoS_E	#
	addq	$24, %rsp	#,
	popq	%rbx	#
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE974:
	.size	_ZN6Output7displayEv, .-_ZN6Output7displayEv
	.text
	.globl	main
	.type	main, @function
main:
.LFB975:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%rbx	#
	subq	$40, %rsp	#,
	.cfi_offset 3, -24
	movl	%edi, -36(%rbp)	# argc, argc
	movq	%rsi, -48(%rbp)	# argv, argv
	movl	$12, %edi	#,
	call	_Znwm	#
	movq	%rax, %rbx	# D.20956, o.0
	movl	$1, %ecx	#,
	movl	$8, %edx	#,
	movl	$6, %esi	#,
	movq	%rbx, %rdi	# o.0,
	call	_ZN6OutputC1Eiii	#
	movq	%rbx, -32(%rbp)	# o.0, o
	movl	$12, %edi	#,
	call	_Znwm	#
	movq	%rax, %rbx	# D.20956, oo.1
	movl	$2, %ecx	#,
	movl	$88, %edx	#,
	movl	$66, %esi	#,
	movq	%rbx, %rdi	# oo.1,
	call	_ZN6OutputC1Eiii	#
	movq	%rbx, -24(%rbp)	# oo.1, oo
	movq	-32(%rbp), %rax	# o, tmp65
	movq	%rax, %rdi	# tmp65,
	call	_ZN6Output7displayEv	#
	cmpq	$0, -32(%rbp)	#, o
	je	.L4	#,
	movq	-32(%rbp), %rax	# o, tmp66
	movq	%rax, %rdi	# tmp66,
	call	_ZdaPv	#
.L4:
	movl	$0, %eax	#, D.20957
	addq	$40, %rsp	#,
	popq	%rbx	#
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE975:
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB981:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$16, %rsp	#,
	movl	%edi, -4(%rbp)	# __initialize_p, __initialize_p
	movl	%esi, -8(%rbp)	# __priority, __priority
	cmpl	$1, -4(%rbp)	#, __initialize_p
	jne	.L6	#,
	cmpl	$65535, -8(%rbp)	#, __priority
	jne	.L6	#,
	movl	$_ZStL8__ioinit, %edi	#,
	call	_ZNSt8ios_base4InitC1Ev	#
	movl	$__dso_handle, %edx	#,
	movl	$_ZStL8__ioinit, %esi	#,
	movl	$_ZNSt8ios_base4InitD1Ev, %edi	#,
	call	__cxa_atexit	#
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE981:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB982:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	movl	$65535, %esi	#,
	movl	$1, %edi	#,
	call	_Z41__static_initialization_and_destruction_0ii	#
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE982:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits

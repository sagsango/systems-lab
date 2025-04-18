	.file	"01.c"
	.text
	.globl	cntr
	.bss
	.align 4
	.type	cntr, @object
	.size	cntr, 4
cntr:
	.zero	4
	.text
	.globl	incr
	.type	incr, @function
incr:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
.L3:
	leaq	cntr(%rip), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -32(%rbp)
	movl	-32(%rbp), %eax
	movl	%eax, -36(%rbp)
	movl	-36(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -28(%rbp)
	leaq	cntr(%rip), %rax
	movq	%rax, -16(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, -32(%rbp)
	movl	-32(%rbp), %eax
	movl	%eax, %ecx
	movq	-16(%rbp), %rsi
	leaq	-36(%rbp), %rdx
	movl	(%rdx), %eax
	lock cmpxchgl	%ecx, (%rsi)
	movl	%eax, %ecx
	sete	%al
	testb	%al, %al
	jne	.L2
	movl	%ecx, (%rdx)
.L2:
	xorl	$1, %eax
	testb	%al, %al
	jne	.L3
	movl	$0, %eax
	movq	-8(%rbp), %rdi
	xorq	%fs:40, %rdi
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	incr, .-incr
	.globl	tf
	.type	tf, @function
tf:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L7
.L8:
	movl	$0, %eax
	call	incr
	addl	$1, -4(%rbp)
.L7:
	cmpl	$9999999, -4(%rbp)
	jle	.L8
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	tf, .-tf
	.section	.rodata
	.align 8
.LC0:
	.string	"threads = %d, loops: %d, cntr : %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -112(%rbp)
	jmp	.L11
.L12:
	leaq	-96(%rbp), %rax
	movl	-112(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movl	$0, %ecx
	leaq	tf(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	addl	$1, -112(%rbp)
.L11:
	cmpl	$9, -112(%rbp)
	jle	.L12
	movl	$0, -108(%rbp)
	jmp	.L13
.L14:
	movl	-108(%rbp), %eax
	cltq
	movq	-96(%rbp,%rax,8), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	addl	$1, -108(%rbp)
.L13:
	cmpl	$9, -108(%rbp)
	jle	.L14
	leaq	cntr(%rip), %rax
	movq	%rax, -104(%rbp)
	movq	-104(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -116(%rbp)
	movl	-116(%rbp), %eax
	movl	%eax, %ecx
	movl	$10000000, %edx
	movl	$10, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L16
	call	__stack_chk_fail@PLT
.L16:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.5.0-1ubuntu1~22.04) 9.5.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:

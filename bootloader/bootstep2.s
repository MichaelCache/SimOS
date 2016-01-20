	.file	"bootstep2.c"
	.text
	.type	org, @function
org:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
#APP
# 40 "./x86.h" 1
	.org    0x7c70
# 0 "" 2
#NO_APP
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	org, .-org
	.type	waitdisk, @function
waitdisk:
.LFB15:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	nop
.L4:
	movw	$503, -2(%ebp)
	movzwl	-2(%ebp), %eax
	movl	%eax, %edx
#APP
# 46 "./x86.h" 1
	inb %dx, %al
# 0 "" 2
#NO_APP
	movb	%al, -3(%ebp)
	movzbl	-3(%ebp), %eax
	movzbl	%al, %eax
	andl	$192, %eax
	cmpl	$64, %eax
	jne	.L4
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE15:
	.size	waitdisk, .-waitdisk
	.type	readsect, @function
readsect:
.LFB16:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%ebx
	subl	$32, %esp
	.cfi_offset 7, -12
	.cfi_offset 3, -16
	call	waitdisk
	movw	$498, -32(%ebp)
	movb	$1, -38(%ebp)
	movzbl	-38(%ebp), %eax
	movzwl	-32(%ebp), %edx
#APP
# 62 "./x86.h" 1
	outb %al, %dx
# 0 "" 2
#NO_APP
	movl	12(%ebp), %eax
	movzbl	%al, %eax
	movw	$499, -30(%ebp)
	movb	%al, -37(%ebp)
	movzbl	-37(%ebp), %eax
	movzwl	-30(%ebp), %edx
#APP
# 62 "./x86.h" 1
	outb %al, %dx
# 0 "" 2
#NO_APP
	movl	12(%ebp), %eax
	shrl	$8, %eax
	movzbl	%al, %eax
	movw	$500, -28(%ebp)
	movb	%al, -36(%ebp)
	movzbl	-36(%ebp), %eax
	movzwl	-28(%ebp), %edx
#APP
# 62 "./x86.h" 1
	outb %al, %dx
# 0 "" 2
#NO_APP
	movl	12(%ebp), %eax
	shrl	$16, %eax
	movzbl	%al, %eax
	movw	$501, -26(%ebp)
	movb	%al, -35(%ebp)
	movzbl	-35(%ebp), %eax
	movzwl	-26(%ebp), %edx
#APP
# 62 "./x86.h" 1
	outb %al, %dx
# 0 "" 2
#NO_APP
	movl	12(%ebp), %eax
	shrl	$24, %eax
	andl	$15, %eax
	orl	$-32, %eax
	movzbl	%al, %eax
	movw	$502, -24(%ebp)
	movb	%al, -34(%ebp)
	movzbl	-34(%ebp), %eax
	movzwl	-24(%ebp), %edx
#APP
# 62 "./x86.h" 1
	outb %al, %dx
# 0 "" 2
#NO_APP
	movw	$503, -22(%ebp)
	movb	$32, -33(%ebp)
	movzbl	-33(%ebp), %eax
	movzwl	-22(%ebp), %edx
#APP
# 62 "./x86.h" 1
	outb %al, %dx
# 0 "" 2
#NO_APP
	call	waitdisk
	movl	$496, -20(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	$128, -12(%ebp)
	movl	-20(%ebp), %edx
	movl	-16(%ebp), %ecx
	movl	-12(%ebp), %eax
	movl	%ecx, %ebx
	movl	%ebx, %edi
	movl	%eax, %ecx
#APP
# 52 "./x86.h" 1
	cld;repne; insl;
# 0 "" 2
#NO_APP
	movl	%ecx, %eax
	movl	%edi, %ebx
	movl	%ebx, -16(%ebp)
	movl	%eax, -12(%ebp)
	addl	$32, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE16:
	.size	readsect, .-readsect
	.type	readseg, @function
readseg:
.LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	addl	%edx, %eax
	movl	%eax, -4(%ebp)
	movl	16(%ebp), %eax
	andl	$511, %eax
	subl	%eax, 8(%ebp)
	movl	16(%ebp), %eax
	shrl	$9, %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	.L7
.L8:
	movl	8(%ebp), %eax
	movl	-8(%ebp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	readsect
	addl	$512, 8(%ebp)
	addl	$1, -8(%ebp)
.L7:
	movl	8(%ebp), %eax
	cmpl	-4(%ebp), %eax
	jb	.L8
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE17:
	.size	readseg, .-readseg
	.globl	bootstep2
	.type	bootstep2, @function
bootstep2:
.LFB18:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	call	org
	movl	$0, 8(%esp)
	movl	$4096, 4(%esp)
	movl	$1048576, (%esp)
	call	readseg
	movl	$1048576, %eax
	movl	(%eax), %eax
	cmpl	$1179403647, %eax
	je	.L10
	jmp	.L11
.L10:
	movl	$1048576, %eax
	movl	28(%eax), %eax
	addl	$1048576, %eax
	movl	%eax, -16(%ebp)
	movl	$1048576, %eax
	movzwl	44(%eax), %eax
	movzwl	%ax, %eax
	sall	$5, %eax
	movl	%eax, %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -12(%ebp)
	jmp	.L12
.L13:
	movl	-16(%ebp), %eax
	movl	4(%eax), %edx
	movl	-16(%ebp), %eax
	movl	20(%eax), %eax
	movl	-16(%ebp), %ecx
	movl	8(%ecx), %ecx
	andl	$16777215, %ecx
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	%ecx, (%esp)
	call	readseg
	addl	$32, -16(%ebp)
.L12:
	movl	-16(%ebp), %eax
	cmpl	-12(%ebp), %eax
	jb	.L13
	movl	$1048576, %eax
	movl	24(%eax), %eax
	andl	$16777215, %eax
	call	*%eax
.L11:
	movw	$-30208, -24(%ebp)
	movw	$-30208, -22(%ebp)
	movzwl	-22(%ebp), %eax
	movzwl	-24(%ebp), %edx
#APP
# 67 "./x86.h" 1
	outw %ax, %dx
# 0 "" 2
#NO_APP
	movw	$-30208, -20(%ebp)
	movw	$-29184, -18(%ebp)
	movzwl	-18(%ebp), %eax
	movzwl	-20(%ebp), %edx
#APP
# 67 "./x86.h" 1
	outw %ax, %dx
# 0 "" 2
#NO_APP
.L14:
	jmp	.L14
	.cfi_endproc
.LFE18:
	.size	bootstep2, .-bootstep2
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits

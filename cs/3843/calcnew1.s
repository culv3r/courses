	.file	"calcnew.c"
	.text
.globl calcnew
	.type	calcnew, @function
calcnew:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	16(%ebp), %ecx
	sall    $2, %edx
	addl    %eax, %edx
	movl	12(%ebp), %eax
	sall    $3, %eax
	subl    $1, %eax
	movl	%ecx, %edx
	sall    $4, %edx
	subl    %ecx, %edx
	subl    %ecx, %edx
	addl	%edx, %eax
	popl	%ebp
	ret
	.size	calcnew, .-calcnew
	.ident	"GCC: (Ubuntu 4.3.3-5ubuntu4) 4.3.3"
	.section	.note.GNU-stack,"",@progbits

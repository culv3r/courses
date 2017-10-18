	.file	"hello.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	mov	DWORD PTR -28[rbp], 51266
	lea	rax, -28[rbp]
	mov	QWORD PTR -24[rbp], rax
	mov	eax, DWORD PTR -28[rbp]
	lea	rdx, -16[rbp]
	mov	rcx, QWORD PTR -24[rbp]
	mov	rsi, rcx
	mov	edi, eax
	call	func
	mov	eax, 1
	mov	rsi, QWORD PTR -8[rbp]
	xor	rsi, QWORD PTR fs:40
	je	.L3
	call	__stack_chk_fail@PLT
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.globl	func
	.type	func, @function
func:
.LFB1:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR -4[rbp], edi
	mov	QWORD PTR -16[rbp], rsi
	mov	QWORD PTR -24[rbp], rdx
	mov	DWORD PTR -4[rbp], 74565
	mov	rax, QWORD PTR -16[rbp]
	mov	DWORD PTR [rax], -1
	lea	rax, -4[rbp]
	mov	QWORD PTR -16[rbp], rax
	mov	rax, QWORD PTR -16[rbp]
	mov	DWORD PTR [rax], -1095041334
	mov	rax, QWORD PTR -24[rbp]
	add	rax, 5
	mov	BYTE PTR [rax], 13
	mov	rax, QWORD PTR -24[rbp]
	add	rax, 6
	mov	BYTE PTR [rax], -16
	mov	eax, 1
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	func, .-func
	.data
	.align 4
	.type	tmpIS.2255, @object
	.size	tmpIS.2255, 4
tmpIS.2255:
	.long	60
	.ident	"GCC: (GNU) 7.2.0"
	.section	.note.GNU-stack,"",@progbits

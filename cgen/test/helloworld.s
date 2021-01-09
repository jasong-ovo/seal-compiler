# start of generated code
	.section		.rodata
.LC0:
	.string	"Hello world!"
	.text
	.globl	main
	.type	main, @function
main:
	pushq	 %rbp
	movq	%rsp, %rbp
	pushq	 %rbx
	pushq	 %r10
	pushq	 %r11
	pushq	 %r12
	pushq	 %r13
	pushq	 %r14
	pushq	 %r15
	movq	$.LC0, -64(%rbp)
	subq	$8, %rsp
	movq	-64(%rbp), %rdi
	subq	$8, %rsp
	movl	$0, %eax
	call	 printf
	movq	-64(%rbp), %rax
	movq	$-56, %rbx
	addq	%rbp, %rbx
	movq	%rbx, %rsp
	popq	 %r15
	popq	 %r14
	popq	 %r13
	popq	 %r12
	popq	 %r11
	popq	 %r10
	popq	 %rbx
	leave	
	ret	
	.size	main, .-main

# end of generated code

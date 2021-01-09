# start of generated code
	.section		.rodata
.LC0:
	.string	"ind(%lld) = %lld \n"
	.text
	.globl	ind
	.type	ind, @function
ind:
	pushq	 %rbp
	movq	%rsp, %rbp
	pushq	 %rbx
	pushq	 %r10
	pushq	 %r11
	pushq	 %r12
	pushq	 %r13
	pushq	 %r14
	pushq	 %r15
	subq	$8, %rsp
	movq	%rdi, -64(%rbp)
	subq	$8, %rsp
	movq	%rsi, -72(%rbp)
	subq	$8, %rsp
	subq	$8, %rsp
	movq	$1, -96(%rbp)
	subq	$8, %rsp
	movq	-96(%rbp), %rbx
	movq	%rbx, -80(%rbp)
	movq	-64(%rbp), %rbx
	movq	%rbx, -104(%rbp)
	subq	$8, %rsp
	movq	-104(%rbp), %rbx
	movq	%rbx, -88(%rbp)
	jmp	 .POS1
.POS0:
	movq	-88(%rbp), %rbx
	movq	%rbx, -112(%rbp)
	subq	$8, %rsp
	movq	-64(%rbp), %rbx
	movq	%rbx, -120(%rbp)
	subq	$8, %rsp
	movq	-112(%rbp), %rax
	movq	-120(%rbp), %rbx
	imulq	%rax, %rbx
	subq	$8, %rsp
	movq	%rbx, -128(%rbp)
	movq	-72(%rbp), %rbx
	movq	%rbx, -136(%rbp)
	subq	$8, %rsp
	movq	-128(%rbp), %rax
	cqto	
	movq	-136(%rbp), %rcx
	idivq	%rcx
	subq	$8, %rsp
	movq	%rdx, -144(%rbp)
	movq	-144(%rbp), %rbx
	movq	%rbx, -88(%rbp)
	movq	-80(%rbp), %rbx
	movq	%rbx, -152(%rbp)
	subq	$8, %rsp
	movq	$1, -160(%rbp)
	subq	$8, %rsp
	movq	-152(%rbp), %rax
	movq	-160(%rbp), %rbx
	addq	%rax, %rbx
	subq	$8, %rsp
	movq	%rbx, -168(%rbp)
	movq	-168(%rbp), %rbx
	movq	%rbx, -80(%rbp)
.POS1:
	movq	-88(%rbp), %rbx
	movq	%rbx, -176(%rbp)
	subq	$8, %rsp
	movq	$1, -184(%rbp)
	subq	$8, %rsp
	movq	-176(%rbp), %rax
	movq	-184(%rbp), %rbx
	cmpq	%rax, %rbx
	jne	 .POS3
	movq	$0, %rax
	jmp	 .POS4
.POS3:
	movq	$1, %rax
.POS4:
	subq	$8, %rsp
	movq	%rax, -192(%rbp)
	movq	-192(%rbp), %rax
	testq	%rax, %rax
	jnz	 .POS0
.POS2:
	movq	-80(%rbp), %rbx
	movq	%rbx, -200(%rbp)
	subq	$8, %rsp
	movq	-200(%rbp), %rax
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
	.size	ind, .-ind
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
	subq	$8, %rsp
	movq	$1, -72(%rbp)
	subq	$8, %rsp
	movq	-72(%rbp), %rbx
	movq	%rbx, -64(%rbp)
	jmp	 .POS6
.POS5:
	movq	$.LC0, -80(%rbp)
	subq	$8, %rsp
	movq	-64(%rbp), %rbx
	movq	%rbx, -88(%rbp)
	subq	$8, %rsp
	movq	-64(%rbp), %rbx
	movq	%rbx, -96(%rbp)
	subq	$8, %rsp
	movq	$23, -104(%rbp)
	subq	$8, %rsp
	movq	-96(%rbp), %rdi
	movq	-104(%rbp), %rsi
	call	 ind
	movq	%rax, -112(%rbp)
	subq	$8, %rsp
	movq	-80(%rbp), %rdi
	movq	-88(%rbp), %rsi
	movq	-112(%rbp), %rdx
	subq	$8, %rsp
	movl	$0, %eax
	call	 printf
	movq	-64(%rbp), %rbx
	movq	%rbx, -120(%rbp)
	subq	$8, %rsp
	movq	$1, -128(%rbp)
	subq	$8, %rsp
	movq	-120(%rbp), %rax
	movq	-128(%rbp), %rbx
	addq	%rax, %rbx
	subq	$8, %rsp
	movq	%rbx, -136(%rbp)
	movq	-136(%rbp), %rbx
	movq	%rbx, -64(%rbp)
.POS6:
	movq	-64(%rbp), %rbx
	movq	%rbx, -144(%rbp)
	subq	$8, %rsp
	movq	$23, -152(%rbp)
	subq	$8, %rsp
	movq	-144(%rbp), %rax
	movq	-152(%rbp), %rbx
	cmpq	%rax, %rbx
	jg	 .POS8
	movq	$0, %rax
	jmp	 .POS9
.POS8:
	movq	$1, %rax
.POS9:
	subq	$8, %rsp
	movq	%rax, -160(%rbp)
	movq	-160(%rbp), %rax
	testq	%rax, %rax
	jnz	 .POS5
.POS7:
	movq	-160(%rbp), %rax
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

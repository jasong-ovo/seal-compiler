# start of generated code
	.section		.rodata
.LC0:
	.string	"fib(%lld) = %lld \n"
	.text
	.globl	fib
	.type	fib, @function
fib:
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
	movq	-64(%rbp), %rbx
	movq	%rbx, -72(%rbp)
	subq	$8, %rsp
	movq	$2, -80(%rbp)
	subq	$8, %rsp
	movq	-72(%rbp), %rax
	movq	-80(%rbp), %rbx
	cmpq	%rax, %rbx
	jge	 .POS0
	movq	$0, %rax
	jmp	 .POS1
.POS0:
	movq	$1, %rax
.POS1:
	subq	$8, %rsp
	movq	%rax, -88(%rbp)
	movq	-88(%rbp), %rax
	testq	%rax, %rax
	jz	 .POS3
	movq	$1, -96(%rbp)
	subq	$8, %rsp
	movq	-96(%rbp), %rax
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
	jmp	 .POS4
.POS3:
.POS4:
	movq	-64(%rbp), %rbx
	movq	%rbx, -104(%rbp)
	subq	$8, %rsp
	movq	$1, -112(%rbp)
	subq	$8, %rsp
	movq	-104(%rbp), %rax
	movq	-112(%rbp), %rbx
	subq	%rbx, %rax
	subq	$8, %rsp
	movq	%rax, -120(%rbp)
	movq	-120(%rbp), %rdi
	call	 fib
	movq	%rax, -128(%rbp)
	subq	$8, %rsp
	movq	-64(%rbp), %rbx
	movq	%rbx, -136(%rbp)
	subq	$8, %rsp
	movq	$2, -144(%rbp)
	subq	$8, %rsp
	movq	-136(%rbp), %rax
	movq	-144(%rbp), %rbx
	subq	%rbx, %rax
	subq	$8, %rsp
	movq	%rax, -152(%rbp)
	movq	-152(%rbp), %rdi
	call	 fib
	movq	%rax, -160(%rbp)
	subq	$8, %rsp
	movq	-128(%rbp), %rax
	movq	-160(%rbp), %rbx
	addq	%rax, %rbx
	subq	$8, %rsp
	movq	%rbx, -168(%rbp)
	movq	-168(%rbp), %rax
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
	.size	fib, .-fib
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
	movq	-96(%rbp), %rdi
	call	 fib
	movq	%rax, -104(%rbp)
	subq	$8, %rsp
	movq	-80(%rbp), %rdi
	movq	-88(%rbp), %rsi
	movq	-104(%rbp), %rdx
	subq	$8, %rsp
	movl	$0, %eax
	call	 printf
	movq	-64(%rbp), %rbx
	movq	%rbx, -112(%rbp)
	subq	$8, %rsp
	movq	$1, -120(%rbp)
	subq	$8, %rsp
	movq	-112(%rbp), %rax
	movq	-120(%rbp), %rbx
	addq	%rax, %rbx
	subq	$8, %rsp
	movq	%rbx, -128(%rbp)
	movq	-128(%rbp), %rbx
	movq	%rbx, -64(%rbp)
.POS6:
	movq	-64(%rbp), %rbx
	movq	%rbx, -136(%rbp)
	subq	$8, %rsp
	movq	$15, -144(%rbp)
	subq	$8, %rsp
	movq	-136(%rbp), %rax
	movq	-144(%rbp), %rbx
	cmpq	%rax, %rbx
	jg	 .POS8
	movq	$0, %rax
	jmp	 .POS9
.POS8:
	movq	$1, %rax
.POS9:
	subq	$8, %rsp
	movq	%rax, -152(%rbp)
	movq	-152(%rbp), %rax
	testq	%rax, %rax
	jnz	 .POS5
.POS7:
	movq	-152(%rbp), %rax
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

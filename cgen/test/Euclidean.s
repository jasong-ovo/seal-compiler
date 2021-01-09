# start of generated code
	.section		.rodata
.LC0:
	.string	"gcd(23398, 14567) = %lld \n "
	.text
	.globl	euclidean
	.type	euclidean, @function
euclidean:
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
	movq	-64(%rbp), %rbx
	movq	%rbx, -88(%rbp)
	subq	$8, %rsp
	movq	-72(%rbp), %rbx
	movq	%rbx, -96(%rbp)
	subq	$8, %rsp
	movq	-88(%rbp), %rax
	movq	-96(%rbp), %rbx
	cmpq	%rax, %rbx
	jg	 .POS0
	movq	$0, %rax
	jmp	 .POS1
.POS0:
	movq	$1, %rax
.POS1:
	subq	$8, %rsp
	movq	%rax, -104(%rbp)
	movq	-104(%rbp), %rax
	testq	%rax, %rax
	jz	 .POS3
	movq	-64(%rbp), %rbx
	movq	%rbx, -112(%rbp)
	subq	$8, %rsp
	movq	-112(%rbp), %rbx
	movq	%rbx, -80(%rbp)
	movq	-72(%rbp), %rbx
	movq	%rbx, -120(%rbp)
	subq	$8, %rsp
	movq	-120(%rbp), %rbx
	movq	%rbx, -64(%rbp)
	movq	-80(%rbp), %rbx
	movq	%rbx, -128(%rbp)
	subq	$8, %rsp
	movq	-128(%rbp), %rbx
	movq	%rbx, -72(%rbp)
	jmp	 .POS4
.POS3:
.POS4:
	jmp	 .POS6
.POS5:
	movq	-72(%rbp), %rbx
	movq	%rbx, -136(%rbp)
	subq	$8, %rsp
	movq	-136(%rbp), %rbx
	movq	%rbx, -80(%rbp)
	movq	-64(%rbp), %rbx
	movq	%rbx, -144(%rbp)
	subq	$8, %rsp
	movq	-72(%rbp), %rbx
	movq	%rbx, -152(%rbp)
	subq	$8, %rsp
	movq	-144(%rbp), %rax
	cqto	
	movq	-152(%rbp), %rcx
	idivq	%rcx
	subq	$8, %rsp
	movq	%rdx, -160(%rbp)
	movq	-160(%rbp), %rbx
	movq	%rbx, -72(%rbp)
	movq	-80(%rbp), %rbx
	movq	%rbx, -168(%rbp)
	subq	$8, %rsp
	movq	-168(%rbp), %rbx
	movq	%rbx, -64(%rbp)
.POS6:
	movq	-64(%rbp), %rbx
	movq	%rbx, -176(%rbp)
	subq	$8, %rsp
	movq	-72(%rbp), %rbx
	movq	%rbx, -184(%rbp)
	subq	$8, %rsp
	movq	-176(%rbp), %rax
	cqto	
	movq	-184(%rbp), %rcx
	idivq	%rcx
	subq	$8, %rsp
	movq	%rdx, -192(%rbp)
	movq	$0, -200(%rbp)
	subq	$8, %rsp
	movq	-192(%rbp), %rax
	movq	-200(%rbp), %rbx
	cmpq	%rax, %rbx
	jne	 .POS8
	movq	$0, %rax
	jmp	 .POS9
.POS8:
	movq	$1, %rax
.POS9:
	subq	$8, %rsp
	movq	%rax, -208(%rbp)
	movq	-208(%rbp), %rax
	testq	%rax, %rax
	jnz	 .POS5
.POS7:
	movq	-72(%rbp), %rbx
	movq	%rbx, -216(%rbp)
	subq	$8, %rsp
	movq	-216(%rbp), %rax
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
	.size	euclidean, .-euclidean
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
	subq	$8, %rsp
	movq	$23398, -80(%rbp)
	subq	$8, %rsp
	movq	-80(%rbp), %rbx
	movq	%rbx, -64(%rbp)
	movq	$14567, -88(%rbp)
	subq	$8, %rsp
	movq	-88(%rbp), %rbx
	movq	%rbx, -72(%rbp)
	movq	$.LC0, -96(%rbp)
	subq	$8, %rsp
	movq	-64(%rbp), %rbx
	movq	%rbx, -104(%rbp)
	subq	$8, %rsp
	movq	-72(%rbp), %rbx
	movq	%rbx, -112(%rbp)
	subq	$8, %rsp
	movq	-104(%rbp), %rdi
	movq	-112(%rbp), %rsi
	call	 euclidean
	movq	%rax, -120(%rbp)
	subq	$8, %rsp
	movq	-96(%rbp), %rdi
	movq	-120(%rbp), %rsi
	subq	$8, %rsp
	movl	$0, %eax
	call	 printf
	movq	-120(%rbp), %rax
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

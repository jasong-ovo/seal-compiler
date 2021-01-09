# start of generated code
	.section		.rodata
.LC0:
	.string	"tan = %f\n"
	.text
	.globl	tan
	.type	tan, @function
tan:
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
	movq	%xmm0, -64(%rbp)
	subq	$8, %rsp
	movq	%xmm1, -72(%rbp)
	subq	$8, %rsp
	movq	%xmm2, -80(%rbp)
	subq	$8, %rsp
	movq	%xmm3, -88(%rbp)
	movq	-64(%rbp), %xmm0
	movq	%xmm0, -96(%rbp)
	subq	$8, %rsp
	movq	-80(%rbp), %xmm0
	movq	%xmm0, -104(%rbp)
	subq	$8, %rsp
	movq	-96(%rbp), %xmm0
	movq	-104(%rbp), %xmm1
	ucomisd	%xmm0, %xmm1
	je	 .POS0
	movq	$0, %rax
	jmp	 .POS1
.POS0:
	movq	$1, %rax
.POS1:
	subq	$8, %rsp
	movq	%rax, -112(%rbp)
	movq	-112(%rbp), %rax
	testq	%rax, %rax
	jz	 .POS3
	subq	$8, %rsp
	movq	$0, %rax
	movq	%rax, -120(%rbp)
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
	jmp	 .POS4
.POS3:
.POS4:
	movq	-88(%rbp), %xmm0
	movq	%xmm0, -128(%rbp)
	subq	$8, %rsp
	movq	-72(%rbp), %xmm0
	movq	%xmm0, -136(%rbp)
	subq	$8, %rsp
	movq	-128(%rbp), %xmm0
	movq	-136(%rbp), %xmm1
	subsd	%xmm1, %xmm0
	subq	$8, %rsp
	movq	%xmm0, -144(%rbp)
	movq	-80(%rbp), %xmm0
	movq	%xmm0, -152(%rbp)
	subq	$8, %rsp
	movq	-64(%rbp), %xmm0
	movq	%xmm0, -160(%rbp)
	subq	$8, %rsp
	movq	-152(%rbp), %xmm0
	movq	-160(%rbp), %xmm1
	subsd	%xmm1, %xmm0
	subq	$8, %rsp
	movq	%xmm0, -168(%rbp)
	movq	-144(%rbp), %xmm0
	movq	-168(%rbp), %xmm1
	divsd	%xmm1, %xmm0
	subq	$8, %rsp
	movq	%xmm0, -176(%rbp)
	movq	-176(%rbp), %rax
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
	.size	tan, .-tan
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
	subq	$8, %rsp
	subq	$8, %rsp
	subq	$8, %rsp
	movq	$4615063718147915776, %rax
	movq	%rax, -96(%rbp)
	movq	-96(%rbp), %xmm0
	movq	%xmm0, -64(%rbp)
	subq	$8, %rsp
	movq	$4615063718147915776, %rax
	movq	%rax, -104(%rbp)
	movq	-104(%rbp), %xmm0
	movq	%xmm0, -80(%rbp)
	subq	$8, %rsp
	movq	$4617878467915022336, %rax
	movq	%rax, -112(%rbp)
	movq	-112(%rbp), %xmm0
	movq	%xmm0, -72(%rbp)
	subq	$8, %rsp
	movq	$4616752568008179712, %rax
	movq	%rax, -120(%rbp)
	movq	-120(%rbp), %xmm0
	movq	%xmm0, -88(%rbp)
	movq	$.LC0, -128(%rbp)
	subq	$8, %rsp
	movq	-64(%rbp), %xmm0
	movq	%xmm0, -136(%rbp)
	subq	$8, %rsp
	movq	-80(%rbp), %xmm0
	movq	%xmm0, -144(%rbp)
	subq	$8, %rsp
	movq	-72(%rbp), %xmm0
	movq	%xmm0, -152(%rbp)
	subq	$8, %rsp
	movq	-88(%rbp), %xmm0
	movq	%xmm0, -160(%rbp)
	subq	$8, %rsp
	movsd	-136(%rbp), %xmm0
	movsd	-144(%rbp), %xmm1
	movsd	-152(%rbp), %xmm2
	movsd	-160(%rbp), %xmm3
	call	 tan
	movq	%rax, -168(%rbp)
	subq	$8, %rsp
	movq	-128(%rbp), %rdi
	movsd	-168(%rbp), %xmm0
	subq	$8, %rsp
	movl	$1, %eax
	call	 printf
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
	.size	main, .-main

# end of generated code

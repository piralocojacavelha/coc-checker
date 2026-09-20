	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
@feat.00 = 0
	.file	"out.cpp"
	.def	main;
	.scl	2;
	.type	32;
	.endef
	.section	.rdata,"dr"
	.p2align	2, 0x0                          # -- Begin function main
.LCPI0_0:
	.long	0xbf800000                      # float -1
.LCPI0_1:
	.long	0xbe800000                      # float -0.25
.LCPI0_2:
	.long	0x3c23d70a                      # float 0.00999999977
	.text
	.globl	main
	.p2align	4
main:                                   # @main
.seh_proc main
# %bb.0:
	pushq	%rbp
	.seh_pushreg %rbp
	pushq	%rsi
	.seh_pushreg %rsi
	pushq	%rdi
	.seh_pushreg %rdi
	subq	$80, %rsp
	.seh_stackalloc 80
	leaq	80(%rsp), %rbp
	.seh_setframe %rbp, 80
	movaps	%xmm8, -16(%rbp)                # 16-byte Spill
	.seh_savexmm %xmm8, 64
	movaps	%xmm7, -32(%rbp)                # 16-byte Spill
	.seh_savexmm %xmm7, 48
	movaps	%xmm6, -48(%rbp)                # 16-byte Spill
	.seh_savexmm %xmm6, 32
	.seh_endprologue
	xorps	%xmm7, %xmm7
	movb	$1, %dil
	xorps	%xmm6, %xmm6
	movl	$64, %esi
	callq	__main
	movss	.LCPI0_0(%rip), %xmm0           # xmm0 = [-1.0E+0,0.0E+0,0.0E+0,0.0E+0]
	movss	.LCPI0_1(%rip), %xmm1           # xmm1 = [-2.5E-1,0.0E+0,0.0E+0,0.0E+0]
	movss	.LCPI0_2(%rip), %xmm2           # xmm2 = [9.99999977E-3,0.0E+0,0.0E+0,0.0E+0]
	xorps	%xmm3, %xmm3
	jmp	.LBB0_1
	.p2align	4
.LBB0_2:                                #   in Loop: Header=BB0_1 Depth=1
	xorl	%edi, %edi
	xorps	%xmm3, %xmm3
	xorps	%xmm7, %xmm7
	decl	%esi
	je	.LBB0_6
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	testb	$1, %dil
	je	.LBB0_2
# %bb.3:                                #   in Loop: Header=BB0_1 Depth=1
	movaps	%xmm7, %xmm4
	mulps	%xmm7, %xmm4
	movaps	%xmm3, %xmm5
	addss	%xmm0, %xmm5
	mulss	%xmm5, %xmm5
	movaps	%xmm4, %xmm8
	shufps	$85, %xmm4, %xmm8               # xmm8 = xmm8[1,1],xmm4[1,1]
	addss	%xmm5, %xmm8
	addss	%xmm4, %xmm8
	xorps	%xmm4, %xmm4
	sqrtss	%xmm8, %xmm4
	addss	%xmm1, %xmm4
	ucomiss	%xmm4, %xmm2
	ja	.LBB0_2
# %bb.4:                                #   in Loop: Header=BB0_1 Depth=1
	addss	%xmm4, %xmm3
	mulss	%xmm6, %xmm4
	shufps	$0, %xmm4, %xmm4                # xmm4 = xmm4[0,0,0,0]
	addps	%xmm4, %xmm7
	movb	$1, %dil
	decl	%esi
	jne	.LBB0_1
.LBB0_6:
	xorb	$1, %dil
	movzbl	%dil, %edx
	leaq	.L.str(%rip), %rcx
	callq	printf
	xorl	%eax, %eax
	movaps	-48(%rbp), %xmm6                # 16-byte Reload
	movaps	-32(%rbp), %xmm7                # 16-byte Reload
	movaps	-16(%rbp), %xmm8                # 16-byte Reload
	.seh_startepilogue
	addq	$80, %rsp
	popq	%rdi
	popq	%rsi
	popq	%rbp
	.seh_endepilogue
	retq
	.seh_endproc
                                        # -- End function
	.section	.rdata,"dr"
.L.str:                                 # @.str
	.asciz	"%d\n"

	.section	.debug$S,"dr"
	.p2align	2, 0x0
	.long	4                               # Debug section magic
	.long	241
	.long	.Ltmp1-.Ltmp0                   # Subsection size
.Ltmp0:
	.short	.Ltmp3-.Ltmp2                   # Record length
.Ltmp2:
	.short	4353                            # Record kind: S_OBJNAME
	.long	0                               # Signature
	.byte	0                               # Object name
	.p2align	2, 0x0
.Ltmp3:
	.short	.Ltmp5-.Ltmp4                   # Record length
.Ltmp4:
	.short	4412                            # Record kind: S_COMPILE3
	.long	1                               # Flags and language
	.short	208                             # CPUType
	.short	21                              # Frontend version
	.short	1
	.short	8
	.short	0
	.short	21018                           # Backend version
	.short	0
	.short	0
	.short	0
	.asciz	"clang version 21.1.8 (https://github.com/msys2/MSYS2-packages 6774da8e8412d17da344f6b18223f20f4f9ba38f)" # Null-terminated compiler version string
	.p2align	2, 0x0
.Ltmp5:
.Ltmp1:
	.p2align	2, 0x0
	.addrsig

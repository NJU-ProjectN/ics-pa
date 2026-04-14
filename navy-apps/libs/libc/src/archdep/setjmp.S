/*
  This file contains setjmp/longjmp for all supported ISAs.
*/

#ifdef __ISA_X86__
#
# our buffer looks like:
#  eax,ebx,ecx,edx,esi,edi,esp,ebp,pc
.globl  setjmp
setjmp:
  pushl  %ebx
  movl  8(%esp), %ebx
  movl  %eax, (%ebx)
  popl  %eax
  movl  %eax, 4(%ebx)
  movl  %ecx, 8(%ebx)
  movl  %edx, 12(%ebx)
  movl  %esi, 16(%ebx)
  movl  %edi, 20(%ebx)
  movl  %esp, 24(%ebx)
  movl  %ebp, 28(%ebx)
  movl  (%esp), %eax
  movl  %eax, 32(%ebx)
  movl -4(%esp), %ebx # EBX is caller saved!
  xorl  %eax, %eax
  ret

#
# our buffer looks like:
#  eax,ebx,ecx,edx,esi,edi,esp,ebp,pc
#
# _longjmp is called with two parameters:  jmp_buf*,int
# jmp_buf* is at 4(%esp), int is at 8(%esp)
# retaddr is, of course, at (%esp)

.globl  longjmp
longjmp:
  movl  4(%esp), %ebx  # address of buf
  movl  8(%esp), %eax  # store return value

  movl  24(%ebx), %esp  # restore stack
  movl  32(%ebx), %edi
# Next line sets up return address.
  movl  %edi, 0(%esp)  
  movl  8(%ebx), %ecx
  movl  12(%ebx), %edx
  movl  16(%ebx), %esi
  movl  20(%ebx), %edi
  movl  28(%ebx), %ebp
  movl  4(%ebx), %ebx
  testl  %eax,%eax
  jne  bye
  incl  %eax    # eax hold 0 if we are here
bye:
  ret
#endif

#ifdef __ISA_MIPS32__
/* This is a simple version of setjmp and longjmp.

   This version does NOT save the floating point register, which is
   wrong, but I don't know how to cleanly handle machines without a
   floating point coprocessor.

   Ian Lance Taylor, Cygnus Support, 13 May 1993.  */

/* int setjmp (jmp_buf);  */
  .globl	setjmp
  .ent	setjmp
setjmp:
  .frame	$sp,0,$31

  sw	$16,0($4)	/* $s0 */
  sw	$17,4($4)	/* $s1 */
  sw	$18,8($4)	/* $s2 */
  sw	$19,12($4)	/* $s3 */
  sw	$20,16($4)	/* $s4 */
  sw	$21,20($4)	/* $s5 */
  sw	$22,24($4)	/* $s6 */
  sw	$23,28($4)	/* $s7 */
  sw	$30,32($4)	/* $s8 */

  sw	$sp,36($4)
  sw	$31,40($4)

  move	$2,$0

  j	$31

  .end	setjmp

/* volatile void longjmp (jmp_buf, int);  */
  .globl	longjmp
  .ent	longjmp
longjmp:
  .frame	$sp,0,$31

  lw	$16,0($4)	/* $s0 */
  lw	$17,4($4)	/* $s1 */
  lw	$18,8($4)	/* $s2 */
  lw	$19,12($4)	/* $s3 */
  lw	$20,16($4)	/* $s4 */
  lw	$21,20($4)	/* $s5 */
  lw	$22,24($4)	/* $s6 */
  lw	$23,28($4)	/* $s7 */
  lw	$30,32($4)	/* $s8 */

  lw	$sp,36($4)
  lw	$31,40($4)

  bne	$5,$0,1f
  li	$5,1
1:
  move	$2,$5

  j	$31

  .end longjmp
#endif

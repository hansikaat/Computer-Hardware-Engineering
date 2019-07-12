  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line

  .text
  .global hexasc
  .global delay
  .global time2string
    hexasc:

       andi    $t0, $a0, 0xf

       ble     $t0, 9, numbers
       nop
       ble     $t0, 15, letters
       nop

  numbers:

       addi     $v0, $t0, 0x30
       jr      $ra
       nop
  letters:

       addi    $v0, $t0, 0x37
       jr     $ra
       nop

   delay:
      PUSH   $ra
      li     $t0, 0
      li     $t1, 4711

    while:
      ble    $a0, 0,finish
      nop
      sub    $a0, $a0, 1

    for:
      beq    $t0, $t1, while
      nop
      addi   $t0, $t0, 1
      j      for
      nop

    finish:
      POP   $ra
      jr    $ra
      nop


  time2string:

       PUSH    $s1
       PUSH    $ra
       move    $s1, $a0

       andi    $t0, $a1, 0xf000
       srl     $a0, $t0, 12
       jal     hexasc
       nop
       move    $a2, $v0
       sb      $a2, 0($s1)

       andi    $t0, $a1, 0x0f00
       srl     $a0, $t0, 8
       jal     hexasc
       nop
       move    $a2, $v0
       sb      $a2, 1($s1)

       li      $t0, 0x3a
       sb      $t0, 2($s1)

       andi    $t0, $a1, 0x00f0
       srl     $a0, $t0, 4
       jal     hexasc
       nop
       move    $a2, $v0
       sb      $a2, 3($s1)

       andi    $t0, $a1, 0x000f
       srl     $a0, $t0, 0
       jal     hexasc
       nop
       move    $a2, $v0
       sb      $a2, 4($s1)

       li      $t0, 0x00
       sb      $t0, 5($s1)

   end:

       POP     $ra
       POP     $s1

       jr      $ra
       nop

       j       end
       nop

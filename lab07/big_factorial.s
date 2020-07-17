# Big factorial function
# n < 1 yields n! = 1
# $s0 is used for n
# we use an s register because the convention is their value
# is preserved across function calls
# f is in $t0

main:
    addi    $sp, $sp, -8  # create stack frame
    sw      $ra, 4($sp)   # save return address
    sw      $s0, 0($sp)   # save $s0

    li      $s0, 0
    la      $a0, msg1
    li      $v0, 4
    syscall            # printf(Enter n: ")

    li      $v0, 5
    syscall            # scanf("%d", &n)
    move    $s0, $v0

    move    $a0, $s0     # factorial(n)
    jal     factorial    #
    move    $t0, $v0     #

    # move    $a0, $v0
    # li      $v0, 1
    # syscall            # printf ("%d", n)
    # li      $a0, '['
    # li      $v0, 11
    # syscall            # printf ("%d", n)

    move    $a0, $s0
    li      $v0, 1
    syscall            # printf ("%d", n)

    la      $a0, msg2
    li      $v0, 4
    syscall            # printf("! = ")

    move    $t1, $t0
main_print_loop:
    blez    $t1, main_print_loop_end

    sub     $t1, $t1, 1
    lb      $a0, digits($t1)
    li      $v0, 1
    syscall

    b       main_print_loop
main_print_loop_end:
    li      $a0, '\n'     # printf("%c", '\n');
    li      $v0, 11
    syscall

                          # clean up stack frame
    lw      $s0, 0($sp)   # restore $s0
    lw      $ra, 4($sp)   # restore $ra
    addi    $sp, $sp, 8   # restore sp

    li      $v0, 0         # return 0
    jr      $ra

    .data
msg1:   .asciiz "Enter n: "
msg2:   .asciiz "! = "
digits: .space 1000         # 1000 digits

    .text
# Big factorial function
# return length of number in digits array
factorial:
factorial_init:
    sub     $sp, $sp, 8
    sw      $s0, 4($sp)
    sw      $ra, 0($sp)

factorial_body:
    move    $s0, $a0        # s0 = n

    li      $a0, '\n'     # printf("%c", '\n');
    li      $v0, 11
    syscall

    li      $t7, 1          # num_digits = 1
    li      $t0, 1          # digits[0] = 1
    sb      $t0, digits

    ble     $s0, 1, factorial_le_1

    # move $a0, $s0
    # li $v0, 1
    # syscall

    li      $t0, 2          # i = 2
factorial_loop:             # while (i <= n)
    bgt     $t0, $s0, factorial_loop_end

    li      $t1, 0          # j = 0
    li      $t2, 0          # carry = 0
    move    $t3, $t7        # copy num_digits
factorial_multiply_loop:    # while (j < num_digits)
    bge     $t1, $t3, factorial_multiply_end

    lb      $t4, digits($t1)
    mul     $t4, $t4, $t0   # x = digits[j] * n
    div     $t6, $t4, 10    # newCarry = x / 10
    rem     $t5, $t4, 10    # y = x % 10
    add     $t5, $t5, $t2   # y = y + carry
    sb      $t5, digits($t1)

    move    $a0, $t6
    li      $v0, 1
    syscall

    move    $t2, $t5        # carry = newCarry
    addi    $t1, $t1, 1
    b       factorial_multiply_loop
factorial_multiply_end:

    beqz    $t2, factorial_if_carry_else
factorial_if_carry:

    sb      $t2, digits($t7)
    addi    $t7, $t7, 1

factorial_if_carry_else:
    addi    $t0, $t0, 1     # i++
    b       factorial_loop

factorial_loop_end:
    b       factorial_post

factorial_le_1:
factorial_post:
    li      $a0, '\n'     # printf("%c", '\n');
    li      $v0, 11
    syscall

    lw      $ra, 0($sp)
    lw      $s0, 4($sp)
    addi    $sp, $sp, 8
    move    $v0, $t7        # return num_digits
    jr  $ra


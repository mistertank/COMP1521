# this code reads a line of input and prints 42
# change it to evaluate the arithmetic expression

main:
    sub     $sp, $sp, 4
    sw      $ra, ($sp)

    la   $a0, line
    la   $a1, 10000
    li   $v0, 8          # fgets(buffer, 256, stdin)
    syscall              #

    la      $s7, line       # s = line;

    jal expression

    move    $a0, $v0
    # li   $a0, 42         # printf("%d", 42);
    li   $v0, 1
    syscall

    li   $a0, '\n'       # printf("%c", '\n');
    li   $v0, 11
    syscall

main_end:
    lw      $ra, ($sp)
    add     $sp, $sp, 4
    li   $v0, 0          # return 0
    jr   $ra


expression:
expression_init:
    sub     $sp, $sp, 12
    sw      $ra, ($sp)
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)

    li   $a0, 1
    li   $v0, 1
    syscall

    li   $a0, '\n'       # printf("%c", '\n');
    li   $v0, 11
    syscall

expression_body:
    jal     term            # left = term()
    move    $s0, $v0

    lb      $t1, ($s7)
    beq     $t1, '+', expression_is_add_end
expression_is_add:
    move    $v0, $s0        # return left;
    jr      $ra

expression_is_add_end:
    addi    $s7, $s7, 1     # s++;

    jal     expression      # right = expression()
    move    $s1, $v0

    add     $v0, $s0, $s1   # return left + right

expression_post:
    lw      $s1, 8($sp)
    lw      $s0, 4($sp)
    lw      $ra, ($sp)
    add     $sp, $sp, 12

expression_end:
    jr   $ra


term:
term_init:
    sub     $sp, $sp, 12
    sw      $ra, ($sp)
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)

    li   $a0, 2
    li   $v0, 1
    syscall

    li   $a0, '\n'       # printf("%c", '\n');
    li   $v0, 11
    syscall

term_body:
    jal     number          # left = number()
    move    $s0, $v0

    li   $a0, 4
    li   $v0, 1
    syscall
    li   $a0, '\n'       # printf("%c", '\n');
    li   $v0, 11
    syscall

    lb      $t1, ($s7)
    beq     $t1, '*', term_is_mul_end
term_is_mul:
    move    $v0, $s0        # return left;
    jr      $ra

term_is_mul_end:
    addi    $s7, $s7, 1     # s++;

    jal     term            # right = term()
    move    $s1, $v0

    mul     $v0, $s0, $s1   # return left * right

term_post:
    lw      $s1, 8($sp)
    lw      $s0, 4($sp)
    lw      $ra, ($sp)
    add     $sp, $sp, 12

term_end:
    jr   $ra


number:

number_init:
number_body:
    li      $t0, 0          # n = 0;

number_loop:
    lb      $t7, ($s7)
    blt     $t7, '0', number_loop_end   # if (*s < '0') break;
    bgt     $t7, '9', number_loop_end   # if (*s > '9') break;

    mul     $t1, $t0, 10    # t1 = n * 10
    sub     $t3, $t7, '0'   # t3 = *s - '0'

    add     $t0, $t1, $t3   # n = 10 * n + *s - '0'
    add     $s7, $s7, 1     # s++

    b       number_loop
number_loop_end:
number_post:
number_end:
    move    $v0, $t0
    jr      $ra

.data
line:
    .space 10000


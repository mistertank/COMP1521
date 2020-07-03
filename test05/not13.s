main:
    li $v0, 5           #   scanf("%d", &x);
    syscall             #
    move $t0, $v0

    li $v0, 5           #   scanf("%d", &y);
    syscall             #
    move $t1, $v0

    move $t2, $t0
    addi $t2, $t2, 1

loop0:
    bge $t2, $t1, end0

    beq $t2, 13, skip_print

    move $a0, $t2
    li $v0, 1
    syscall
    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall

skip_print:
    addi $t2, $t2, 1
    b loop0

end0:

end:

    li $v0, 0           # return 0
    jr $31

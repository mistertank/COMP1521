main:
    li $v0, 5           #   scanf("%d", &x);
    syscall             #
    move $t0, $v0

    li $t1, 0
outer_loop:
    bge $t1, $t0, outer_end

    li $t2, 0

inner_loop:
    bge $t2, $t0, inner_end

    li $a0, '*'
    li $v0, 11
    syscall

    addi $t2, $t2, 1
    b inner_loop

inner_end:

    addi $t1, $t1, 1

    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall

    b outer_loop

outer_end:

end:

    li $v0, 0           # return 0
    jr $31

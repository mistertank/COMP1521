# this code reads 1 integer and prints it
# change it to read integers until their sum is >= 42
# and then print their sum

main:
    li   $s0, 0

loop:
    bge  $s0, 42 loop_end
    li   $v0, 5        #   scanf("%d", &x);
    syscall            #
    move $t0, $v0

    add  $s0, $s0, $t0 # add to sum
    b loop

loop_end:
    move $a0, $s0      #   printf("%d\n", x);
    li   $v0, 1
    syscall

    li   $a0, '\n'     #   printf("%c", '\n');
    li   $v0, 11
    syscall

    li   $v0, 0        # return 0
    jr   $ra

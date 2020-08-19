# this code reads 1 integer and prints it
# change it to read integers until their sum is >= 42
# and then print their sum

main:
    li   $v0, 5        #   scanf("%d", &x);
    syscall            #

    move $a0, $v0      #   printf("%d\n", x);
    li   $v0, 1
    syscall

    li   $a0, '\n'     #   printf("%c", '\n');
    li   $v0, 11
    syscall

    li   $v0, 0        # return 0
    jr   $ra

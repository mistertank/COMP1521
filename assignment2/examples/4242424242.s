main:
    li   $a0, 4242424242  # printf("%d", 4242424242);
    li   $v0, 1
    syscall
    li   $a0,'\n'       # printf("%c", '\n');
    li   $v0, 11
    syscall
    jr   $ra

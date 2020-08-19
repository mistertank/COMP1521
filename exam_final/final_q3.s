# this code reads 1 integer and prints it
# add code so that prints 1 iff
# the least significant (bottom) byte of the number read
# is equal to the 2nd least significant byte
# and it prints 0 otherwise

main:
    li   $v0, 5
    syscall

    # PUT YOUR CODE HERE

    li   $v0, 1
    syscall

    li   $a0, '\n'
    li   $v0, 11
    syscall

    li   $v0, 0
    jr   $ra

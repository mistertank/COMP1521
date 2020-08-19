# this code reads 1 integer and prints it
# add code so that prints 1 iff
# the least significant (bottom) byte of the number read
# is equal to the 2nd least significant byte
# and it prints 0 otherwise

main:
    li   $v0, 5
    syscall
    move $s0, $v0                   # input

    and  $t0, $s0, 255              # lowestByte = input & 0xFF

    and  $t1, $s0, 0xFF00           # secondLowestByte = (input & 0xFF00) >> 8
    srl  $t2, $t1, 8                #

    bne  $t0, $t2, if_match_else

if_match:
    li   $a0, 1
    b print

if_match_else:
    li   $a0, 0

print:
    li   $v0, 1
    syscall

    li   $a0, '\n'
    li   $v0, 11
    syscall

    li   $v0, 0
    jr   $ra

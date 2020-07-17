# read a line and print whether it is a palindrom

main:
    la   $a0, str0       # printf("Enter a line of input: ");
    li   $v0, 4
    syscall

    la   $a0, line
    la   $a1, 256
    li   $v0, 8          # fgets(buffer, 256, stdin)
    syscall              #

    li   $t0, 0          # i = 0
while1:
    lb   $t1, line($t0)
    beqz $t1, while1_end

    add  $t0, 1          # i++
    b    while1
while1_end:

    li   $t1, 0          # j = 0
    sub  $t2, $t0, 2     # k = i - 2
while2:
    bge  $t1, $t2, while2_end
    lb   $t3, line($t1)  # a = line[j]
    lb   $t4, line($t2)  # b = line[k]

    bne  $t3, $t4, if_not_palindrome

    add  $t1, 1          # j++
    sub  $t2, 1          # k--
    b while2
while2_end:

if_palindrome:
    la   $a0, palindrome
    li   $v0, 4
    syscall
    b    end

if_not_palindrome:
    la   $a0, not_palindrome
    li   $v0, 4
    syscall

end:
    li   $v0, 0          # return 0
    jr   $31


.data
str0:
    .asciiz "Enter a line of input: "
palindrome:
    .asciiz "palindrome\n"
not_palindrome:
    .asciiz "not palindrome\n"


# line of input stored here
line:
    .space 256


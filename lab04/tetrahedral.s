# Read a number n and print the first n tetrahedral numbers
# https://en.wikipedia.org/wiki/Tetrahedral_number

main:                           # int main(void) {

    la $a0, prompt              # printf("Enter how many: ");
    li $v0, 4
    syscall

    li $v0, 5                   # scanf("%d", number);
    syscall
    move $s0 $v0

    li $t0, 1                   # n = 1

loop1:
    bgt $t0, $s0, end           # while (j <= n)
    li $t7, 0                   # total = 0
    li $t1, 1                   # j = 1
    b loop2

loop1_end:
    move $a0, $t7               # a0 = total
    li $v0, 1                   # printf("%d", a0)
    syscall

    li $a0, 0xa                 # use ascii code for special characters
    li $v0, 11                  # printf("\n")
    syscall

    addi $t0, $t0, 1            # n++
    b loop1

loop2:
    bgt $t1, $t0, loop1_end     # while (j <= n)
    li $t2, 1                   # i = 1
    b loop3

loop2_end:
    addi $t1, $t1, 1            # j++
    b loop2

loop3:
    bgt $t2, $t1, loop2_end     # while (i <= j)
    add $t7, $t7 $t2            # total += i
    add $t2, $t2, 1             # i++
    b loop3

end:
    jr   $ra           # return

    .data
prompt:
    .asciiz "Enter how many: "

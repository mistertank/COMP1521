# read 10 numbers into an array
# bubblesort them
# then print the 10 numbers

# i in register $t0
# registers $t1, $t2 & $t3 used to hold temporary results
# swapped in register $t7

main:

    li $t0, 0                   # i = 0
in_loop:
    bge $t0, 10, in_end         # while (i < 10) {

    li $v0, 5                   #   scanf("%d", &numbers[i]);
    syscall                     #

    mul $t1, $t0, 4             #   calculate &numbers[i]
    la $t2, numbers             #
    add $t3, $t1, $t2           #
    sw $v0, ($t3)               #   store entered number in array

    add $t0, $t0, 1             #   i++;
    b in_loop                   # }
in_end:

    li $t7, 1                   # swapped = 1
outer_loop:
    beqz $t7, outer_end         # while (swapped)

    li $t7, 0                   # swapped = 0
    li $t0, 1                   # i = 1

inner_loop:
    bge $t0, 10, inner_end

    mul $t1, $t0, 4             # calculate &numbers[i]
    la $t2, numbers
    add $t3, $t1, $t2
    lw $t4, ($t3)               # x = numbers[i]

    addi $t5, $t3, -4           # calculate &numbers[i - 1]
    lw $t6, ($t5)               # y = numbers[i - 1]

    bge $t4, $t6, skip_swap     # if (x >= y) don't swap

    sw $t4, ($t5)               # numbers[i - 1] = x
    sw $t6, ($t3)               # numbers[i] = y
    li $t7, 1                   # swapped = 1

skip_swap:
    add $t0, $t0, 1             # i++
    b inner_loop

inner_end:

    b outer_loop

outer_end:


    li $t0, 0                   # i = 0
write_loop:
    bge $t0, 10, write_end      # while (i < 10) {

    mul $t1, $t0, 4             #   calculate &numbers[i]
    la $t2, numbers             #
    add $t3, $t1, $t2           #
    lw $a0, ($t3)               #   load numbers[i] into $a0
    li $v0, 1                   #   printf("%d", numbers[i])
    syscall

    li   $a0, '\n'              #   printf("%c", '\n');
    li   $v0, 11
    syscall

    add $t0, $t0, 1             #   i++
    b write_loop                # }
write_end:

    jr $ra                      # return

.data

numbers:
    .word 0 0 0 0 0 0 0 0 0 0  # int numbers[10] = {0};


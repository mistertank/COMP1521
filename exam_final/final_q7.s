# this code reads 1 integer and prints it
# change it to read integers until their sum is >= 42
# and then print theintgers read in reverse order

main:
    li   $s0, 0         # sum = 0;
    li   $t0, 0         # i = 0;
in_loop:
    bge  $s0, 42, in_loop_end

    li   $v0, 5        #   scanf("%d", &x);
    syscall            #

    mul  $t1, $t0, 4   # calculate &numbers[i]
    sw   $v0, numbers($t1)

    addi $t0, $t0, 1
    add  $s0, $s0, $v0

    b in_loop
in_loop_end:

out_loop:
    ble  $t0, 0, out_loop_end

    sub  $t0, $t0, 1

    mul  $t1, $t0, 4
    lw   $a0, numbers($t1)
    li   $v0, 1
    syscall

    li   $a0, '\n'     #   printf("%c", '\n');
    li   $v0, 11
    syscall


    b out_loop
out_loop_end:
    li   $v0, 0        # return 0
    jr   $ra

    .data
numbers:
    .space 4000         # 1000 32bit ints

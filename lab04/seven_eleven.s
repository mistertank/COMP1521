# Read a number and print positive multiples of 7 or 11 < n

main:                           # int main(void)
    la  $a0, prompt             # printf("Enter a number: ");
    li  $v0, 4
    syscall

    li  $v0, 5                  # scanf("%d", number);
    syscall
    move $s0, $v0

    li  $t0, 1
    b loop_start

loop_start:
    bgt $t0, $s0, end   # while (t0 <= s0)

    rem $t1, $t0, 7
    beqz $t1 print_num
    rem $t1, $t0, 11
    beqz $t1 print_num

loop_end:
    addi $t0, $t0, 1

    b loop_start

end:
    jr  $ra             # return

print_num:
    move $a0, $t0       # print(t0)
    li  $v0, 1
    syscall

    li $a0, 0xa         # use ascii code for special characters
    # li   $a0, '\n'
    li   $v0, 11
    syscall

    b loop_end

    .data
prompt:
    .asciiz "Enter a number: "

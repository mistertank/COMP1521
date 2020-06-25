# read a number n and print the integers 1..n one per line

main:                           # int main(void)
    la  $a0, prompt             # printf("Enter a number: ");
    li  $v0, 4
    syscall

    li  $v0, 5                  # scanf("%d", number);
    syscall

    li  $a0, 42                 # printf("%d", 42);
    li  $v0, 1
    syscall

    li   $a0, '\n'      # printf("%c", '\n');
    li   $v0, 11
    syscall

end:
    jr  $ra                     # return

    .data
prompt:
    .asciiz "Enter a number: "

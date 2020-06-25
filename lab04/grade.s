# read a mark and print the corresponding UNSW grade

main:
    la $a0, prompt      # printf("Enter a mark: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", mark);
    syscall

    la $a0, fl          # printf("FL\n");
    li $v0, 4
    syscall


    jr $ra              # return

    .data
prompt:
    .asciiz "Enter a mark: "
fl:
    .asciiz "FL\n"
ps:
    .asciiz "PS\n"
cr:
    .asciiz "CR\n"
dn:
    .asciiz "DN\n"
hd:
    .asciiz "HD\n"

# read a mark and print the corresponding UNSW grade

main:
    la $a0, prompt      # printf("Enter a mark: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", mark);
    syscall
    move $t0, $v0

    blt $t0, 50, handle_fl

    blt $t0, 65, handle_ps

    blt $t0, 75, handle_cr

    blt $t0, 85, handle_dn

    b handle_hd
    b end


handle_fl:
    la $a0, fl
    li $v0, 4
    syscall

    b end

handle_ps:
    la $a0, ps
    li $v0, 4
    syscall

    b end

handle_cr:
    la $a0, cr
    li $v0, 4
    syscall

    b end

handle_dn:
    la $a0, dn
    li $v0, 4
    syscall

    b end

handle_hd:
    la $a0, hd
    li $v0, 4
    syscall

    b end

end:
    li $v0, 0
    jr $ra

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

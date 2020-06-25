# read a mark and print the corresponding UNSW grade

main:
    la $a0, prompt      # printf("Enter a mark: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", mark);
    syscall
    move $t0, $v0

    li $t1, 50
    blt $t0, $t1, handle_fl

    li $t1, 65
    blt $t0, $t1, handle_ps

    li $t1, 75
    blt $t0, $t1, handle_cr

    li $t1, 85
    blt $t0, $t1, handle_dn

    b handle_hd
    b end


handle_fl:
    la $a0, fl          # printf("FL\n");
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

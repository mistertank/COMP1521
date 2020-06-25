main:
    la   $a0, pun
    li   $v0, 4
    syscall
    jr   $ra

    .data
pun:
    .asciiz "I love MIPS\n"

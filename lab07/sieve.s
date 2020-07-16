# Sieve of Eratosthenes
# https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
main:
    li      $s0, 0
    li      $s1, 1

    li      $t0, 0                          # i = 0
main_setup_loop:
    bge     $t0, 1000, main_setup_end       # while (i < 1000)

    sb      $s1, prime($t0)

    addi    $t0, $t0, 1                     # i++
    b       main_setup_loop
main_setup_end:

    li      $t0, 2                          # i = 2
main_run_loop:
    bge     $t0, 1000, main_run_end         # while (i < 1000)

    lb      $t2, prime($t0)                 # if (prime[i])
    bne     $t2, 1, main_run_not_prime

main_run_prime:
    move    $a0, $t0                        # printf("%d\n", i)
    li      $v0, 1
    syscall
    li      $a0, '\n'
    li      $v0, 11
    syscall

    mul     $t1, $t0, 2                     # j = 2 * i
main_run_prime_loop:
    bge     $t1, 1000, main_run_prime_end   # while (j < 1000)

    sb      $s0, prime($t1)

    add     $t1, $t1, $t0                   # j = j + i
    b       main_run_prime_loop
main_run_prime_end:

main_run_not_prime:

    addi    $t0, $t0, 1                     # i++
    b       main_run_loop
main_run_end:

    li $v0, 0           # return 0
    jr $31

.data
prime:
    .space 1000

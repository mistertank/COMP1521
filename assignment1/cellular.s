########################################################################
# COMP1521 20T2 --- assignment 1: a cellular automaton renderer
#
# Written by z5258068, July 2020.


# Maximum and minimum values for the 3 parameters.

MIN_WORLD_SIZE  =    1
MAX_WORLD_SIZE  =  128
MIN_GENERATIONS = -256
MAX_GENERATIONS =  256
MIN_RULE        =    0
MAX_RULE        =  255

# Characters used to print alive/dead cells.

ALIVE_CHAR      = '#'
DEAD_CHAR       = '.'

# Maximum number of bytes needs to store all generations of cells.

MAX_CELLS_BYTES = (MAX_GENERATIONS + 1) * MAX_WORLD_SIZE

    .data

# `cells' is used to store successive generations.  Each byte will be 1
# if the cell is alive in that generation, and 0 otherwise.

cells:                  .space MAX_CELLS_BYTES


# Some strings you'll need to use:

prompt_world_size:      .asciiz "Enter world size: "
error_world_size:       .asciiz "Invalid world size\n"
prompt_rule:            .asciiz "Enter rule: "
error_rule:             .asciiz "Invalid rule\n"
prompt_n_generations:   .asciiz "Enter how many generations: "
error_n_generations:    .asciiz "Invalid number of generations\n"

    .text

###########################################################################
# main
#
# local variables:
# $s0   world_size
# $s1   rule
# $s2   n_generations
# $s3   reverse
# $s4   g

main:
    sub     $sp, $sp, 4                     # save $ra on the stack
    sw      $ra, ($sp)

    #######################
    # Get params from stdin

    # get world size
    la      $a0, prompt_world_size          # printf("Enter world size: ");
    li      $v0, 4
    syscall

    li      $v0, 5                          # scanf("%d", world_size);
    syscall
    move    $s0, $v0

    # if (rule < MIN_WORLD_SIZE) goto main_err_world_size;
    blt     $s0, MIN_WORLD_SIZE, main_err_world_size
    # if (rule > MAX_WORLD_SIZE) goto main_err_world_size;
    bgt     $s0, MAX_WORLD_SIZE, main_err_world_size

    # get rule
    la      $a0, prompt_rule                # printf("Enter rule: ");
    li      $v0, 4
    syscall

    li      $v0, 5                          # scanf("%d", rule);
    syscall
    move    $s1, $v0

    # if (rule < MIN_RULE) goto main_err_rule;
    blt     $s1, MIN_RULE, main_err_rule
    # if (rule > MAX_RULE) goto main_err_rule;
    bgt     $s1, MAX_RULE, main_err_rule

    # get number of generations
    la      $a0, prompt_n_generations       # printf("Enter how many generations: ");
    li      $v0, 4
    syscall

    li      $v0, 5                          # scanf("%d", n_generations);
    syscall
    move    $s2, $v0

    # if (n_generations < MIN_GENERATIONS) goto main_err_generations;
    blt     $s2, MIN_GENERATIONS, main_err_generations
    # if (n_generations > MAX_GENERATIONS) goto main_err_generations;
    bgt     $s2, MAX_GENERATIONS, main_err_generations

    li      $a0, '\n'                       # putchar('\n');
    li      $v0, 11
    syscall

    # Handle negative generations
    li      $s3, 0                          # int reverse = 0;
main_if_reverse_cond:
    bgez    $s2, main_if_reverse_false      # if (n_generations < 0)

    li      $s3, 1                          # reverse = 1:
    mul     $s2, $s2, -1                    # n_generations = -n_generations:

main_if_reverse_false:

    # Set middle cell in first generation to alive
    div     $t0, $s0, 2                     # t0 = world_size / 2;
    li      $t1, 1                          # t1 = 1;
    sb      $t1, cells($t0)                 # cells[0][t0] = t1;

    #################
    # Run generations

    li      $s4, 1                          # g = 1
main_run_loop:
    bgt     $s4, $s2, main_run_end          # while (g <= n_generations) {

    move    $a0, $s0                        # run_generation(world_size,
    move    $a1, $s4                        #                g,
    move    $a2, $s1                        #                rule)
    jal     run_generation

    addi    $s4, $s4, 1                     # g++
    b       main_run_loop                   # }

main_run_end:

    ###################
    # Print generations

    bne     $s3, 1, main_print_normal       # if (!reverse) goto main_print_normal
main_print_reverse:
    move    $s4, $s2                        # g = n_generations;
main_print_reverse_loop:
    blt     $s4, 0, main_print_reverse_end  # while (g >= 0) {

    move    $a0, $s0                        #   print_generation(world_size, g);
    move    $a1, $s4
    jal     print_generation

    sub     $s4, $s4, 1                     #   g--;
    b       main_print_reverse_loop
main_print_reverse_end:
    b       main_print_end                  # }

main_print_normal:
    li      $s4, 0                          # g = 0;
main_print_normal_loop:
    bgt     $s4, $s2, main_print_normal_end # while (g <= n_generations) {

    move    $a0, $s0                        #   print_generation(world_size, g);
    move    $a1, $s4
    jal     print_generation

    add     $s4, $s4, 1                     #   g++;
    b       main_print_normal_loop
main_print_normal_end:
    b       main_print_end                  # }
main_print_end:

main_exit_normal:
    lw      $ra, ($sp)                      # restore $ra from the stack
    add     $sp, $sp, 4
    li      $v0, 0
    jr      $ra

    ###################
    # Error exit labels

main_err_world_size:
    la      $a0, error_world_size
    li      $v0, 4
    syscall
    b       main_exit_err

main_err_rule:
    la      $a0, error_rule
    li      $v0, 4
    syscall
    b       main_exit_err

main_err_generations:
    la      $a0, error_n_generations
    li      $v0, 4
    syscall
    b       main_exit_err

main_exit_err:
    li      $v0, 1
    jr      $ra


###########################################################################
# run_generation
#
# Given `world_size', `which_generation', and `rule', calculate
# a new generation according to `rule' and store it in `cells'.
#
# arguments:
# $a0   world_size
# $a1   which_generation
# $a2   rule
#
# local variables:
# $s0   left
# $s1   center
# $s2   right
# $s3   state
# $s4   bit
# $s5   set
# $t0   x
# $t1   indexing the cells array

run_generation:
run_generation_init:

    # Store registers on the stack
    sub     $sp, $sp, 6
    sb      $s5, 5($sp)
    sb      $s4, 4($sp)
    sb      $s3, 3($sp)
    sb      $s2, 2($sp)
    sb      $s1, 1($sp)
    sb      $s0, 0($sp)

    li      $t0, 0                          # x = 0;
run_generation_loop:
    bge     $t0, $a0, run_generation_end    # while (x < world_size)

    # Calculate index of left cell
    sub     $t1, $a1, 1                     # calculate (which_generation - 1)
    mul     $t1, $t1, $a0                   # calculate cells[which_generation - 1]
    add     $t1, $t1, $t0                   # calculate &cells[which_generation - 1][x]
    sub     $t1, $t1, 1                     # calculate &cells[which_generation - 1][x - 1]

    li      $s0, 0                          # int left = 0;
    ble     $t0, 0, run_generation_skip_left# if (x <= 0) goto run_generation_skip_left;
    lb      $s0, cells($t1)                 # left = cells[which_generation - 1][x - 1];
run_generation_skip_left:

    # Calculate index of centre cell
    addi    $t1, $t1, 1                     # calculate &cells[which_generation - 1][x]
    lb      $s1, cells($t1)                 # int centre = cells[which_generation - 1][x];

    # Calculate index of right cell
    addi    $t1, $t1, 1                     # calculate &cells[which_generation - 1][x + 1]

    li      $s2, 0                          # int right = 0;
    # if (x >= world_size - ) goto run_generation_skip_right
    addi    $t3, $a0, -1
    bge     $t0, $t3, run_generation_skip_right

    lb      $s2, cells($t1)                 # right = cells[which_generation - 1][x + 1];
run_generation_skip_right:

    # Convert states into one value
    sll     $s3, $s0, 2                     # state = left << 2;
    sll     $t2, $s1, 1                     # t2 = center << 1;
    or      $s3, $s3, $t2                   # state |= t2;
    sll     $t2, $s2, 0                     # t2 = right << 0;
    or      $s3, $s3, $t2                   # state |= t2;

    # Get corresponding bit
    li      $t3, 1
    sll     $s4, $t3, $s3                   # bit = 1 << state;

    # Check if bit is set in the rule
    and     $s5, $a2, $s4                   # set = rule & bit;

    # Get index of new cell
    mul     $t1, $a1, $a0                   # calculate cells[which_generation]
    add     $t1, $t1, $t0                   # calculate &cells[which_generation][x]

    # move    $t7, $a0
    # move    $a0, $a1
    # li      $v0, 1
    # syscall
    # li      $a0, '-'
    # li      $v0, 11
    # syscall
    # move    $a0, $t0
    # li      $v0, 1
    # syscall
    # li      $a0, ' '
    # li      $v0, 11
    # syscall
    # move    $a0, $s0
    # li      $v0, 1
    # syscall
    # li      $a0, ' '
    # li      $v0, 11
    # syscall
    # move    $a0, $s1
    # li      $v0, 1
    # syscall
    # li      $a0, ' '
    # li      $v0, 11
    # syscall
    # move    $a0, $s2
    # li      $v0, 1
    # syscall
    # li      $a0, ' '
    # li      $v0, 11
    # syscall
    # move    $a0, $s3
    # li      $v0, 1
    # syscall
    # li      $a0, ' '
    # li      $v0, 11
    # syscall
    # move    $a0, $s4
    # li      $v0, 1
    # syscall
    # li      $a0, ' '
    # li      $v0, 11
    # syscall
    # move    $a0, $s5
    # li      $v0, 1
    # syscall
    # li      $a0, '\n'
    # li      $v0, 11
    # syscall
    # move    $a0, $t7

    bnez    $s5, run_generation_set_alive   # if (set != 0) goto run_generation_set_alive;

run_generation_set_dead:
    li      $t4, 0
    sb      $t4, cells($t1)                 # cells[which_generation][x] = 0;

    b       run_generation_step

run_generation_set_alive:
    li      $t4, 1
    sb      $t4, cells($t1)                 # cells[which_generation][x] = 1;

run_generation_step:
    add     $t0, $t0, 1                     # x++;
    b       run_generation_loop

run_generation_end:
run_generation_post:

    # move    $t7, $a0
    # li      $a0, '\n'
    # li      $v0, 11
    # syscall
    # move    $a0, $t7

    # Restore registers from the stack
    lb      $s0, 0($sp)
    lb      $s1, 1($sp)
    lb      $s2, 2($sp)
    lb      $s3, 3($sp)
    lb      $s4, 4($sp)
    lb      $s5, 5($sp)
    add     $sp, $sp, 6

    # Return to caller
    jr  $ra


###########################################################################
# print_generation
#
# Given `world_size', and `which_generation', print out the
# specified generation.
#
# arguments:
# $a0   world_size
# $a1   which_generation
#
# local variables:
# $s0   world_size
# $s1   which_generation
# $t0   x

print_generation:
print_generation_init:
    # Store registers on the stack
    sub     $sp, $sp, 8
    sw      $s1, 4($sp)
    sw      $s0, 0($sp)

    move    $s0, $a0                        # Copy arguments so I can use syscalls
    move    $s1, $a1

    # Show generation number
    move    $a0, $s1                        # printf("%d", which_generation);
    li      $v0, 1
    syscall

    li      $a0, '\t'                       # putchar('\t')
    li      $v0, 11
    syscall

    li      $t0, 0                          # x = 0;
print_generation_loop:
    bge     $t0, $s0, print_generation_end  # while (x < world_size)

    # Calculate index
    mul     $t1, $s1, $s0                   # calculate cells[which_generation]
    add     $t1, $t1, $t0                   # calculate &cells[which_generation][x]
    lb      $t2, cells($t1)                 # t2 = cells[which_generation][x]

    beqz    $t2, print_generation_dead      # if (cells[which_generation][x])

print_generation_alive:
    li      $a0, ALIVE_CHAR                 # putchar(ALIVE_CHAR)
    li      $v0, 11
    syscall
    b print_generation_step

print_generation_dead:
    li      $a0, DEAD_CHAR                  # putchar(DEAD_CHAR)
    li      $v0, 11
    syscall

print_generation_step:
    add     $t0, $t0, 1                     # x++;
    b       print_generation_loop

print_generation_end:
    li      $a0, '\n'                       # putchar('\n')
    li      $v0, 11
    syscall

print_generation_post:
    # Restore registers from the stack
    lw      $s0, 0($sp)
    lw      $s1, 4($sp)
    addi    $sp, $sp, 8

    # Return to caller
    jr  $ra

###############################################################################
# END OF PROGRAM
###############################################################################

###############################################################################
# possibly useful stuff

    # # Crappy debug print
    # li      $a0, 2
    # li      $v0, 1
    # syscall
    # li      $a0, '\n'
    # li      $v0, 11
    # syscall

    # move    $a0, $s0
    # li      $v0, 1
    # syscall
    # li      $a0, '\n'
    # li      $v0, 11
    # syscall

    # move    $a0, $s1
    # li      $v0, 1
    # syscall
    # li      $a0, '\n'
    # li      $v0, 11
    # syscall

    # move    $a0, $s2
    # li      $v0, 1
    # syscall
    # li      $a0, '\n'
    # li      $v0, 11
    # syscall

    # move    $a0, $s3
    # li      $v0, 1
    # syscall
    # li      $a0, '\n'
    # li      $v0, 11
    # syscall

# # print_cell
# # arguments:
# # $a0   is_alive
# print_cell:
#     bne     $a0, 1, print_cell_dead
# print_cell_alive:
#     li      $a0, ALIVE_CHAR
#     li      $v0, 11
#     syscall
#     b print_cell_end
# print_cell_dead:
#     li      $a0, DEAD_CHAR
#     li      $v0, 11
#     syscall
# print_cell_end:
#     jr $ra


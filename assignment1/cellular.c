////////////////////////////////////////////////////////////////////////
// COMP1521 20T2 --- assignment 1: a cellular automaton renderer
//
// This code runs a one-dimensional, three-neighbour cellular automaton.
// It examines its neighbours and its value in the previous generation
// to derive the value for the next generation.
//
//     . . . . # . . . .
//     . . . # # # . . .
//     . . # # . . # . .
//     . # # . # # # # .
//     # # . . # . . . #
//
// Here, we using '#' to indicate a cell that's alive; and '.' to
// indicate a cell that is not.
//
// Given we examine three neighbours, there are eight states that the
// prior cells could be in.  They are:
//
//     . . .   . . #   . # .   . # #   # . .   # . #   # # .   # # #
//       0       1       2       3       4       5       6       7
//
// For each one, we decide what action to take.  For example, we might
// choose to have the following `rule':
//
//     . . .   . . #   . # .   . # #   # . .   # . #   # # .   # # #
//       .       #       #       #       #       .       .       .
//
// We apply this rule to every cell, to determine whether the next state
// is alive or dead; and this forms the next generation.  If we print
// these generations, one after the other, we can get some interesting
// patterns.
//
// The description of the rule above --- by example, showing each case
// and how it should be handled --- is inefficient.  We can abbreviate
// this rule by reading it in binary, considering live cells as 1's and
// dead cells as 0s; and if we consider the prior states to be a binary
// value too --- the above rule could be 0b00001110, or 30.
//
// To use that rule, we would mix together the previous states we're
// interested in --- left, middle, and right --- which tells us which
// bit of the rule value gives our next state.
//
// The world size, rule and the number of generations, are read from stdin:
//
// $ ./cellular
// Enter world size: 60
// Enter rule: 30
// Enter how many generations: 10
//
//       0     ..............................#.............................
//       1     .............................###............................
//       2     ............................##..#...........................
//       3     ...........................##.####..........................
//       4     ..........................##..#...#.........................
//       5     .........................##.####.###........................
//       6     ........................##..#....#..#.......................
//       7     .......................##.####..######......................
//       8     ......................##..#...###.....#.....................
//       9     .....................##.####.##..#...###....................
//       10    ....................##..#....#.####.##..#...................
//
// $ ./cellular
// Enter world size: 60
// Enter rule: 30
// Enter how many generations: -10
//
// 10   ....................##..#....#.####.##..#...................
// 9    .....................##.####.##..#...###....................
// 8    ......................##..#...###.....#.....................
// 7    .......................##.####..######......................
// 6    ........................##..#....#..#.......................
// 5    .........................##.####.###........................
// 4    ..........................##..#...#.........................
// 3    ...........................##.####..........................
// 2    ............................##..#...........................
// 1    .............................###............................
// 0    ..............................#.............................

#include <stdio.h>
#include <stdint.h>

// maximum and minimum values for the 3 parameters

#define MIN_WORLD_SIZE     1
#define MAX_WORLD_SIZE   128
#define MIN_GENERATIONS -256
#define MAX_GENERATIONS  256
#define MIN_RULE           0
#define MAX_RULE         255

// characters used to print alive/dead cells

#define ALIVE_CHAR        '#'
#define DEAD_CHAR         '.'

// `cells' is used to store successive generations.  Each byte will be 1
// if the cell is alive in that generation, and 0 otherwise.

static int8_t cells[MAX_GENERATIONS + 1][MAX_WORLD_SIZE];

static void run_generation(int world_size, int which_generation, int rule);
static void print_generation(int world_size, int which_generation);

int main(int argc, char *argv[]) {

    // read 3 integer parameters from stdin

    printf("Enter world size: ");
    int world_size = 0;
    scanf("%d", &world_size);
    if (world_size < MIN_WORLD_SIZE || world_size > MAX_WORLD_SIZE) {
        printf("Invalid world size\n");
        return 1;
    }

    printf("Enter rule: ");
    int rule = 0;
    scanf("%d", &rule);
    if (rule < MIN_RULE || rule > MAX_RULE) {
        printf("Invalid rule\n");
        return 1;
    }

    printf("Enter how many generations: ");
    int n_generations = 0;
    scanf("%d", &n_generations);
    if (n_generations < MIN_GENERATIONS || n_generations > MAX_GENERATIONS) {
        printf("Invalid number of generations\n");
        return 1;
    }

    putchar('\n');

    // negative generations means show the generations in reverse
    int reverse = 0;
    if (n_generations < 0) {
        reverse = 1;
        n_generations = -n_generations;
    }

    // the first generation always has a only single cell which is alive
    // this cell is in the middle of the world
    cells[0][world_size / 2] = 1;

    for (int g = 1; g <= n_generations; g++) {
        run_generation(world_size, g, rule);
    }

    if (reverse) {
        for (int g = n_generations; g >= 0; g--) {
            print_generation(world_size, g);
        }
    } else {
        for (int g = 0; g <= n_generations; g++) {
            print_generation(world_size, g);
        }
    }

    return 0;
}

//
// Calculate a new generation using rule and store it in cells
//
static void run_generation(int world_size, int which_generation, int rule) {
    for (int x = 0; x < world_size; x++) {

        // Get the values in the left and right neighbour cells.
        // This requires some care, otherwise we could read beyond the
        // bounds of the array.  In the cases we are at the limits of
        // the function, we consider those out-of-bounds cells zero.

        int left = 0;
        if (x > 0) {
            left = cells[which_generation - 1][x - 1];
        }

        int centre = cells[which_generation - 1][x];

        int right = 0;
        if (x < world_size - 1) {
            right = cells[which_generation - 1][x + 1];
        }

        // Convert the left, centre, and right states into one value.
        int state = left << 2 | centre << 1 | right << 0;

        // And check whether that bit is set or not in the rule.
        // by testing the corresponding bit of the rule number.
        int bit = 1 << state;
        int set = rule & bit;

        if (set) {
            cells[which_generation][x] = 1;
        } else {
            cells[which_generation][x] = 0;
        }
    }
}

//
// Print out the specified generation
//
static void print_generation(int world_size, int which_generation) {
    printf("%d", which_generation);
    putchar('\t');

    for (int x = 0; x < world_size; x++) {
        if (cells[which_generation][x]) {
            putchar(ALIVE_CHAR);
        } else {
            putchar(DEAD_CHAR);
        }
    }

    putchar('\n');
}

#include "inttypes.h"
#include "ncurses.h"
#include "universe.h"
#include <unistd.h>
#define ROW   0
#define DELAY 50000
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define OPTIONS "tsn:i:o:h"

// received instruction from CSE 13S tutors and TA's on file setups, ncurses implementation, error handling, and swapping universes

void h_option(void);
void game_of_life(Universe *universe1, Universe *universe2);
void game_of_life_print(Universe *universe1);

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Conway's Game of Life\n"
        "\n"
        "USAGE\n"
        "   %s tsn:i:o:h\n"
        "\n"
        "OPTIONS\n"
        "   -t             Create your universe as a toroidal.\n"
        "   -s             Silent - do not use animate the evolution.\n"
        "   -n {number}    Number of generations [default: 100]\n"
        "   -i {file}      Input file [default: stdin]\n"
        "   -o {file}      Output file [default: stdout]\n",
        exec);
}

int main(int argc, char **argv) {
    bool is_toroidal = false;
    bool use_ncurses = true;
    uint32_t gen_num = 100;
    FILE *input_file = stdin;
    FILE *output_file = stdout;

    int opt = 0;
    
    if (argc < 2) {
        h_option();
    }

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        // toroidal
        case 't': is_toroidal = true; break;
        // silent
        case 's': use_ncurses = false; break;
        // number of generations
        case 'n': gen_num = (uint32_t) (strtoul(optarg, NULL, 10)); break;
        // input file
        case 'i':
            input_file = fopen(optarg, "r");
            if (input_file == NULL) {
                printf("Error opening %s. \n", optarg);
                return -1;
            };
            break;
        // output file
        case 'o': output_file = fopen(optarg, "w"); break;
        // help
        case 'h': h_option(); break;
        default:
            usage(argv[0]); 
            return EXIT_FAILURE;
        }
    }
    // initialize rows and cols
    uint32_t rows;
    uint32_t cols;
    // read file, set universe dimensions, and populate universes
    fscanf(input_file, "%u %u\n", &rows, &cols);
    Universe *universe1 = uv_create(rows, cols, is_toroidal);
    if (uv_populate(universe1, input_file) != false) {
        uv_populate(universe1, input_file);
    } else {
        printf("Malinformed input.\n");
        return 0;
    };
    Universe *universe2 = uv_create(rows, cols, is_toroidal);
    if (uv_populate(universe2, input_file) != false) {
        uv_populate(universe2, input_file);
    } else {
        printf("Malinformed input.\n");
        return 0;
    };
    fclose(input_file);
    // set up ncurses
    if (use_ncurses) {
        initscr();
        curs_set(FALSE);
    }
    // perform generations
    for (uint32_t i = 0; i < gen_num; i++) {
        game_of_life(universe1, universe2);
        if (use_ncurses) {
            clear();
            game_of_life_print(universe1);
            refresh();
            usleep(DELAY);
        }
    }
    endwin();
    // print to file
    uv_print(universe1, output_file);
    fclose(output_file);
    // Delete universes from memory
    uv_delete(universe1);
    uv_delete(universe2);
    return 0;
}

// game of life conditions
void game_of_life(Universe *universe1, Universe *universe2) {
    for (uint32_t row = 0; row < uv_rows(universe1); row++) {
        for (uint32_t col = 0; col < uv_cols(universe1); col++) {
            bool alive = uv_get_cell(universe1, row, col);
            uint32_t neighbors = uv_census(universe1, row, col);
            if ((alive && neighbors == 2) || (alive && neighbors == 3)) {
                uv_live_cell(universe2, row, col);
            } else if ((!(alive)) && neighbors == 3) {
                uv_live_cell(universe2, row, col);
            } else {
                uv_dead_cell(universe2, row, col);
            }
        }
    }
    // swap the universe
    for (uint32_t row = 0; row < uv_rows(universe2); row++) {
        for (uint32_t col = 0; col < uv_cols(universe2); col++) {
            bool alive = uv_get_cell(universe2, row, col);
            if (alive) {
                uv_live_cell(universe1, row, col);
            } else {
                uv_dead_cell(universe1, row, col);
            }
        }
    }
}

// ncurses display of universe
void game_of_life_print(Universe *universe1){
    for (uint32_t row = 0; row < uv_rows(universe1); row++) {
        for (uint32_t col = 0; col < uv_cols(universe1); col++) {
            bool alive = uv_get_cell(universe1, row, col);
            if (alive) {
                mvprintw(row, col, "o");
            }
        }
    }
}

void h_option(void) {
    printf("SYNOPSIS\n"
           "   Conway's Game of Life\n"
           "\n"
           "USAGE\n"
           "   tsn:i:o:h\n"
           "\n"
           "OPTIONS\n"
           "   -t             Create your universe as a toroidal.\n"
           "   -s             Silent - do not use animate the evolution.\n"
           "   -n {number}    Number of generations [default: 100]\n"
           "   -i {file}      Input file [default: stdin]\n"
           "   -n {file}      Output file [default: stdout]\n");
}

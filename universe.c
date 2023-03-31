#include "universe.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// received instruction from CSE 13S tutors and TA's on memory allocation concepts and general approaches to implementing ADT and helper functions and file manipulation

// Universe data type
typedef struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
} Universe;

// returns pointers to Universe
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    // universe object in Type Universe
    Universe *universe = (Universe *) calloc(1, sizeof(Universe));
    // rows
    universe->rows = rows;
    // columns
    universe->cols = cols;
    // toroidal
    universe->toroidal = toroidal;
    // create grid based on Universe data type allocate column of pointers to rows
    universe->grid = (bool **) calloc(rows, sizeof(bool *));
    // allocate row for each column
    for (uint32_t r = 0; r < rows; r += 1) {
        universe->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    return universe;
}

// clear memory of grid space 
void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; ++i) {
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

// return number of rows in Universe
uint32_t uv_rows(Universe *u) {
    return u->rows;
}

// return number of columns in Universe
uint32_t uv_cols(Universe *u) {
    return u->cols;
}

// mark cell as live as long as it's within range
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= u->rows || c >= u->cols) {
        u->grid[r][c] = false;
    } else {
        u->grid[r][c] = true;
    }
}

// mark cell as dead (if it's out of range and if not)
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= u->rows || c >= u->cols) {
        u->grid[r][c] = false;
    } else {
        u->grid[r][c] = false;
    }
}

// return cell value given specified row and column
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= u->rows || c >= u->cols) {
        u->grid[r][c] = false;
    }
    return u->grid[r][c];
}

// read row-column from file and populate locations of cell with live cell
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t rows;
    uint32_t cols;
    while (fscanf(infile, "%u %u\n", &rows, &cols) != EOF) {
        if ((rows > u->rows) || (cols > u->cols)) {
            return false;
        }
        uv_live_cell(u, rows, cols);
    }
    return true;
}

// return number of live neighbors adjacent to cell
// code provided by tutor
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t live_neighbor_count = 0;
    for (int32_t i = -1; i < 2; i++) {
        for (int32_t j = -1; j < 2; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            uint32_t row = r + i;
            uint32_t col = c + j;
            if (u->toroidal) {
                row += u->rows;
                row %= u->rows;
                col += u->cols;
                col %= u->cols;
            } else if ((row >= u->rows) || (col >= u->cols)) {
                continue;
            }
            live_neighbor_count += uv_get_cell(u, row, col);
        }
    }
    return live_neighbor_count;
}

// prints universe to file, where 'o' is live and '.' is dead
void uv_print(Universe *u, FILE *outfile) {
    // initialize file
    for (uint32_t i = 0; i < u->rows; i++) {
        for (uint32_t j = 0; j < u->cols; j++) {
            if (uv_get_cell(u, i, j) == true) {
                fputc('o', outfile); // write 'o' to file
            }
            if (uv_get_cell(u, i, j) == false) {
                fputc('.', outfile); // write '.' to file
            }
        }
        fputc('\n', outfile);
    }
}

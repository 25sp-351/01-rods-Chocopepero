#ifndef CUTTING_H
#define CUTTING_H
#include <stdio.h>
#include <stdlib.h>

#include "piece_values.h"
#include "vec.h"

#define BUFFER_SIZE 100
#define BASE_10 10
#define MAX(LEFT, RIGHT) (((LEFT) > (RIGHT)) ? (LEFT) : (RIGHT))
#define UNSEEN_ELEMENT -1

typedef struct {
    int length;
    int value;
} piece_t;

piece_t* piece_at(Vec vec, size_t index);

int parse_arguments(int argc, char* argv[], int* supplied_length);
Vec initialize_dp_table(int supplied_length);
Vec initialize_cuts(int supplied_length);
void compute_max_value(Vec piece_value, Vec dp_table, Vec cuts,
                       int supplied_length);
void track_piece_usage(Vec piece_value, Vec cuts, Vec piece_use_count,
                       int supplied_length);
void print_results(Vec piece_value, Vec piece_use_count, int supplied_length,
                   Vec dp_table);
void clean_up(Vec piece_value, Vec dp_table, Vec cuts, Vec piece_use_count);
#endif

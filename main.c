#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutting.h"



int main(int argc, char* argv[]) {
    int supplied_length;
    if (parse_arguments(argc, argv, &supplied_length) != 0)
        return 1;

    Vec piece_value = read_piece_values();
    if (piece_value == NULL) {
        fprintf(stderr, "Error reading piece values.\n");
        return 1;
    }

    Vec dp_table = initialize_dp_table(supplied_length);
    Vec cuts     = initialize_cuts(supplied_length);

    compute_max_value(piece_value, dp_table, cuts, supplied_length);

    Vec piece_use_count = new_vec(sizeof(int));
    for (int pu_index = 0; pu_index < vec_length(piece_value); pu_index++) {
        int zero = 0;
        vec_add(piece_use_count, &zero);
    }

    track_piece_usage(piece_value, cuts, piece_use_count, supplied_length);
    print_results(piece_value, piece_use_count, supplied_length, dp_table);

    clean_up(piece_value, dp_table, cuts, piece_use_count);

    return 0;
}

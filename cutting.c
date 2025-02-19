#include "cutting.h"
#include "piece_values.h"


piece_t* piece_at(Vec vec, size_t index) {
    return (piece_t*)vec_at(vec, index);
}

int parse_arguments(int argc, char* argv[], int* supplied_length) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <length>\n", argv[0]);
        return 1;
    }

    char* endptr;
    *supplied_length = strtol(argv[1], &endptr, BASE_10);
    if (*endptr != '\0' || *supplied_length <= 0) {
        fprintf(stderr, "Invalid rod length: %s\n", argv[1]);
        return 1;
    }

    return 0;
}

Vec initialize_dp_table(int supplied_length) {
    Vec dp_table = new_vec(sizeof(int));
    for (int dp_index = 0; dp_index < supplied_length + 1; dp_index++) {
        int zero = 0;
        vec_add(dp_table, &zero);
    }
    return dp_table;
}

Vec initialize_cuts(int supplied_length) {
    Vec cuts = new_vec(sizeof(int));
    for (int cut_index = 0; cut_index < supplied_length + 1; cut_index++) {
        int unseen = UNSEEN_ELEMENT;
        vec_add(cuts, &unseen);
    }
    return cuts;
}

void compute_max_value(Vec piece_value, Vec dp_table, Vec cuts,
                       int supplied_length) {
    size_t pv_size = vec_length(piece_value);

    for (int curr_length = 1; curr_length <= supplied_length; curr_length++) {
        for (int val_index = 0; val_index < pv_size; val_index++) {
            piece_t* p = piece_at(piece_value, val_index);
            if (p->length <= curr_length) {
                int* curr_max = (int*)vec_at(dp_table, curr_length);
                int temp      = *curr_max;
                int* potential_max =
                    (int*)vec_at(dp_table, curr_length - p->length);
                int new_potential_max = p->value + *potential_max;

                *curr_max             = MAX(*curr_max, new_potential_max);
                if (*curr_max != temp) {
                    int* cut_at_curlen = (int*)vec_at(cuts, curr_length);
                    *cut_at_curlen     = p->length;
                }
            }
        }
    }
}

void track_piece_usage(Vec piece_value, Vec cuts, Vec piece_use_count,
                       int supplied_length) {
    int supp_length_rem = supplied_length;
    int* rem_cuts       = (int*)vec_at(cuts, supplied_length);

    while (*rem_cuts != UNSEEN_ELEMENT) {
        int len_to_cut = *rem_cuts;
        for (int pv_index = 0; pv_index < vec_length(piece_value); pv_index++) {
            if (piece_at(piece_value, pv_index)->length == len_to_cut) {
                int* use_count = (int*)vec_at(piece_use_count, pv_index);
                (*use_count)++;
                break;
            }
        }
        supp_length_rem -= len_to_cut;
        rem_cuts = (int*)vec_at(cuts, supp_length_rem);
    }
}

void print_results(Vec piece_value, Vec piece_use_count, int supplied_length,
                   Vec dp_table) {
    for (int index = 0; index < vec_length(piece_value); index++) {
        piece_t* p     = piece_at(piece_value, index);
        int* use_count = (int*)vec_at(piece_use_count, index);
        if (*use_count > 0) {
            printf("%d @ %d = %d\n", *use_count, p->length,
                   *use_count * p->value);
        }
    }
    int remaining_length = supplied_length;
    for (int index = 0; index < vec_length(piece_value); index++) {
        piece_t* p     = piece_at(piece_value, index);
        int* use_count = (int*)vec_at(piece_use_count, index);
        remaining_length -= (*use_count) * p->length;
    }
    printf("Remainder: %d\n", remaining_length);
    printf("Value: %d\n", *(int*)vec_at(dp_table, supplied_length));
}

void clean_up(Vec piece_value, Vec dp_table, Vec cuts, Vec piece_use_count) {
    vec_free(piece_value);
    vec_free(dp_table);
    vec_free(cuts);
    vec_free(piece_use_count);
}

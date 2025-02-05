#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "piece_values.h"
#include "vec.h"

#define BUFFER_SIZE 100
#define BASE_10 10
#define MAX(left, right) (((left) > (right)) ? (left) : (right))
#define UNSEEN_ELEMENT -1

typedef struct {
    int length;
    int value;
    int use_count;
} piece;

inline piece* piece_at(Vec vec, size_t index) {
    return (piece*)vec_at(vec, index);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <length>\n", argv[0]);
        return 1;
    }

    char* endptr;
    int supplied_length = strtol(argv[1], &endptr, BASE_10);
    if (*endptr != '\0' || supplied_length <= 0) {
        fprintf(stderr, "Invalid rod length: %s\n", argv[1]);
        return 1;
    }

    Vec piece_value = read_piece_values();
    if (piece_value == NULL) {
        fprintf(stderr, "Error reading piece values.\n");
        return 1;
    }

    size_t pv_size = vec_length(piece_value);

    Vec dp_table = new_vec(sizeof(int));
    for (int dp_index = 0; dp_index < supplied_length + 1; dp_index++) {
        int zero = 0;
        vec_add(dp_table, &zero);
    }

    Vec cuts = new_vec(sizeof(int));
    for (int cut_index = 0; cut_index < supplied_length + 1; cut_index++) {
        int unseen = UNSEEN_ELEMENT;
        vec_add(cuts, &unseen);
    }

    for (int curr_length = 1; curr_length <= supplied_length; curr_length++) {
        for (int val_index = 0; val_index < pv_size; val_index++) {
            piece* p = piece_at(piece_value, val_index);
            if (p->length <= curr_length) {
                int* curr_max       = (int*)vec_at(dp_table, curr_length);
                int temp = *curr_max;
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

    Vec piece_use_count = new_vec(sizeof(int));
    for (int pu_index = 0; pu_index < pv_size; pu_index++) {
        int zero = 0;
        vec_add(piece_use_count, &zero);
    }

    int supp_length_rem = supplied_length;
    int* rem_cuts       = (int*)vec_at(cuts, supplied_length);

    while (*rem_cuts != UNSEEN_ELEMENT) {
        int len_to_cut = *rem_cuts;
        for (int pv_index = 0; pv_index < pv_size; pv_index++) {
            if (piece_at(piece_value, pv_index)->length == len_to_cut) {
                int* use_count = (int*)vec_at(piece_use_count, pv_index);
                (*use_count)++;
                break;
            }
        }
        supp_length_rem -= len_to_cut;
        rem_cuts = (int*)vec_at(cuts, supp_length_rem);
    }

    for (int index = 0; index < pv_size; index++) {
        piece* p       = piece_at(piece_value, index);
        int* use_count = (int*)vec_at(piece_use_count, index);
        if (*use_count > 0) {
            printf("%d @ %d = %d\n", *use_count, p->length,
                   *use_count * p->value);
        }
    }
    printf("Remainder: %d\n", supp_length_rem);
    printf("Value: %d\n", *(int*)vec_at(dp_table, supplied_length));

    vec_free(piece_value);
    vec_free(dp_table);
    vec_free(cuts);
    vec_free(piece_use_count);

    return 0;
}

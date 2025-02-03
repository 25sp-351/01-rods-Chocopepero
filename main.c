#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "piece_values.h"
#include "vec.h"

#define BASE_10 10
#define MAX(left, right) (((left) > (right)) ? (left) : (right))
#define VEC_AT(type, vec, index) ((type*)vec_at(vec, index))
#define UNSEEN_ELEMENT -1

// int compare_piecelen(const void* left, const void* right) {
//     piece* pieceL = (piece*)left;
//     piece* pieceR = (piece*)right;
//     return pieceL->length - pieceR->length;
// }

// Vec read_input() {
//     Vec piece_value = new_vec(sizeof(piece));
//     int length;
//     int value;

//     while (scanf("%d, %d", &length, &value) == 2) {
//         piece new_piece = {length, value, 0};
//         vec_add(piece_value, &new_piece);
//     }
//     return piece_value;
// }

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
    size_t pv_size  = vec_length(piece_value);

    // Vec dp_table = new_vec(supplied_length + 1);
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
    printf("Before DP loop\n");
    for (int curr_length = 1; curr_length <= supplied_length; curr_length++)
        for (int val_index = 0; val_index < pv_size; val_index++) {
            if (piece_value[val_index].length <= curr_length) {
                int* len_to_compare = VEC_AT(int, dp_table, curr_length);
                    VEC_AT(int, dp_table,
                           curr_length -
                               VEC_AT(PieceLengthValue, piece_value, val_index)
                                   ->length);
                int* curr_max = VEC_AT(int, dp_table, curr_length);
                int* potential_max =
                    VEC_AT(PieceLengthValue, piece_value, val_index)->value +
                    VEC_AT(int, dp_table,
                           (curr_length -
                            VEC_AT(PieceLengthValue, piece_value, val_index)
                                ->length));

                *curr_max = MAX(*curr_max, *potential_max);
                if (len_to_compare != VEC_AT(int, dp_table, curr_length)) {
                    int* cut_at_curlen = VEC_AT(int, cuts, curr_length);
                    *cut_at_curlen =
                        VEC_AT(PieceLengthValue, piece_value, val_index)
                            ->length;
                }
            }
        }

    Vec piece_use_count = new_vec(sizeof(int));
    int supp_length_rem = supplied_length;
    int* rem_cuts       = VEC_AT(int, cuts, supplied_length);

    // SEG FAULT HERE
    printf("Before cut loop\n");
    print_piece_values(piece_value);
    int *test = VEC_AT(int, dp_table, 0);
    for(int i = 0; i < dp_table->length; i++) {
        printf("%d\n", test[i]);
    }
    while (*rem_cuts != -1) {
        printf("rem cuts = %d\n", *rem_cuts);
        int len_to_cut = *rem_cuts;
        for (int pv_index = 0; pv_index < pv_size; pv_index++) {
            printf("%d\n", pv_index);
            if (VEC_AT(PieceLengthValue, piece_value, pv_index)->length ==
                len_to_cut) {
                int* use_count = VEC_AT(int, piece_use_count, pv_index);
                (*use_count)++;
                break;
            }
        }
        rem_cuts = VEC_AT(int, cuts, supp_length_rem - len_to_cut);
    }
    printf("About to print results\n");
    for (int index = 0; index < pv_size; index++) {
        int* use_count = VEC_AT(int, piece_use_count, index);
        if (*use_count > 0) {
            printf("%d @ %d = %d\n", *use_count,
                   VEC_AT(PieceLengthValue, piece_value, index)->length,
                   *use_count *
                       VEC_AT(PieceLengthValue, piece_value, index)->value);
        }
    }
    printf("Remainder: %d\n", supp_length_rem);
    printf("Value: %d\n", *VEC_AT(int, dp_table, supplied_length));

    vec_free(piece_value);
    vec_free(dp_table);
    vec_free(cuts);

    return 0;
}

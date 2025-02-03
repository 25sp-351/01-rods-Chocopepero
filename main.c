#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

#define BUFFER_SIZE 100
#define BASE_10 10
#define MAX(left, right) (((left) > (right)) ? (left) : (right))

typedef struct {
    int length;
    int value;
    int use_count;
} piece;

inline piece* piece_at(Vec vec, size_t index) {
    return (piece*)vec_at(vec, index);
}

int compare_piecelen(const void* left, const void* right) {
    piece* pieceL = (piece*)left;
    piece* pieceR = (piece*)right;
    return pieceL->length - pieceR->length;
}

Vec read_input() {
    Vec piece_value = new_vec(sizeof(piece));
    int length;
    int value;

    while (scanf("%d, %d", &length, &value) == 2) {
        piece new_piece = {length, value, 0};
        vec_add(piece_value, &new_piece);
    }
    return piece_value;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <length>\n", argv[0]);
        return 1;
    }
    char* endptr;
    int supplied_length = strtol(argv[1], &endptr, BASE_10);
    char buffer[BUFFER_SIZE];

    if (*endptr != '\0' || supplied_length <= 0) {
        fprintf(stderr, "Invalid rod length: %s\n", argv[1]);
        return 1;
    }

    Vec piece_value = read_input();
    size_t pv_size  = vec_length(piece_value);

    qsort(piece_value, pv_size, sizeof(piece), compare_piecelen);

    int* dp_table = malloc((supplied_length + 1) * sizeof(int));
    int* cuts     = malloc((supplied_length + 1) * sizeof(int));

    memset(dp_table, 0, (supplied_length + 1) * sizeof(int));
    memset(cuts, -1, (supplied_length + 1) * sizeof(int));

    for (int curr_length = 1; curr_length <= supplied_length; curr_length++)
        for (int val_index = 0; val_index < pv_size; val_index++) {
            if (piece_at(piece_value, val_index)->length <= curr_length) {
                int temp = dp_table[curr_length];

                dp_table[curr_length] =
                    MAX(dp_table[curr_length],
                        piece_at(piece_value, val_index)->value +
                            dp_table[curr_length -
                                     piece_at(piece_value, val_index)->length]);
                if (temp != dp_table[curr_length])
                    cuts[curr_length] =
                        piece_at(piece_value, val_index)->length;
            }
        }
    int supp_length_rem = supplied_length;
    while (cuts[supp_length_rem] != -1) {
        int len_to_cut = cuts[supplied_length];
        for (int pv_index = 0; pv_index < pv_size; pv_index++) {
            if (piece_at(piece_value, pv_index)->length == len_to_cut) {
                piece_at(piece_value, pv_index)->use_count++;
                break;
            }
        }
        supp_length_rem -= len_to_cut;
    }

    for (int index = 0; index < pv_size; index++) {
        if (piece_at(piece_value, index)->use_count > 0) {
            printf("%d @ %d = %d\n", piece_at(piece_value, index)->use_count,
                   piece_at(piece_value, index)->length,
                   piece_at(piece_value, index)->use_count *
                       piece_at(piece_value, index)->value);
        }
    }
    printf("Remainder: %d\n", supp_length_rem);
    printf("Value: %d\n", dp_table[supplied_length]);

    vec_free(piece_value);
    free(dp_table);
    free(cuts);

    return 0;
}

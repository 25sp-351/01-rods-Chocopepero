#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(left, right) (((left) > (right)) ? (left) : (right))

struct piece {
    int length;
    int value;
    int use_count;
};

int compare(const void* left, const void* right) {
    struct piece* pieceL = (struct piece*)left;
    struct piece* pieceR = (struct piece*)right;
    return pieceL->length - pieceR->length;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <length>\n", argv[0]);
        return 1;
    }
    char* endptr;
    int supplied_length = strtol(argv[1], &endptr, 10);
    char buffer[100];

    if (*endptr != '\0' || supplied_length <= 0) {
        fprintf(stderr, "Invalid rod length: %s\n", argv[1]);
        return 1;
    }

    // Making a pseudo-vector (Thank you 131)
    struct piece* piece_value;
    int capacity = 5;
    int count    = 0;
    piece_value  = malloc(capacity * sizeof(struct piece));

    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (count >= capacity) {
            capacity *= 2;
            piece_value = realloc(piece_value, capacity * sizeof(struct piece));
        }
        sscanf(buffer, "%d, %d", &piece_value[count].length,
               &piece_value[count].value);
        piece_value[count].use_count = 0;
        count++;
    }

    qsort(piece_value, count, sizeof(struct piece), compare);

    int* dp_table = malloc((supplied_length + 1) * sizeof(int));
    int* cuts     = malloc((supplied_length + 1) * sizeof(int));

    memset(dp_table, 0, (supplied_length + 1) * sizeof(int));
    memset(cuts, -1, (supplied_length + 1) * sizeof(int));

    for (int curr_length = 1; curr_length <= supplied_length; curr_length++) {
        for (int val_index = 0; val_index < count; val_index++) {
            if (piece_value[val_index].length <= curr_length) {
                int temp              = dp_table[curr_length];

                dp_table[curr_length] = MAX(
                    dp_table[curr_length],
                    piece_value[val_index].value +
                        dp_table[curr_length - piece_value[val_index].length]);
                if (temp != dp_table[curr_length])
                    cuts[curr_length] = piece_value[val_index].length;
            }
        }
    }
    int supp_length_rem = supplied_length;
    while (cuts[supp_length_rem] != -1) {
        int len_to_cut       = cuts[supplied_length];
        for (int pv_index = 0; pv_index < count; pv_index++) {
            if (piece_value[pv_index].length == len_to_cut) {
                piece_value[pv_index].use_count++;
                break;
            }
        }
        supp_length_rem -= len_to_cut;
    }

    for (int i = 0; i < count; i++) {
        if (piece_value[i].use_count > 0) {
            printf("%d @ %d = %d\n", piece_value[i].use_count,
                   piece_value[i].length,
                   piece_value[i].use_count * piece_value[i].value);
        }
    }
    printf("Remainer: %d\n", supp_length_rem);
    printf("Value: %d\n", dp_table[supplied_length]);

    free(piece_value);
    free(dp_table);
    free(cuts);

    return 0;
}

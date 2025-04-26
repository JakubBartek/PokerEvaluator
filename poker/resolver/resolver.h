#ifndef RESOLVER_H
#define RESOLVER_H

#include "../structs/poker_structs.h"

int compare_desc(const void *a, const void *b);

EvaluatedHand rank_hand_with_kickers(Card hand[5]);

void generate_5card_combinations(Card input[7], Card output[21][5],
    int *num_combinations);

EvaluatedHand evaluate_best_5_of_7(Card cards[7]);

int resolve(int argc, char *argv[], short *winners);

#endif // RESOLVER_H
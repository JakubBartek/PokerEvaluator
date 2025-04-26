#include "../structs/poker_structs.h"

void init_deck(Card *deck) {
    for (int i = 0; i < 52; i++) {
        deck[i].rank = i % 13 + 2;
        deck[i].suite = i % 4;
    }
}

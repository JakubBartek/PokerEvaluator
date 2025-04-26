#ifndef CARD_PARSER_H
#define CARD_PARSER_H

#include "../structs/poker_structs.h"

void card_to_str(Card card, char card_str[3]);

void card_to_char(Card card, char *rank, char *suite);

void str_to_card(char *card_str, Card *card);

int parse_cards(int argc, char *argv[], Player players[], Table *table, short table_card_count, short used_cards[CARDS_IN_DECK]);

short get_number_of_cards(char argv[10]);

#endif // CARD_PARSER_H

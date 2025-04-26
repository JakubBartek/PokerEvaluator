#include <stdio.h>
#include <stdlib.h>
#include "../structs/poker_structs.h"

void card_to_str(Card card, char card_str[3])
{
	switch (card.rank)
	{
	case TWO:
		card_str[0] = '2';
		break;
	case THREE:
		card_str[0] = '3';
		break;
	case FOUR:
		card_str[0] = '4';
		break;
	case FIVE:
		card_str[0] = '5';
		break;
	case SIX:
		card_str[0] = '6';
		break;
	case SEVEN:
		card_str[0] = '7';
		break;
	case EIGHT:
		card_str[0] = '8';
		break;
	case NINE:
		card_str[0] = '9';
		break;
	case TEN:
		card_str[0] = 'T';
		break;
	case JACK:
		card_str[0] = 'J';
		break;
	case QUEEN:
		card_str[0] = 'Q';
		break;
	case KING:
		card_str[0] = 'K';
		break;
	case ACE:
		card_str[0] = 'A';
		break;
	default:
		fprintf(stderr, "Invalid card rank: %d\n", card.rank);
		exit(EXIT_FAILURE);
	}
	switch (card.suite)
	{
	case HEARTS:
		card_str[1] = 'h';
		break;
	case DIAMONDS:
		card_str[1] = 'd';
		break;
	case CLUBS:
		card_str[1] = 'c';
		break;
	case SPADES:
		card_str[1] = 's';
		break;
	default:
		fprintf(stderr, "Invalid card suite: %d\n", card.suite);
		exit(EXIT_FAILURE);
	}

	card_str[2] = '\0';
}	

void card_to_char(Card card, char *rank, char *suite) {

	switch (card.rank)
	{
	case TWO:
		*rank = '2';
		break;
	case THREE:
		*rank = '3';
		break;
	case FOUR:
		*rank = '4';
		break;
	case FIVE:
		*rank = '5';
		break;
	case SIX:
		*rank = '6';
		break;
	case SEVEN:
		*rank = '7';
		break;
	case EIGHT:
		*rank = '8';
		break;
	case NINE:
		*rank = '9';
		break;
	case TEN:
		*rank = 'T';
		break;
	case JACK:
		*rank = 'J';
		break;
	case QUEEN:
		*rank = 'Q';
		break;
	case KING:
		*rank = 'K';
		break;
	case ACE:
		*rank = 'A';
		break;
	default:
		fprintf(stderr, "Invalid card rank: %d\n", card.rank);
		exit(EXIT_FAILURE);
	}
	switch (card.suite)
	{
	case HEARTS:
		*suite = 'h';
		break;
	case DIAMONDS:
		*suite = 'd';
		break;
	case CLUBS:
		*suite = 'c';
		break;
	case SPADES:
		*suite = 's';
		break;
	default:
		fprintf(stderr, "Invalid card suite: %d\n", card.suite);
		exit(EXIT_FAILURE);
	}
}

void str_to_card(char *card_str, Card *card)
{
	switch (card_str[0])
	{
	case '2':
		card->rank = TWO;
		break;
	case '3':
		card->rank = THREE;
		break;
	case '4':
		card->rank = FOUR;
		break;
	case '5':
		card->rank = FIVE;
		break;
	case '6':
		card->rank = SIX;
		break;
	case '7':
		card->rank = SEVEN;
		break;
	case '8':
		card->rank = EIGHT;
		break;
	case '9':
		card->rank = NINE;
		break;
	case 'T':
		card->rank = TEN;
		break;
	case 'J':
		card->rank = JACK;
		break;
	case 'Q':
		card->rank = QUEEN;
		break;
	case 'K':
		card->rank = KING;
		break;
	case 'A':
		card->rank = ACE;
		break;
	default:
		fprintf(stderr, "Invalid card rank: %c\n", card_str[0]);
		exit(EXIT_FAILURE);
	}

	switch (card_str[1])
	{
	case 'h':
		card->suite = HEARTS;
		break;
	case 'd':
		card->suite = DIAMONDS;
		break;
	case 'c':
		card->suite = CLUBS;
		break;
	case 's':
		card->suite = SPADES;
		break;
	default:
		fprintf(stderr, "Invalid card suite: %c\n", card_str[1]);
		exit(EXIT_FAILURE);
	}
}

int parse_cards(int argc, char *argv[], Player players[], Table *table, short table_card_count, short used_cards[CARDS_IN_DECK])
{
	short num_players = argc - 2;

	if (num_players > 9 || num_players < 2)
	{
		return EXIT_FAILURE;
	}

	for (short i = 0; i < num_players; i++)
	{
		players[i].position = i;

		str_to_card(argv[i + 1], &players[i].cards[0]);
		str_to_card(argv[i + 1] + 2, &players[i].cards[1]);
		used_cards[(players[i].cards[0].rank - 2) * 4 + players[i].cards[0].suite] = 1;
		used_cards[(players[i].cards[1].rank - 2) * 4 + players[i].cards[1].suite] = 1;
	}

	for (short i = 0; i < table_card_count; i++)
	{
		str_to_card(argv[num_players + 1] + i * 2, &table->cards[i]);
		used_cards[(table->cards[i].rank - 2) * 4 + table->cards[i].suite] = 1;
	}

	return EXIT_SUCCESS;
}

short get_number_of_cards(char argv[10])
{
    if (argv[0] == '-')
    {
        return 0;
    }

    short count = 0;
    for (short i = 0; argv[i] != '\0'; i += 2)
    {
        count++;
    }
    return count;
}

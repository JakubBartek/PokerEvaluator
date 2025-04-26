#include <stdio.h>
#include "../structs/poker_structs.h"
#include "../utils/card_parser.h"

void print_card(Card card)
{
	char rank_char;
	char suite_char;

	card_to_char(card, &rank_char, &suite_char);

	printf("%c%c", rank_char, suite_char);
}

void print_player(Player player)
{
	printf("Player %d: ", player.position);
	for (int i = 0; i < 2; i++)
	{
		print_card(player.cards[i]);
		if (i < 1)
		{
			printf(" ");
		}
	}
	printf("\n");
}

void print_table(Table table)
{
	printf("Table: ");
	for (int i = 0; i < 5; i++)
	{
		print_card(table.cards[i]);
		if (i < 4)
		{
			printf(" ");
		}
	}
	printf("\n");
}
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structs/poker_structs.h"
#include "../utils/print_poker.h"
#include "../utils/card_parser.h"

int compare_desc(const void *a, const void *b)
{
	return (*(int *)b - *(int *)a);
}

EvaluatedHand rank_hand_with_kickers(Card hand[5])
{
	int rank_count[15] = {0};
	int suit_count[4] = {0};
	int ranks[5];

	for (int i = 0; i < 5; i++)
	{
		rank_count[hand[i].rank]++;
		suit_count[hand[i].suite]++;
		ranks[i] = hand[i].rank;
	}

	int pairs = 0, threes = 0, fours = 0;
	int pair_vals[2] = {0}, three_val = 0, four_val = 0;
	int pair_idx = 0;

	for (int r = ACE; r >= TWO; r--)
	{
		if (rank_count[r] == 4)
		{
			fours = 1;
			four_val = r;
		}
		else if (rank_count[r] == 3)
		{
			threes = 1;
			three_val = r;
		}
		else if (rank_count[r] == 2)
		{
			if (pair_idx < 2)
				pair_vals[pair_idx++] = r;
			pairs++;
		}
	}

	bool is_flush = false;
	for (int s = 0; s < 4; s++)
	{
		if (suit_count[s] == 5)
		{
			is_flush = true;
			break;
		}
	}

	int sorted_ranks[5];
	memcpy(sorted_ranks, ranks, sizeof(ranks));
	qsort(sorted_ranks, 5, sizeof(int), compare_desc);

	bool is_straight = true;
	for (int i = 0; i < 4; i++)
	{
		if (sorted_ranks[i] - 1 != sorted_ranks[i + 1])
		{
			is_straight = false;
			break;
		}
	}

	// Special case: A-2-3-4-5
	if (!is_straight && sorted_ranks[0] == ACE && sorted_ranks[1] == 5 &&
		sorted_ranks[2] == 4 && sorted_ranks[3] == 3 && sorted_ranks[4] == 2)
	{
		is_straight = true;
		sorted_ranks[0] = 5;
		sorted_ranks[4] = ACE;
	}

	EvaluatedHand result;

	if (is_straight && is_flush)
	{
		result.rank = STRAIGHT_FLUSH;
		memcpy(result.kickers, sorted_ranks, sizeof(sorted_ranks));
	}
	else if (fours)
	{
		result.rank = FOUR_OF_A_KIND;
		result.kickers[0] = four_val;
		result.kickers[1] = four_val;
		result.kickers[2] = four_val;
		result.kickers[3] = four_val;
		for (int i = ACE; i >= TWO; i--)
		{
			if (i != four_val && rank_count[i])
			{
				result.kickers[4] = i;
				break;
			}
		}
	}
	else if (threes && pairs)
	{
		result.rank = FULL_HOUSE;
		result.kickers[0] = three_val;
		result.kickers[1] = three_val;
		result.kickers[2] = three_val;
		result.kickers[3] = pair_vals[0];
		result.kickers[4] = pair_vals[0];
	}
	else if (is_flush)
	{
		result.rank = FLUSH;
		memcpy(result.kickers, sorted_ranks, sizeof(sorted_ranks));
	}
	else if (is_straight)
	{
		result.rank = STRAIGHT;
		memcpy(result.kickers, sorted_ranks, sizeof(sorted_ranks));
	}
	else if (threes)
	{
		result.rank = THREE_OF_A_KIND;
		result.kickers[0] = three_val;
		result.kickers[1] = three_val;
		result.kickers[2] = three_val;
		int k = 3;
		for (int i = ACE; i >= TWO && k < 5; i--)
		{
			if (i != three_val && rank_count[i])
			{
				result.kickers[k++] = i;
			}
		}
	}
	else if (pairs == 2)
	{
		result.rank = TWO_PAIR;
		result.kickers[0] = pair_vals[0];
		result.kickers[1] = pair_vals[0];
		result.kickers[2] = pair_vals[1];
		result.kickers[3] = pair_vals[1];
		for (int i = ACE; i >= TWO; i--)
		{
			if (i != pair_vals[0] && i != pair_vals[1] && rank_count[i])
			{
				result.kickers[4] = i;
				break;
			}
		}
	}
	else if (pairs == 1)
	{
		result.rank = ONE_PAIR;
		result.kickers[0] = pair_vals[0];
		result.kickers[1] = pair_vals[0];
		int k = 2;
		for (int i = ACE; i >= TWO && k < 5; i--)
		{
			if (i != pair_vals[0] && rank_count[i])
			{
				result.kickers[k++] = i;
			}
		}
	}
	else
	{
		result.rank = HIGH_CARD;
		memcpy(result.kickers, sorted_ranks, sizeof(sorted_ranks));
	}

	return result;
}

int compare_hands(EvaluatedHand h1, EvaluatedHand h2)
{
	if (h1.rank != h2.rank)
		return h1.rank > h2.rank ? 1 : -1;

	for (int i = 0; i < 5; i++)
	{
		if (h1.kickers[i] > h2.kickers[i])
			return 1;
		if (h1.kickers[i] < h2.kickers[i])
			return -1;
	}

	return 0; // tie
}

void generate_5card_combinations(Card input[7], Card output[21][5],
								 int *num_combinations)
{
	int count = 0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = i + 1; j < 7; j++)
		{
			int k = 0;
			for (int x = 0; x < 7; x++)
			{
				if (x != i && x != j)
				{
					output[count][k++] = input[x];
				}
			}
			count++;
		}
	}
	*num_combinations = count;
}

EvaluatedHand evaluate_best_5_of_7(Card cards[7])
{
	Card combinations[21][5];
	int num_combinations;

	generate_5card_combinations(cards, combinations, &num_combinations);

	EvaluatedHand best = rank_hand_with_kickers(combinations[0]);

	for (int i = 1; i < num_combinations; i++)
	{

		EvaluatedHand current = rank_hand_with_kickers(combinations[i]);

		if (compare_hands(current, best) > 0)
		{
			best = current;
		}
	}
	return best;
}

int resolve(int argc, char *argv[], short *winners)
{
	Player players[MAX_PLAYERS];
	Table table;
	short used_cards[CARDS_IN_DECK] = {0};

	if (parse_cards(argc, argv, players, &table, 5, used_cards) != EXIT_SUCCESS)
	{
		fprintf(stderr, "Error parsing cards\nUsage: 2-9 player's cards + table "
						"cards\n\n Example: ./poker AhAd 2s7c 9cKhQhJhTh");
		return EXIT_FAILURE;
	}

	short num_players = argc - 2;
	EvaluatedHand evaluated[MAX_PLAYERS];

	for (short i = 0; i < num_players; i++)
	{

		Card all_cards[7];
		all_cards[0] = players[i].cards[0];
		all_cards[1] = players[i].cards[1];

		for (int j = 0; j < 5; j++)
		{
			all_cards[2 + j] = table.cards[j];
		}

		evaluated[i] = evaluate_best_5_of_7(all_cards);
	}

	EvaluatedHand best = evaluated[0];
	winners[0] = 1;
	int num_winners = 1;

	for (short i = 1; i < num_players; i++)
	{
		int cmp = compare_hands(evaluated[i], best);

		if (cmp > 0)
		{
			best = evaluated[i];
			memset(winners, 0, sizeof(&winners));
			winners[i] = 1;
			num_winners = 1;
		}
		else if (cmp == 0)
		{
			winners[i] = 1;
			num_winners++;
		}
	}

	// For testing purposes TODO: Remove
	// for (short i = 0; i < num_players; i++)
	// {
	// 	if (winners[i])
	// 	{
	// 		print_player(players[i]);
	// 	}
	// }

	return EXIT_SUCCESS;
}

// TODO: Remove
// int main(int argc, char *argv[])
// {
// 	short winners[MAX_PLAYERS] = {0};
// 	resolve(argc, argv, winners);

// 	return EXIT_SUCCESS;
// }

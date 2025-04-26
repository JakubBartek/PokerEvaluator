#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../structs/poker_structs.h"
#include "../utils/card_parser.h"
#include "../resolver/resolver.h"
#include "../utils/poker_deck.h"
#include "../utils/print_poker.h"

int evaluate(int argc, char *argv[]) {
    Player players[MAX_PLAYERS];
    Table table;
    short used_cards[CARDS_IN_DECK] = {0};

    short table_card_count = get_number_of_cards(argv[argc - 1]);

    if (parse_cards(argc, argv, players, &table, table_card_count, used_cards) != EXIT_SUCCESS)
    {
        fprintf(stderr, "Error parsing cards\nUsage: 2-9 player's cards + table "
                        "cards\n\n Example: ./poker AhAd 2s7c 9cKhQhJhTh");
        return EXIT_FAILURE;
    }

    short num_players = argc - 2;
    short winners[MAX_PLAYERS] = {0};

    int win_counts[MAX_PLAYERS] = {0};
    int total_simulations = 0;
	int tie_count = 0;

    Card deck[CARDS_IN_DECK];
    init_deck(deck); 

    int available_cards[CARDS_IN_DECK];
    int available_count = 0;
    for (int i = 0; i < CARDS_IN_DECK; i++) {
        if (!used_cards[i]) {
            available_cards[available_count++] = i;
        }
    }

    if (table_card_count == 5) {
        if (resolve(argc, argv, winners) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
        for (int i = 0; i < num_players; i++) {
            if (winners[i]) {
                win_counts[i]++;
            }
        }
        total_simulations = 1;
    } else {
        int missing = 5 - table_card_count;

        if (missing == 1) {
            for (int i = 0; i < available_count; i++) {
				char new_card[3];
                card_to_str(deck[available_cards[i]], new_card);

                argv[argc - 1] = strcat(argv[argc - 1], new_card);

				memset(winners, 0, sizeof(winners));
                if (resolve(argc, argv, winners) != EXIT_SUCCESS) {
                    return EXIT_FAILURE;
                }

				int cur_winners = 0;
                for (int j = 0; j < num_players; j++) {
                    if (winners[j]) {
                        win_counts[j]++;
						cur_winners++;
                    }
                }
				if (cur_winners > 1) {
					tie_count++;
					for (int j = 0; j < num_players; j++) {
						if (winners[j]) {
							win_counts[j]--;
						}
					}
				}
                total_simulations++;

                argv[argc - 1][strlen(argv[argc - 1]) - 2] = '\0';
            }
        }
        else if (missing == 2) {
            for (int i = 0; i < available_count; i++) {
                for (int j = i + 1; j < available_count; j++) {

					char new_card1[3], new_card2[3];
					card_to_str(deck[available_cards[i]], new_card1);
					card_to_str(deck[available_cards[j]], new_card2);

					argv[argc - 1] = strcat(argv[argc - 1], new_card1);
					argv[argc - 1] = strcat(argv[argc - 1], new_card2);
					
					memset(winners, 0, sizeof(winners));
                    if (resolve(argc, argv, winners) != EXIT_SUCCESS) {
                        return EXIT_FAILURE;
                    }

					int cur_winners = 0;
                    for (int k = 0; k < num_players; k++) {
                        if (winners[k]) {
                            win_counts[k]++;
							cur_winners++;
                        }
                    }
					if (cur_winners > 1) {
						tie_count++;
						for (int k = 0; k < num_players; k++) {
							if (winners[k]) {
								win_counts[k]--;
							}
						}
					}
                    total_simulations++;

                    argv[argc - 1][strlen(argv[argc - 1]) - 4] = '\0';
                }
            }
        }
		else {
			argv[argc - 1][0] = '\0';
			for (int i = 0; i < available_count; i++) {
				for (int j = i + 1; j < available_count; j++) {
					for (int k = j + 1; k < available_count; k++) {
						for (int l = k + 1; l < available_count; l++) {
							for (int m = l + 1; m < available_count; m++) {

								char new_card1[3], new_card2[3], new_card3[3], new_card4[3], new_card5[3];
								card_to_str(deck[available_cards[i]], new_card1);
								card_to_str(deck[available_cards[j]], new_card2);
								card_to_str(deck[available_cards[k]], new_card3);
								card_to_str(deck[available_cards[l]], new_card4);
								card_to_str(deck[available_cards[m]], new_card5);

								argv[argc - 1] = strcat(argv[argc - 1], new_card1);
								argv[argc - 1] = strcat(argv[argc - 1], new_card2);
								argv[argc - 1] = strcat(argv[argc - 1], new_card3);
								argv[argc - 1] = strcat(argv[argc - 1], new_card4);
								argv[argc - 1] = strcat(argv[argc - 1], new_card5);

								memset(winners, 0, sizeof(winners));
								if (resolve(argc, argv, winners) != EXIT_SUCCESS) {
									return EXIT_FAILURE;
								}

								int cur_winners = 0;
								for (int n = 0; n < num_players; n++) {
									if (winners[n]) {
										win_counts[n]++;
										cur_winners++;
									}
								}
								if (cur_winners > 1) {
									tie_count++;
									for (int n = 0; n < num_players; n++) {
										if (winners[n]) {
											win_counts[n]--;
										}
									}
								}
								total_simulations++;

								argv[argc - 1][strlen(argv[argc - 1]) - 10] = '\0';
							}
						}
					}
				}
			}
		}
    }

    printf("Results after %d simulations:\n", total_simulations);
    for (int i = 0; i < num_players; i++) {
        double percentage = (100.0 * win_counts[i]) / total_simulations;
        printf("Player %d: %.2f%%\n", i + 1, percentage);
    }
	double tie_percentage = (100.0 * tie_count) / total_simulations;
	printf("Tie percentage: %.2f%%\n", tie_percentage);

    return EXIT_SUCCESS;
}

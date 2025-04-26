#ifndef POKER_STRUCTS_H
#define POKER_STRUCTS_H

#define MAX_PLAYERS 9
#define CARDS_IN_DECK 52

typedef enum Suite
{
	HEARTS = 0,
	DIAMONDS,
	CLUBS,
	SPADES
} Suite;

typedef enum Rank
{
	TWO = 2,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	ACE
} Rank;

typedef struct Card
{
	Rank rank;
	Suite suite;
} Card;

typedef enum HandRank
{
	HIGH_CARD = 0,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH
} HandRank;

typedef struct EvaluatedHand
{
	HandRank rank;
	int kickers[5];
} EvaluatedHand;

typedef struct Player
{
	short position;
	Card cards[2];
} Player;

typedef struct Table
{
	Card cards[5];
} Table;

#endif // POKER_STRUCTS_H

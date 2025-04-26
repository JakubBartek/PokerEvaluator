import subprocess
import random
from treys import Card, Evaluator

RANKS = "23456789TJQKA"
SUITS = "hdcs"


def generate_random_hand():
    deck = [r + s for r in RANKS for s in SUITS]
    random.shuffle(deck)
    players = [deck[i:i + 2] for i in range(0, 8, 2)]
    table = deck[34:39]
    return players, table


def evaluate_with_treys(player_hands, table_cards):
    evaluator = Evaluator()
    board = [Card.new(card) for card in table_cards]
    scores = []
    for hand in player_hands:
        hole = [Card.new(card) for card in hand]
        score = evaluator.evaluate(board, hole)
        scores.append(score)
    return scores


def run_c_program(player_hands, table_cards):
    args = ['./poker_resolver'] + [''.join(ph) for ph in player_hands] + [''.join(table_cards)]
    result = subprocess.run(args, capture_output=True, text=True)
    return result.stdout


def main():
    for _ in range(1000):
        players, table = generate_random_hand()
        treys_scores = evaluate_with_treys(players, table)
        c_output = run_c_program(players, table)

        treys_winners = [i for i, score in enumerate(treys_scores) if score == min(treys_scores)]
        c_winners = [int(line.split(":")[0][-1]) for line in c_output.splitlines()]

        if treys_winners != c_winners:
            print(f"Players: {players}")
            print(f"Table: {table}")
            print("Mismatch found!")
            print(f"Treys winners: {treys_winners}")
            print(f"C program winners: {c_winners}")
            print("-" * 40)


if __name__ == "__main__":
    main()

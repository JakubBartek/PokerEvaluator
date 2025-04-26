CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = main.c resolver/resolver.c evaluator/evaluator.c structs/poker_structs.c utils/card_parser.c utils/print_poker.c utils/poker_deck.c
OBJ = $(SRC:.c=.o)
TARGET = poker_evaluator

INCLUDES = -I./structs -I./evaluator -I./resolver

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
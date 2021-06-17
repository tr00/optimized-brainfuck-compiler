CC=clang
CFLAGS=-Wall

SRCDIR=src
OBJDIR=obj

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

$(info $(shell mkdir -p $(OBJDIR)))

all:

debug: CFLAGS=-Wall -Wextra -pedantic -g -fsanitize=address
debug: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*

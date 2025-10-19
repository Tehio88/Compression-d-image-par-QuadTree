# Makefile PROJET Codage hiérarchique par QuadTree 

CC = gcc
CFLAGS = -Wall -std=c17
LDLIBS = -lm

# Fichiers sources et objets
SRC = src/quadtree.c src/qtc.c src/bits.c src/main.c src/pgm.c
OBJ = $(SRC:.c=.o)
EXEC = codec

# Règle principale
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LDLIBS)

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
.PHONY: clean

clean:
	rm -f $(EXEC) $(OBJ)
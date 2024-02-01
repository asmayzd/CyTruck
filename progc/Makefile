CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Cible principale
all: t s

# Compilation des exécutables
t: t.o
	$(CC) $(CFLAGS) -o t t.o

s: s.o
	$(CC) $(CFLAGS) -o s s.o

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Nettoyage
clean:
	rm -f *.o t s

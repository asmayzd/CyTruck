# Nom des compilateurs et options de compilation
CC = gcc
CFLAGS = -Wall -g

# Noms des fichiers sources et objets
S_SRC = s.c
S_OBJ = $(S_SRC:.c=.o)
S_EXE = s

T_SRC = t.c
T_OBJ = $(T_SRC:.c=.o)
T_EXE = t

# Nom de la librairie
LIBS = -lm

# Règle par défaut
all: $(S_EXE) $(T_EXE)

# Règles pour compiler les exécutables
$(S_EXE): $(S_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(T_EXE): $(T_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Règle pour générer les fichiers objets
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et exécutables
clean:
	rm -f $(S_OBJ) $(T_OBJ) $(S_EXE) $(T_EXE)

.PHONY: all clean

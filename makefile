# Compilateur
CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image

# Répertoires
SRC_DIR=Snakefinal/src
OBJ_DIR=$(SRC_DIR)  # Les fichiers objets restent dans src
EXEC=Snakefinal/snake  # L'exécutable sera dans le dossier principal

# Trouver tous les fichiers .c et générer leurs fichiers objets correspondants
SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)

# Règle principale
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJ) $(EXEC)

# Exécution du programme
run: all
	./$(EXEC)

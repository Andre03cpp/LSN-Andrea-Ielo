CC = g++
CFLAGS = -Wall -O3 --std=c++11
# Percorsi alle cartelle esterne
RANDOM_DIR = ../random
LIB_DIR = ../Librerie

# Target principale
all : main.exe main2.exe
	rm -f *.o

# Regole per gli eseguibili
main.exe : main.o $(RANDOM_DIR)/random.o
	$(CC) $(RANDOM_DIR)/random.o main.o -o main.exe

main2.exe : main2.o $(RANDOM_DIR)/random.o
	$(CC) $(RANDOM_DIR)/random.o main2.o -o main2.exe

# Regole per i file oggetto locali
# Aggiungiamo -I$(LIB_DIR) per far trovare gli header in Librerie
main.o : main.cpp $(RANDOM_DIR)/random.h $(LIB_DIR)/Posizione.h $(LIB_DIR)/AlgebraVettoriale.h
	$(CC) -c main.cpp -o main.o $(CFLAGS) -I$(RANDOM_DIR) -I$(LIB_DIR)

main2.o : main2.cpp $(RANDOM_DIR)/random.h $(LIB_DIR)/Posizione.h $(LIB_DIR)/AlgebraVettoriale.h
	$(CC) -c main2.cpp -o main2.o $(CFLAGS) -I$(RANDOM_DIR) -I$(LIB_DIR)

# Regola per compilare random.o
# Deve conoscere Posizione.h se RandomWalk_lattice lo usa
$(RANDOM_DIR)/random.o : $(RANDOM_DIR)/random.cpp $(RANDOM_DIR)/random.h $(LIB_DIR)/Posizione.h
	$(CC) -c $(RANDOM_DIR)/random.cpp -o $(RANDOM_DIR)/random.o $(CFLAGS) -I$(LIB_DIR)

clean :
	rm -f *.o main.exe main2.exe $(RANDOM_DIR)/random.o

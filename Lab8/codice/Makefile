CC = g++
CFLAGS = -Wall -O3 --std=c++11 -I../random -I../Librerie

# Percorsi alle cartelle esterne
RANDOM_DIR = ../random
LIB_DIR = ../Librerie

# Target principale: compila entrambi gli eseguibili
all : main.exe main2.exe

# --- REGOLE PER GLI ESEGUIBILI ---

main.exe : main.o $(RANDOM_DIR)/random.o 
	$(CC) $(RANDOM_DIR)/random.o main.o -o main.exe

main2.exe : main2.o $(RANDOM_DIR)/random.o 
	$(CC) $(RANDOM_DIR)/random.o main2.o -o main2.exe

# --- REGOLE PER I FILE OGGETTO LOCALI ---

main.o : main.cpp $(RANDOM_DIR)/random.h $(LIB_DIR)/Posizione.h $(LIB_DIR)/AlgebraVettoriale.h Simulated_annealing.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

main2.o : main2.cpp $(RANDOM_DIR)/random.h $(LIB_DIR)/Posizione.h $(LIB_DIR)/AlgebraVettoriale.h Simulated_annealing.h
	$(CC) -c main2.cpp -o main2.o $(CFLAGS)

# --- REGOLE PER LE LIBRERIE ESTERNE ---

$(RANDOM_DIR)/random.o : $(RANDOM_DIR)/random.cpp $(RANDOM_DIR)/random.h
	$(CC) -c $(RANDOM_DIR)/random.cpp -o $(RANDOM_DIR)/random.o $(CFLAGS)

# Pulizia di tutti i file oggetto e di entrambi gli eseguibili
clean :
	rm -f *.o main.exe main2.exe $(RANDOM_DIR)/random.o
CC = g++
CFLAGS = -Wall -O3 --std=c++11
# Percorso alla cartella con i file comuni
RANDOM_DIR = ../random

# Target principale
all : main.exe main2.exe main3.exe  
		rm -f *.o

# Regole per gli eseguibili: ora puntano a $(RANDOM_DIR)/random.o
main.exe : main.o $(RANDOM_DIR)/random.o
	$(CC) $(RANDOM_DIR)/random.o main.o -o main.exe

main2.exe : main2.o $(RANDOM_DIR)/random.o
	$(CC) $(RANDOM_DIR)/random.o main2.o -o main2.exe

main3.exe : main3.o $(RANDOM_DIR)/random.o
	$(CC) $(RANDOM_DIR)/random.o main3.o -o main3.exe

# Regole per i file oggetto locali
# Aggiungiamo -I$(RANDOM_DIR) per far trovare l'header random.h al compilatore
main.o : main.cpp $(RANDOM_DIR)/random.h
	$(CC) -c main.cpp -o main.o $(CFLAGS) -I$(RANDOM_DIR)

main2.o : main2.cpp $(RANDOM_DIR)/random.h
	$(CC) -c main2.cpp -o main2.o $(CFLAGS) -I$(RANDOM_DIR)

main3.o : main3.cpp $(RANDOM_DIR)/random.h
	$(CC) -c main3.cpp -o main3.o $(CFLAGS) -I$(RANDOM_DIR)

# Regola per compilare random.o nella sua cartella
$(RANDOM_DIR)/random.o : $(RANDOM_DIR)/random.cpp $(RANDOM_DIR)/random.h
	$(CC) -c $(RANDOM_DIR)/random.cpp -o $(RANDOM_DIR)/random.o $(CFLAGS)

clean :
	rm -f *.o main.exe main2.exe main3.exe

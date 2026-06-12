# Sostituiamo il compilatore standard con quello di Open MPI per il C++
CC = mpicxx
CFLAGS = -O3 --std=c++11
AFLAGS = -larmadillo

# Target 'all' ora compila l'eseguibile parallelo
all: italia.exe

# Target per l'esercizio sull'Italia (usiamo mpicxx anche per il link finale)
italia.exe : Dominio.o Popolazione.o italia.o Individuo.o random.o
	$(CC) Dominio.o Popolazione.o Individuo.o random.o italia.o -o italia.exe $(AFLAGS)

# Regole di compilazione per gli oggetti (tutti compilati con mpicxx e CFLAGS)
italia.o : italia.cpp Dominio.h Popolazione.h Individuo.h random.h
	$(CC) -c italia.cpp -o italia.o $(CFLAGS)

Dominio.o : Dominio.cpp Dominio.h random.h Individuo.h
	$(CC) -c Dominio.cpp -o Dominio.o $(CFLAGS)

Popolazione.o : Popolazione.cpp Popolazione.h random.h Individuo.h
	$(CC) -c Popolazione.cpp -o Popolazione.o $(CFLAGS)

Individuo.o : Individuo.cpp Individuo.h
	$(CC) -c Individuo.cpp -o Individuo.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o italia.exe

remove:
	rm -f ../OUTPUT/*.*
	rm -f ../OUTPUT/CONFIG/*.*
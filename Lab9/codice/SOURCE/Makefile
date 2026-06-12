CC = g++
CFLAGS = -O3 --std=c++11
AFLAGS = -larmadillo

# Target 'all' ora compila entrambi gli eseguibili
all: main.exe italia.exe

# Target per l'esercizio standard (Cerchio/Quadrato)
main.exe : Dominio.o Popolazione.o main.o Individuo.o random.o
	$(CC) Dominio.o Popolazione.o Individuo.o random.o main.o -o main.exe $(AFLAGS)

# Target per l'esercizio sull'Italia
italia.exe : Dominio.o Popolazione.o italia.o Individuo.o random.o
	$(CC) Dominio.o Popolazione.o Individuo.o random.o italia.o -o italia.exe $(AFLAGS)

# Regole di compilazione per gli oggetti
main.o : main.cpp Dominio.h Popolazione.h Individuo.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

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
	rm *.o main.exe italia.exe

remove:
	rm ../OUTPUT/*.*
	rm ../OUTPUT/CONFIG/*.*
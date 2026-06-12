#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // CONTROL LO RIGIDO: Questo codice funziona SOLO ed ESCLUSIVAMENTE con 4 processi.
    // (Anche qui, per fare le cose pulite in MPI sarebbe meglio usare MPI_Abort prima del return).
    if(size != 4) {
        if(rank == 0) cout << "Servono 4 processi, non " << size << "!!" << endl; 
        return 1;
    }

    int icolor, ikey;

    // DEFINIZIONE DELLE REGOLE DI DIVISIONE:
    // 'icolor' decide IN QUALE GRUPPO andrai (stesso colore = stesso gruppo).
    // 'ikey' decide la NUOVA GERARCHIA (il nuovo rank) all'interno del gruppo (valore minore = rank più basso).
    if(rank == 0) { icolor = 1; ikey = 2; } // Va nel Gruppo 1. Vuole un rank alto (chiave 2)
    if(rank == 1) { icolor = 1; ikey = 1; } // Va nel Gruppo 1. Vuole un rank basso (chiave 1)
    if(rank == 2) { icolor = 2; ikey = 1; } // Va nel Gruppo 2. Vuole un rank basso (chiave 1)
    if(rank == 3) { icolor = 2; ikey = 2; } // Va nel Gruppo 2. Vuole un rank alto (chiave 2)

    // Dichiariamo una nuova variabile che conterrà il nostro nuovo sotto-canale radio
    MPI_Comm nuovocom;

    // LA MAGIA: MPI_Comm_split
    // Questa funzione prende il vecchio mondo e lo spacca in base ai colori. I parametri sono:
    // 1. MPI_COMM_WORLD: Il mondo di partenza da dividere.
    // 2. icolor:         Il colore. Chi ha lo stesso icolor finisce nello stesso sotto-gruppo.
    // 3. ikey:           La chiave di ordinamento. All'interno del nuovo gruppo, i rank vengono 
    //                    assegnati in base a chi ha la chiave più piccola.
    // 4. &nuovocom:      Il puntatore dove salvare il nuovo comunicatore creato.
    MPI_Comm_split(MPI_COMM_WORLD, icolor, ikey, &nuovocom);

    // CHIEDIAMO LE NUOVE IDENTITÀ:
    // Adesso chiediamo a 'nuovocom' (e NON a MPI_COMM_WORLD) chi siamo e quanti siamo
    // all'interno del nostro nuovo mini-gruppo.
    int newsize, newrank;
    MPI_Comm_size(nuovocom, &newsize);
    MPI_Comm_rank(nuovocom, &newrank);

    // STAMPA DEI RISULTATI:
    cout << "Ero: " << rank << " di " << size << " ... e adesso sono: " << newrank << " di " << newsize << endl;

    MPI_Finalize();
    return 0;
}
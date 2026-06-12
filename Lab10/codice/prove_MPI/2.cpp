#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Ogni nodo crea il proprio array locale di 3 interi nella sua memoria privata
    int my_values[3];

    // FASE DI INIZIALIZZAZIONE DIFFERENZIATA:
    for(int i = 0; i < 3; i++) {
        if(rank == 0) {
            // Solo il nodo 0 riempie l'array con i numeri [1, 2, 3]
            my_values[i] = i + 1;
        } else {
            // Tutti gli altri nodi (1, 2, 3...) riempiono l'array di zeri [0, 0, 0]
            my_values[i] = 0;
        }
    }

    // STAMPA DI VERIFICA (PRIMA):
    // Qui vedrai che il nodo 0 ha [1, 2, 3] e gli altri hanno [0, 0, 0]
    cout << "Prima: " << my_values[0] << " " << my_values[1] << " " << my_values[2] 
         << " per il processo " << rank << endl;

    // LA MAGIA: MPI_Bcast (Broadcast)
    // Questa funzione agisce come un megafono. I suoi parametri significano:
    // 1. my_values:     Il puntatore ai dati da spedire o ricevere
    // 2. 3:              Quanti elementi stiamo scambiando
    // 3. MPI_INTEGER:    Il tipo di dato (interi)
    // 4. 0:              Il rank del processo SORGENTE (il "capo" che spedisce)
    // 5. MPI_COMM_WORLD: Il canale radio di comunicazione
    // NOTA: Il nodo 0 esegue questa riga per SPEDIRE, tutti gli altri la eseguono per RICEVERE.
    MPI_Bcast(my_values, 3, MPI_INTEGER, 0, MPI_COMM_WORLD);

    // STAMPA DI VERIFICA (DOPO):
    // Grazie al Broadcast, ora la memoria degli altri nodi è stata sovrascritta.
    // Tutti i nodi, nessuno escluso, stamperanno a schermo [1, 2, 3]!
    cout << "Dopo: " << my_values[0] << " " << my_values[1] << " " << my_values[2] 
         << " per il processo " << rank << endl;

    MPI_Finalize();
    return 0;
}
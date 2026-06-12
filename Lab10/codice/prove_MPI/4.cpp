#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Ogni nodo crea due array locali di 2 elementi: uno per i dati da inviare, uno per ricevere
    int isend[2], irecv[2];

    // INIZIALIZZAZIONE DEI DATI:
    // Facciamo finta di lanciare il programma con 3 processi (-np 3 -> rank 0, 1, 2)
    // - Nodo 0 caricherà: isend[0] = 1,  isend[1] = 2
    // - Nodo 1 caricherà: isend[0] = 2,  isend[1] = 3
    // - Nodo 2 caricherà: isend[0] = 3,  isend[1] = 4
    for(int i = 0; i < 2; i++) {
        isend[i] = rank + i + 1;
    }

    // 1a RIDUZIONE: LA SOMMA (MPI_SUM)
    // Prende l'elemento in posizione 0 da TUTTI i nodi (isend[0]), li somma tra loro
    // e salva il risultato finale solo dentro irecv[0] del nodo 0.
    // Calcolo con 3 nodi: (Nodo0: 1) + (Nodo1: 2) + (Nodo2: 3) = 6
    MPI_Reduce(&isend[0], &irecv[0], 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);

    // 2a RIDUZIONE: IL PRODOTTO (MPI_PROD)
    // Prende l'elemento in posizione 1 da TUTTI i nodi (isend[1]), li moltiplica tra loro
    // e salva il risultato finale solo dentro irecv[1] del nodo 0.
    // Calcolo con 3 nodi: (Nodo0: 2) * (Nodo1: 3) * (Nodo2: 4) = 24
    MPI_Reduce(&isend[1], &irecv[1], 1, MPI_INTEGER, MPI_PROD, 0, MPI_COMM_WORLD);

    // IL RISULTATO: Solo il capo stampa a schermo il resoconto delle due riduzioni
    // Se lanciato con -np 3, l'output sarà esattamente: "irecv: 6 24"
    if(rank == 0) {
        cout << "irecv: " << irecv[0] << " " << irecv[1] << endl;
    }

    MPI_Finalize();
    return 0;
}
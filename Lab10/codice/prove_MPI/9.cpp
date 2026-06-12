#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status stat;

    // CRONOMETRO - PARTE 1: MPI_Wtime() restituisce il tempo attuale in secondi
    // (è un tempo ad altissima precisione, come un cronometro da pista).
    double tstart = MPI_Wtime();

    int n = 100;
    int* imesg = new int[n];
    int sum = 0;

    // 🔴 ERRORE DI EFFICIENZA: Questo ciclo for fa qualcosa di terribile!
    for(int i = 0; i < n; i++){
        imesg[i] = rank;

        if(rank == 1) 
            // CORRETTO: Cambiato MPI::INTEGER in MPI_INTEGER
            // PESSIMA IDEA: Chiamare una Send/Recv DENTRO il ciclo for.
            // Stiamo spedendo 100 messaggi separati, uno alla volta! 
            MPI_Send(&imesg[0], n, MPI_INTEGER, 0, i, MPI_COMM_WORLD);
        
        else if(rank == 0) 
            // Il nodo 0 si ferma ad aspettare 100 volte la ricezione.
            MPI_Recv(&imesg[0], n, MPI_INTEGER, 1, i, MPI_COMM_WORLD, &stat);

        sum += imesg[i];
    }

    // CRONOMETRO - PARTE 2: Fermiamo il cronometro e calcoliamo la differenza
    double tend = MPI_Wtime();
    double dt = tend - tstart; // 'dt' conterrà la durata esatta dell'algoritmo

    // Stampa dei risultati (ricordati di rimettere la stringa su una riga sola se era spezzata)
    cout << "io sono " << rank << "; somma = " << sum << "; tempo = " << dt << endl;

    delete[] imesg;
    MPI_Finalize();
    return 0;
}
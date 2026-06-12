#include "mpi.h"
#include <iostream>
using namespace std;

const int n = 100; // Se aumenti di molto questo numero (es. 100000), il programma si blocca!

int main(int argc, char* argv[]){
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    MPI_Status stat1, stat2;
    int itag = 1; 
    int itag2 = 2;

    // Allocazione dinamica di due array di dimensione 'n'
    int* imesg = new int[n]; 
    int* imesg2 = new int[n];

    // Inizializzazione:
    // - Nodo 0 avrà: imesg = [0,0...] e imesg2 = [1,1...]
    // - Nodo 1 avrà: imesg = [1,1...] e imesg2 = [2,2...]
    for(int i = 0; i < n; i++){
        imesg[i] = rank; 
        imesg2[i] = rank + 1;
    }

    // LA TRAPPOLA DEL DEADLOCK:
    if(rank == 1){
        // Il Nodo 1 invia imesg al Nodo 0, e POI si mette in ascolto
        MPI_Send(&imesg[0], n, MPI_INTEGER, 0, itag, MPI_COMM_WORLD);
        MPI_Recv(&imesg2[0], n, MPI_INTEGER, 0, itag2, MPI_COMM_WORLD, &stat2);
        cout << "messaggio ricevuto dal nodo 1 = " << imesg2[0] << endl;
    }
    else if(rank == 0){
        // Il Nodo 0 invia imesg2 al Nodo 1, e POI si mette in ascolto
        MPI_Send(&imesg2[0], n, MPI_INTEGER, 1, itag2, MPI_COMM_WORLD);
        MPI_Recv(&imesg[0], n, MPI_INTEGER, 1, itag, MPI_COMM_WORLD, &stat1);
        cout << "messaggio ricevuto dal nodo 0 = " << imesg[0] << endl;
    }

    // Buona norma: liberare la memoria allocata con new
    delete[] imesg;
    delete[] imesg2;

    MPI_Finalize();
    return 0;
}
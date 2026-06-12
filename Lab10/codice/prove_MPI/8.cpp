#include "mpi.h"
#include <iostream>
using namespace std;

const int n = 100; // Ora puoi aumentarlo quanto vuoi (es. 100000), non ci sarà PIÙ il deadlock!

int main(int argc, char* argv[]){
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    MPI_Status stat1, stat2;
    
    // MPI_Request è fondamentale per le funzioni non bloccanti (con la 'I').
    // È la nostra "ricevuta di spedizione". Ci servirà dopo per verificare se l'invio è terminato.
    MPI_Request req;
    
    int itag = 1; int itag2 = 2;
    int* imesg = new int[n]; int* imesg2 = new int[n];
    for(int i = 0; i < n; i++) { imesg[i] = rank; imesg2[i] = rank + 1; }

    // STRATEGIA ANTISTALLO ASIMMETRICA:
    if(rank == 1){
        // 1. MPI_Isend è NON bloccante. Passiamo l'indirizzo di '&req'.
        // Il Nodo 1 avvia la spedizione in background e scende IMMEDIATAMENTE alla riga dopo.
        MPI_Isend(&imesg[0], n, MPI_INTEGER, 0, itag, MPI_COMM_WORLD, &req);
        
        // 2. Avendo le braccia libere, il Nodo 1 si mette subito in ascolto della risposta del Nodo 0
        MPI_Recv(&imesg2[0], n, MPI_INTEGER, 0, itag2, MPI_COMM_WORLD, &stat2);
        
        // 3. MPI_Wait è la dogana. Dice: "Ok, ora che ho ricevuto i dati del Nodo 0, mi fermo qui 
        // finché non sono sicuro che la MPI_Isend iniziale abbia finito di svuotare il mio array imesg".
        // Serve a evitare di toccare/modificare 'imesg' mentre MPI lo sta ancora spedendo.
        MPI_Wait(&req, &stat1);
        
        cout << "messaggio = " << imesg2[0] << endl;
    }
    else if(rank == 0){
        // Il Nodo 0 usa ancora la vecchia strategia bloccante:
        // Spedisce imesg2 al Nodo 1.
        // PERCHÉ FUNZIONA ORA? Perché dall'altra parte il Nodo 1 non è bloccato a spedire, 
        // ma è già pronto ad ascoltare dentro la sua MPI_Recv! Il canale è libero.
        MPI_Send(&imesg2[0], n, MPI_INTEGER, 1, itag2, MPI_COMM_WORLD);
        
        // Riceve il messaggio mandato dall'Isend del Nodo 1
        MPI_Recv(&imesg[0], n, MPI_INTEGER, 1, itag, MPI_COMM_WORLD, &stat1);
        
        cout << "messaggio = " << imesg[0] << endl;
    }

    delete[] imesg; delete[] imesg2;
    MPI_Finalize();
    return 0;
}
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // MPI_Status è una struttura speciale che serve a raccogliere informazioni
    // sulla ricezione del messaggio (es. chi lo ha mandato davvero, se ci sono errori, ecc.)
    MPI_Status stat;

    // Il tag è come l'oggetto di una mail. Serve a contrassegnare il messaggio 
    // in modo che il ricevente sappia di che tipo di dato si tratta.
    int itag = 1;

    // Ogni nodo mette il proprio rank come contenuto del messaggio.
    // Il nodo 1 scriverà '1' dentro imesg. Il nodo 0 scriverà '0'.
    int imesg = rank;

    // IL MITTENTE: Se sono il nodo 1, spedisco il mio messaggio al nodo 0
    if(rank == 1) {
        // Parametri di MPI_Send:
        // 1. &imesg:          L'indirizzo della variabile da spedire
        // 2. 1:               Quanti elementi spedisco (1 solo intero)
        // 3. MPI_INTEGER:     Il tipo di dato
        // 4. 0:               Il rank del DESTINATARIO (voglio mandarlo al nodo 0)
        // 5. itag:            Il tag/etichetta del messaggio
        // 6. MPI_COMM_WORLD:  Il canale radio globale
        MPI_Send(&imesg, 1, MPI_INTEGER, 0, itag, MPI_COMM_WORLD);
    }
    // IL RICEVENTE: Se sono il nodo 0, mi metto in ascolto del nodo 1
    else if(rank == 0) {
        // Parametri di MPI_Recv:
        // 1. &imesg:          Dove salvare il dato in arrivo (sovrascriverà lo '0' iniziale)
        // 2. 1:               Quanti elementi mi aspetto di ricevere
        // 3. MPI_INTEGER:     Il tipo di dato atteso
        // 4. 1:               Il rank del MITTENTE da cui voglio ricevere (ascolto SOLO il nodo 1)
        // 5. itag:            Accetto solo messaggi con questo specifico tag
        // 6. MPI_COMM_WORLD:  Il canale radio globale
        // 7. &stat:           La struttura in cui salvare i dettagli dello stato di ricezione
        // NOTA: Questa funzione è "bloccante". Il nodo 0 si ferma qui finché il nodo 1 non spedisce.
        MPI_Recv(&imesg, 1, MPI_INTEGER, 1, itag, MPI_COMM_WORLD, &stat);
    }

    // STAMPA DEI RISULTATI:
    // Questa riga viene eseguita da TUTTI i nodi. Ma cosa stamperanno?
    // - Il nodo 1 stamperà "messaggio = 1" (il suo valore originale).
    // - Il nodo 0 stamperà "messaggio = 1" (perché ha ricevuto il valore dal nodo 1 sovrascrivendo il suo 0).
    // - Se hai lanciato più di 2 nodi (es. -np 4), i nodi 2 e 3 stamperanno il proprio rank (2 e 3) 
    //   perché non sono entrati in nessun 'if' e non hanno modificato la variabile.
    cout << "messaggio = " << imesg << " per il processo " << rank << endl;

    MPI_Finalize();
    return 0;
}
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // CONTROLLO DI SICUREZZA (⚠️ ATTENZIONE: Contiene un bug logico di MPI!)
    // Il codice è pensato per funzionare al massimo con 3 processi, perché l'array 'irecv' ha solo 3 slot.
    if(size > 3) {
        if(rank == 0) cout << "Hai scelto troppi processi" << endl;
        // BUG: Fare 'return 1' qui uccide il singolo processo senza avvisare gli altri.
        // Gli altri rimarranno bloccati all'MPI_Gather ad aspettare un nodo che non c'è più!
        // La via corretta in MPI per uscire in caso di errore sarebbe usare MPI_Abort().
        return 1; 
    }

    // Ogni nodo prepara un array di 3 interi riempito di zeri [0, 0, 0].
    // Nota: Questo array servirà DAVVERO solo al nodo 0. Gli altri lo ignorano ma devono comunque dichiararlo.
    int irecv[3];
    for(int i = 0; i < 3; i++) irecv[i] = 0;

    // Ogni nodo prepara il proprio dato individuale da spedire.
    // Il nodo 0 preparerà (0+1) = 1
    // Il nodo 1 preparerà (1+1) = 2
    // Il nodo 2 preparerà (2+1) = 3
    int isend = rank + 1;

    // LA MAGIA: MPI_Gather (La Riscossione)
    // Questa funzione raccoglie i singoli pezzi 'isend' e li unisce in 'irecv'. I parametri sono:
    // 1. &isend:         L'indirizzo del dato locale che QUESTO nodo vuole spedire.
    // 2. 1:              Quanti elementi spedisce questo nodo (1 solo intero).
    // 3. MPI_INTEGER:    Il tipo di dato spedito.
    // 4. irecv:          L'array di destinazione dove il capo raccoglierà tutto (valido solo per il nodo 0).
    // 5. 1:              Quanti elementi ricevere DA CIASCUN NODO (1 intero a testa).
    // 6. MPI_INTEGER:    Il tipo di dato ricevuto.
    // 7. 0:              Il rank del processo SORGENTE/ROOT (il "capo" che colleziona i dati).
    // 8. MPI_COMM_WORLD: Il canale radio di comunicazione.
    MPI_Gather(&isend, 1, MPI_INTEGER, irecv, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

    // IL RISULTATO: Solo il capo stampa a schermo l'array finale compilato
    // Se lanci con -np 3, l'output sarà esattamente: "irecv: 1 2 3"
    // L'ordine nell'array rispetta rigidamente l'ordine dei rank dei nodi!
    if(rank == 0) {
        cout << "irecv: " << irecv[0] << " " << irecv[1] << " " << irecv[2] << endl;
    }

    MPI_Finalize();
    return 0;
}
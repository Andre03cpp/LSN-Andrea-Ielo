#include "mpi.h"       // Questa è la libreria magica! Serve per caricare tutti i comandi di MPI (come Init, Finalize, ecc.)
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    
    // Creiamo due variabili intere che ogni singolo nodo compilerà con i propri dati
    int size, rank; 
    
    // 1. INIZIALIZZAZIONE: Dice al sistema che da questo punto in poi inizia la zona parallela.
    // Prende i parametri del main (argc e argv) perché mpirun potrebbe passare informazioni utili da riga di comando.
    MPI_Init(&argc, &argv);
    
    // 2. QUANTI SIAMO IN TUTTO? (COMM_SIZE)
    // Chiede a MPI: "Quanti processi in totale ha lanciato l'utente?". 
    // Il risultato viene salvato nella variabile 'size' (se lanci con -np 4, size sarà uguale a 4 per tutti).
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // 3. CHI SONO IO? (COMM_RANK)
    // Chiede a MPI: "Qual è il mio numero identificativo personale?".
    // Il risultato viene salvato nella variabile 'rank'. 
    // Se ci sono 4 processi, questa funzione assegnerà 0 al primo, 1 al secondo, 2 al terzo e 3 al quarto.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // 4. L'OUTPUT: Nota che questa riga è scritta UNA VOLTA SOLA nel codice, 
    // ma verrà eseguita in contemporanea da TUTTI i nodi. 
    // Ognuno stamperà a schermo il proprio 'rank' personale.
    cout << " Sono il nodo " << rank << " dei " << size << " che hai utilizzato!" << endl;
    
    // 5. CHIUSURA: Dice a MPI che la computazione parallela è finita.
    // Libera la memoria e chiude i canali di comunicazione tra i vari processi.
    MPI_Finalize();
    
    return 0;
}
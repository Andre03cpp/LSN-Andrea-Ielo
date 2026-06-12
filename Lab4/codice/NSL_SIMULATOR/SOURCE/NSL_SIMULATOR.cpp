/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include "system.h"

using namespace std;

int main (int argc, char *argv[]){

  int nconf = 1;                                // Prima configurazione
  System SYS;                                   // Crea un oggetto di tipo sistema
  SYS.initialize();                             // Inizializza il sistema (legge i file di input, inizializza le variabili, ecc.) 
  SYS.initialize_properties();                  // Inizializza le variabili per la misura delle proprietà (es. energia potenziale, pressione, ecc.)
  SYS.block_reset(0);                           // Resetta gli accumulatori per le medie a blocchi (es. energia potenziale, pressione, ecc.)


  for(int i=0; i < SYS.get_nbl(); i++){         // Per i che scorre lungo il numero di blocchi
    for(int j=0; j < SYS.get_nsteps(); j++){    // Per j che scorre lungo il numero di step in ogni blocco
  // cout << endl << "stampa di DEBUG" << j << endl;

      SYS.step();                               // Esegue un passo della simulazione
      SYS.measure();                            // Misura le proprietà del sistema (es. energia potenziale, pressione, ecc.) e aggiorna gli accumulatori per le medie a blocchi
      // if(j%50 == 0){                            // Se j è multiplo di 50 (ogni 50 step) scrivo la configurazione del sistema in formato .xyz
      //   SYS.write_XYZ(nconf);                   // Write actual configuration in XYZ format //Commented to avoid "filesystem full"! 
      //   nconf++;                                // Incremento il numero di configurazioni per la scrittura dei file .xyz
      // }
      if(i%50 == 0){                            // Se j è multiplo di 50 (ogni 50 step) scrivo la configurazione del sistema in formato .xyz
        SYS.write_XYZ(nconf);                   // Write actual configuration in XYZ format //Commented to avoid "filesystem full"! 
        nconf++;                                // Incremento il numero di configurazioni per la scrittura dei file .xyz
      }

    }

    SYS.averages(i+1);                          // Calcola le medie a blocchi e le medie progressive, e scrive i risultati su file
    SYS.block_reset(i+1);                       // Resetta gli accumulatori per le medie a blocchi (es. energia potenziale, pressione, ecc.) per il blocco successivo
  }

  SYS.finalize();                               // Finalizza la simulazione 

  return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
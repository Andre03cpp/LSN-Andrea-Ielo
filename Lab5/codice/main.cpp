#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>
#include <iomanip>
#include "../random/random.h"


using namespace std;

double psi_100(double x, double y, double z){return 1./(sqrt(M_PI)) * exp(-sqrt(x*x + y*y + z*z));}

double psi_210(double x, double y, double z){return 1./(4.*sqrt(2.*M_PI)) * z * exp(-sqrt(x*x + y*y + z*z)/2.);}

double P_100(double x, double y, double z){return pow(psi_100(x,y,z), 2);}

double P_210(double x, double y, double z){return pow(psi_210(x,y,z), 2);}


int main (int argc, char *argv[]){

   //------------------------------------------------------------------------------------------------
   //--------------------------------parte generatore------------------------------------------------

   // definisco l'istanza rnd che sarà il mio generatore di numeri casuali
   Random rnd;
   // creo un array di 4 interi che conterrà i semi del generatore
   int seed[4];
   // creo due interi che conterranno i primi numeri letti dal file Primes
   int p1, p2;
   // apro il file Primes. Primes rappresenta i parametri del mio generatore.
   // se voglio cambiare numero casuale, posso cambiare i parametri primes.
   ifstream Primes("../random/Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   // apro il file seed.in e leggo il seme del generatore che per qualche motivo è letto con 4 numeri. Ma in realtà è un solo seme.
   ifstream input("../random/seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   // salvo i semi del generatore, in modo da poter ripordurre la stessa sequenza di numeri casuali in futuro
   rnd.SaveSeed();

   //------------------------------------------------------------------------------------------------
   //------------------------------------------------------------------------------------------------
   //--------------Inizio dell'esercizio vero e proprio----------------------------------------------

   // --- PARAMETRI ---
   int M = 100000;                           // numero di simulazioni totale
   int N = 200;                              // Numero di blocchi
   int L = M/N;                              // Numero di simulazioni per blocco
   double step_100 = 1.22;                   // passo del Metropolis che devo regolare per avere un tasso di accettazione del 50%
   double step_210 = 2.9;                    // stessa cosa ma per f. d'onda 2,1,0

   // file output
   ofstream out_100("output_files/100.dat");                   // file di output per f. d'onda 1,0,0
   ofstream out_210("output_files/210.dat");                   // file di output per f. d'onda 2,1,0
   ofstream out_position_100("output_files/position_100.dat"); // file di output per f. d'onda 1,0,0
   ofstream out_position_210("output_files/position_210.dat"); // file di output per f. d'onda 2,1,0


   double x100 = 0., y100 = 0., z100 = 0.;            // coordinate iniziali 
   double x210 = 100., y210 = 100., z210 = 100.;            // coordinate iniziali 

   // Step di equilibrazione del Metropolis
   for(int i = 0; i < 1000; i++){            // lo faccio girare a vuoto per 1000 iterazioni per farlo equilibrare

      double x_new100 = x100 + rnd.Rannyu(-step_100, step_100);
      double y_new100 = y100 + rnd.Rannyu(-step_100, step_100);
      double z_new100 = z100 + rnd.Rannyu(-step_100, step_100);

      double x_new210 = x210 + rnd.Rannyu(-step_210, step_210);
      double y_new210 = y210 + rnd.Rannyu(-step_210, step_210);
      double z_new210 = z210 + rnd.Rannyu(-step_210, step_210);

      double alpha100 = std::min(1.0, P_100(x_new100, y_new100, z_new100) / P_100(x100, y100, z100));
      if(rnd.Rannyu() <= alpha100){x100 = x_new100; y100 = y_new100; z100 = z_new100;}

      double alpha210 = std::min(1.0, P_210(x_new210, y_new210, z_new210) / P_210(x210, y210, z210));
      if(rnd.Rannyu() <= alpha210){x210 = x_new210; y210 = y_new210; z210 = z_new210;}
   }

   double sum_prog100 = 0.;                     // accumulatore data blocking
   double sum2_prog100 = 0.;                    // accumulatore data blocking
   int accepted100 = 0;                         // Per calcolare l'accettazione

   double sum_prog210 = 0.;                     // accumulatore data blocking
   double sum2_prog210 = 0.;                    // accumulatore data blocking
   int accepted210 = 0;                         // Per calcolare l'accettazione

   for (int i=0; i<N; i++){                     // ciclo sui blocchi

      double sum_r_block100 = 0.;               // Accumulatore di r nel singolo blocco
      double sum_r_block210 = 0.;               // Accumulatore di r nel singolo blocco

      for(int j=0; j<L; j++){                   // ciclo sulle simulazioni in un blocco

         // Mossa Metropolis
         double x_new100 = x100 + rnd.Rannyu(-step_100, step_100);
         double y_new100 = y100 + rnd.Rannyu(-step_100, step_100);
         double z_new100 = z100 + rnd.Rannyu(-step_100, step_100);

         double x_new210 = x210 + rnd.Rannyu(-step_210, step_210);
         double y_new210 = y210 + rnd.Rannyu(-step_210, step_210);
         double z_new210 = z210 + rnd.Rannyu(-step_210, step_210);
         
         double alpha100 = std::min(1.0, P_100(x_new100, y_new100, z_new100) / P_100(x100, y100, z100));
         double alpha210 = std::min(1.0, P_210(x_new210, y_new210, z_new210) / P_210(x210, y210, z210));


         // -- prima 1,0,0 --
         if (rnd.Rannyu() <= alpha100) {
            x100 = x_new100;
            y100 = y_new100;
            z100 = z_new100;
            accepted100++;
         } 

         out_position_100 << x100 << " " << y100 << " " << z100 << " " << endl;

         // Misura della grandezza fisica (Raggio)
         double r100 = sqrt(x100*x100 + y100*y100 + z100*z100);
         sum_r_block100 += r100;

         // -- dopo 2,1,0 --
         if (rnd.Rannyu() <= alpha210) {
            x210 = x_new210;
            y210 = y_new210;
            z210 = z_new210;
            accepted210++;
         } 

         out_position_210 << x210 << " " << y210 << " " << z210 << " " << endl;

         // Misura della grandezza fisica (Raggio)
         double r210 = sqrt(x210*x210 + y210*y210 + z210*z210);
         sum_r_block210 += r210;
        

      }  

      // Calcolo della media del blocco attuale
      double r_media_blocco100 = sum_r_block100 / L;
      double r_media_blocco210 = sum_r_block210 / L;
      
      // Aggiorno gli accumulatori progressivi
      sum_prog100 += r_media_blocco100;
      sum2_prog100 += (r_media_blocco100 * r_media_blocco100);

      sum_prog210 += r_media_blocco210;
      sum2_prog210 += (r_media_blocco210 * r_media_blocco210);
      
      // Calcolo media e errore progressivo
      double ave_prog100 = sum_prog100 / (i+1);
      double ave2_prog100 = sum2_prog100 / (i+1);
      double err_prog100 = 0.;

      double ave_prog210 = sum_prog210 / (i+1);
      double ave2_prog210 = sum2_prog210 / (i+1);
      double err_prog210 = 0.;


      if(i > 0){
         err_prog100 = sqrt((ave2_prog100 - ave_prog100*ave_prog100) / i);
         err_prog210 = sqrt((ave2_prog210 - ave_prog210*ave_prog210) / i);
      }
      
      // Stampa su file
      out_100 << i << "\t\t" << ave_prog100 << "\t\t" << err_prog100 << endl;
      out_210 << i << "\t\t" << ave_prog210 << "\t\t" << err_prog210 << endl;
   }

   // Stampa l'accettazione alla fine per controllare
   cout << "Tasso di accettazione 1s: " << (double)accepted100/M << endl;
   cout << "Tasso di accettazione 2p: " << (double)accepted210/M << endl;

   return 0;

}

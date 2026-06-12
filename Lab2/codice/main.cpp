#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>
#include <iomanip>
#include "../random/random.h"


using namespace std;

// Funzione per l'errore statistico (deviazione standard della media)
double error(double av, double av2, int n) {
    if (n == 0) return 0;
    return sqrt((av2 - pow(av, 2)) / n);
}

double f(double x){return M_PI/2 * cos(M_PI*x/2);}

double g(double x){return M_PI/4 * cos(M_PI*x/2) * 1/(1 - x);}


 
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

   // stampo a video 20 numeri casuali generati dal mio generatore
   // for(int i=0; i<20; i++){
   //    cout << rnd.Rannyu() << endl;
   // }

   // salvo i semi del generatore, in modo da poter ripordurre la stessa sequenza di numeri casuali in futuro
   rnd.SaveSeed();

   //------------------------------------------------------------------------------------------------
   //------------------------------------------------------------------------------------------------
   //--------------Inizio dell'esercizio vero e proprio----------------------------------------------

   ofstream out_integrale("output_files/integrale.dat");    // con distribuzione uniforme
   ofstream out_integrale2("output_files/integrale2.dat");   // con sviluppo di taylor di f attorno a 1

   
   // --- PARAMETRI ---
   int M = 10000;       // numero di lanci totale
   int N = 100;         // numero di blocchi
   int L = M/N;         // numero di lanci per blocco 

   // --- Uniforme ---
   double sum1_u = 0.;
   double sum2_u = 0.;
   double integral_u = 0.;  
   
   // --- Sviluppo di Taylor ---
   double sum1_t = 0.;
   double sum2_t = 0.;
   double integral_t = 0.; 

   for(int i=0; i<N; i++){

      double sum_u = 0;
      double sum_t = 0;

      for(int j=0; j<L; j++){
         double x = rnd.Rannyu(); // numero casuale tra 0 e 1
         double y = rnd.Es2_1(); // numero casuale con distribuzione 1 - sqrt(1 - y)
         sum_u += f(x); // sommo il valore della funzione f(x) al totale del blocco
         sum_t += g(y); // sommo il valore della funzione g(x) al totale del blocco
      }

      integral_u = sum_u/L; // media del blocco
      sum1_u += integral_u;
      sum2_u += integral_u*integral_u;

      integral_t = sum_t/L; // media del blocco
      sum1_t += integral_t;
      sum2_t += integral_t*integral_t;

      // calcolo medie progressive
      double ave_u = sum1_u/double(i+1); // media progressiva
      double ave2_u = sum2_u/double(i+1); // media del quadrato
      double error_u = 0.;

      double ave_t = sum1_t/double(i+1); // media progressiva
      double ave2_t = sum2_t/double(i+1); // media del quadrato
      double error_t = 0.;
    
      if (i > 0) // L'errore si calcola dal secondo blocco in poi
      error_u = sqrt((ave2_u - ave_u*ave_u) / i);
      error_t = sqrt((ave2_t - ave_t*ave_t) / i);

      // 4. Salvo su file: [media progressiva] [errore progressivo]
      out_integrale << i << "\t\t" << ave_u << "\t\t" << error_u << endl;
      out_integrale2 << i << "\t\t" << ave_t << "\t\t" << error_t << endl;

   }


   cout << "✅ Dati salvati in files output integrale.dat e integrale2.dat" << endl;


   return 0;

}




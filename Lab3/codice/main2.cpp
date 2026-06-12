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
double error(double av, double av2, int n){
   if (n == 0) return 0;
   return sqrt((av2 - pow(av, 2)) / n);
}

double N(double x){return 0.5 * (1. + erf(x / sqrt(2.)));}

vector<double> black_scholes(double S0, double K, double T, double r, double sigma){
   double d1 = 1./(sigma * sqrt(T)) * (log(S0 / K) + (r + (sigma*sigma) / 2.) * T);
   double d2 = d1 - sigma * sqrt(T);
   double C = S0 * N(d1) - K * exp(-r * T) * N(d2);
   double P = S0 *(N(d1) - 1.) - K * exp(-r * T) * (N(d2)-1.);
   vector<double> result = {C, P};
   return result;
}


 
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

   ofstream out_cd("output_files/call_discreto.dat");
   ofstream out_pd("output_files/put_discreto.dat");

   
   // --- PARAMETRI ---
   int M = 100000;            // numero di simulazioni totale
   int N = 100;               // Numero di blocchi
   int L = M/N;                // Numero di simulazioni per blocco
   double S0 = 100.;          // asset price
   double T = 1.;             // delivery time
   double K = 100.;           // strike price:
   double r = 0.1;            // risk-free interest rate
   double sigma = 0.25;       // volatilità


   // --- Uniforme ---
   double sum_call = 0.;     // somma delle medie su un blocco
   double sum2_call = 0.;     // somma del quadrato delle medie su un blocco
   double sum_put = 0.;     // somma del quadrato delle medie su un blocco
   double sum2_put = 0.;     // somma del quadrato delle medie su un blocco 
   double call;
   double put;  
   double incremento = T/100.;
   double S;

   for(int i=0; i<N; i++){          // ciclo sui blocchi

        double sum_C = 0.;
        double sum_P = 0.;

        for(int j=0; j<L; j++){       // ciclo sulle simulazioni in un blocco
            S = S0;
            for (double t = 0; t < T; t+=incremento){
                double Z = rnd.Gauss(0., 1.);
                S *= exp(((r - (sigma*sigma/ 2.)) * incremento) + (sigma * Z * sqrt(incremento)));
            }
            double C = exp(-r * T) * max(0., S - K);
            double P = exp(-r * T) * max(0., K - S);
            sum_C += C;
            sum_P += P;
        }

        call = sum_C/L;
        sum_call += call;
        sum2_call += call * call;

        put = sum_P/L;
        sum_put += put;
        sum2_put += put * put;

        // calcolo medie progressive
        double ave_call = sum_call/double(i+1); // media progressiva
        double ave2_call = sum2_call/double(i+1); // media del quadrato
        double error_call = 0.;

        double ave_put = sum_put/double(i+1); // media progressiva
        double ave2_put = sum2_put/double(i+1); // media del quadrato
        double error_put = 0.;



        if (i > 0) // L'errore si calcola dal secondo blocco in poi
        error_call = sqrt((ave2_call - ave_call*ave_call) / i);
        error_put = sqrt((ave2_put - ave_put*ave_put) / i);

        // 4. Salvo su file: [media progressiva] [errore progressivo]
        out_cd << i << "\t\t" << ave_call << "\t\t" << error_call << endl;
        out_pd << i << "\t\t" << ave_put << "\t\t" << error_put << endl;

   }


   cout << "✅ Dati salvati in files output call_discreto.dat e put_discreto.dat" << endl;


   return 0;

}




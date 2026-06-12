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


   // --- PARAMETRI ---
   int M = 100000;              // Lanci totali
   int N = 100;                 // Blocchi
   int L = M / N;               // Lanci per blocco

   double sum_prog = 0;         // Accumulatore media progressiva r
   double su2_prog = 0;         // Accumulatore media quadrati progressiva r
   
   double var_prog = 0;         // Accumulatore media progressiva (r-0.5)^2
   double v2_prog = 0;          // Accumulatore media quadrati progressiva (r-0.5)^2

   // --- SETTORE MEDIE ---
   vector<double> medie_r(N, 0);          // Vettore che registra le medie di r per ogni blocco
   vector<double> varianze_r(N,0);        // Vettore che registra le varianze di r per ogni blocco
   vector<double> medie2_r(N, 0);         // Vettore che registra le medie di r^2 per ogni blocco
   vector<double> cum_sum_medie(N,0);     // Vettore che registra le medie cumulative man mano che aumentano i blocchi
   vector<double> cum_su2_medie(N,0);     // Vettore che registra le medie cumulative man mano che aumentano i blocchi
   vector<double> err_prog_r(N,0);        // Vettore che registra l'errore commesso sulle medie cumulative     

   // --- SETTORE VARIANZE ---
   vector<double> medie_v(N, 0);          // Vettore che registra le medie di (r-0.5)^2 per ogni blocco
   vector<double> varianze_v(N,0);        // Vettore che registra le varianze di (r-0.5)^2 per ogni blocco
   vector<double> medie2_v(N, 0);         // Vettore che registra le medie di ((r-0.5)^2)^2 per ogni blocco
   vector<double> cum_sum_varianze(N,0);  // Vettore che registra le varianze cumulative man mano che aumentano i blocchi
   vector<double> cum_su2_varianze(N,0);  // Vettore che registra le varianze cumulative man mano che aumentano i blocchi
   vector<double> err_prog_v(N,0);        // Vettore che registra l'errore commesso sulle varianze cumulative     


   ofstream out_r("output_files/media_r.dat");
   ofstream out_v("output_files/varianza_r.dat");

   // metto un titolo alle colonne
   out_r << "Blocco\t\tMedia(r)-0.5\t\tMedia cumulativa - 0.5\t\tBarre di Errore" << endl;
   out_v << "Blocco\t\tMedia((r-0.5)^2)-1/12\t\tMedia cumulativa - 1/12\t\tBarre di Errore" << endl;

   // --- CICLO SUI BLOCCHI ---
   for (int i = 0; i<N; i++){

      sum_prog = 0;
      su2_prog = 0;

      var_prog = 0;
      v2_prog = 0;

      // --- CICLO SUI LANCI IN UN BLOCCO ---
      for (int j = 0; j<L; j++){

         double r = rnd.Rannyu();

         sum_prog += r;
         su2_prog += pow(r, 2);

         var_prog += pow(r - 0.5, 2);
         v2_prog += pow(pow(r - 0.5, 2), 2);

      }

      medie_r[i] = sum_prog / L;                               // Media del blocco i-esimo
      medie2_r[i] = pow(medie_r[i],2);                         // Media dei quadrati del blocco i-esimo
      varianze_r[i] = su2_prog / L - pow(medie_r[i],2);        // Varianza del blocco i-esimo

      medie_v[i] = var_prog / L;                               // Media del blocco i-esimo
      medie2_v[i] = pow(medie_v[i],2);                         // Media dei quadrati del blocco i-esimo
      varianze_v[i] = v2_prog / L - pow(medie_v[i],2);         // Varianza del blocco i-esimo

      cum_sum_medie[i] = accumulate(medie_r.begin(), medie_r.begin() + i + 1, 0.0) / (i + 1);
      cum_su2_medie[i] = accumulate(medie2_r.begin(), medie2_r.begin() + i + 1, 0.0) / (i + 1);
      err_prog_r[i] = error(cum_sum_medie[i], cum_su2_medie[i], i);

      cum_sum_varianze[i] = accumulate(medie_v.begin(), medie_v.begin() + i + 1, 0.0) / (i + 1);
      cum_su2_varianze[i] = accumulate(medie2_v.begin(), medie2_v.begin() + i + 1, 0.0) / (i + 1);
      err_prog_v[i] = error(cum_sum_varianze[i], cum_su2_varianze[i], i);


      // --- Stampa su file ---
      out_r << i + 1 << "\t\t " << medie_r[i] - 0.5 << " \t\t" << cum_sum_medie[i] - 0.5 << " \t\t" << err_prog_r[i] << endl;
      out_v << i + 1 << "\t\t " << medie_v[i] - 1./12. << " \t\t" << cum_sum_varianze[i] - 1./12. << " \t\t" << err_prog_v[i] << endl;


   }


   out_r.close();
   out_v.close();
   rnd.SaveSeed();

   cout << "✅ Dati salvati in files output media_r.dat e varianza_r.dat" << endl;


   // --- parte 3 dell'esercizio ---

   M = 100;                               // ridefinisco M come il numero di sottointervalli in [0,1]
   int n = 10000;                         // numeri casuali per ogni blocco
   double expected = n/M;                 // valore atteso per ogni sottointervallo

   ofstream out_chi2("output_files/chi2.dat");         // file output per chi2
   
   for (int i = 0; i < M; i++) {
    
      vector<int> observed(M, 0);         // Reset: 100 intervalli vuoti per questo test

      for (int j = 0; j < n; j++){
         double r = rnd.Rannyu();
         int intervallo = (int)(r * M);   // Trovo l'intervallo. (int) perchè r è un double
         observed[intervallo]++;          // Riempio l'intervallo
      }

      double chi2 = 0.;                   // inizializzo ogni volta

      for (int k = 0; k < M; k++){
         chi2 += pow(observed[k] - expected, 2) / expected;
      }

      out_chi2 << chi2 << endl;
   }


   cout << "✅ Dati salvati in files output chi2.dat" << endl;


   return 0;

}




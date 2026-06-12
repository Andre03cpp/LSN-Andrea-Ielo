#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>
#include <iomanip>
#include "../random/random.h"
#include "../Librerie/Posizione.h"
#include "../Librerie/AlgebraVettoriale.h"


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

   // salvo i semi del generatore, in modo da poter ripordurre la stessa sequenza di numeri casuali in futuro
   rnd.SaveSeed();

   //------------------------------------------------------------------------------------------------
   //------------------------------------------------------------------------------------------------
   //--------------Inizio dell'esercizio vero e proprio----------------------------------------------


    /// --- PARAMETRI ---
    int M = 10000;                                              // Numero totale di Random Walk
    int N = 100;                                                // Numero di blocchi
    int L = M/N;                                                // RW per blocco
    double a = 1.0;                                             // Passo del reticolo
    int t_max = 100;                                            // Numero di passi temporali

    vector<double> sum_ave_l(t_max, 0.);                        // vettore che accumula le medie progressiveper ogni passo
    vector<double> sum_av2_l(t_max, 0.);                        // vettore che accumula le medie^2 progressive per ogni passo
    vector<double> sum_ave_c(t_max, 0.);                        // vettore che accumula le medie progressiveper ogni passo nel continuo
    vector<double> sum_av2_c(t_max, 0.);                        // vettore che accumula le medie^2 progressive per ogni passo nel continuo

    // --- SIMULAZIONE ---
    for(int i = 0; i < N; i++){                                 // Ciclo sui blocchi       
        vector<double> block_r2_sum_l(t_max, 0.);               // Per ogni blocco genero un vettore temporaneo per accumulare r^2 di questo blocco per ogni t 
        vector<double> block_r2_sum_c(t_max, 0.);               // Per ogni blocco genero un vettore temporaneo per accumulare r^2 di questo blocco per ogni t 
        for(int j = 0; j < L; j++) {                            // Ciclo sulle RW nel blocco
            Posizione p_l(0.,0.,0.);                            // Ogni RW parte dall'origine    
            Posizione p_c(0.,0.,0.);                            // Ogni RW parte dall'origine       
            for(int t = 0; t < t_max; t++) {                    // Cilco sui passi (tempi)
                rnd.RandomWalk_lattice(p_l, a);                 // Faccio un passo
                rnd.RandomWalk_continuum(p_c, a);               // Faccio un passo
                block_r2_sum_l[t] += p_l.getR2();               // Accumula la distanza quadratica
                block_r2_sum_c[t] += p_c.getR2();               // Accumula la distanza quadratica
            }
        }

        vector<double> misura_l = sqrt(block_r2_sum_l / double(L)); // ho definito l'operatore /
        vector<double> misura_c = sqrt(block_r2_sum_c / double(L)); // ho definito l'operatore /
        sum_ave_l += misura_l;                                      // vettore di somme di misure sui blocchi (ho definito l'operatore +=)
        sum_ave_c += misura_c;                                      // vettore di somme di misure sui blocchi (ho definito l'operatore +=)
        sum_av2_l += (misura_l * misura_l);                         // vettore di somme di misure^2 sui blocchi
        sum_av2_c += (misura_c * misura_c);                         // vettore di somme di misure^2 sui blocchi
    }

    sum_ave_l = sum_ave_l/double(N);                                // ora diventa media sui blocchi
    sum_ave_c = sum_ave_c/double(N);                                // ora diventa media sui blocchi
    sum_av2_l = sum_av2_l/double(N);                                // ora diventa media sui blocchi
    sum_av2_c = sum_av2_c/double(N);                                // ora diventa media sui blocchi

    // --- SCRITTURA SU FILE ---
    ofstream out_rw_l("output_files/RW_lattice.dat");
    ofstream out_rw_c("output_files/RW_continuum.dat");

    for(int t = 0; t < t_max; t++){

        double error_l = 0;
        double error_c = 0;
        if (N > 1)
        error_l = sqrt((sum_av2_l[t] - pow(sum_ave_l[t], 2)) / (double)(N-1));  // Errore progressio a ogni tempo
        error_c = sqrt((sum_av2_c[t] - pow(sum_ave_c[t], 2)) / (double)(N-1));  // Errore progressio a ogni tempo

        //        Passo |       Media Distanza |       Errore
        out_rw_l << t + 1 << " " << sum_ave_l[t] << " " << error_l << endl;
        out_rw_c << t + 1 << " " << sum_ave_c[t] << " " << error_c << endl;

    }

    out_rw_l.close();
    out_rw_c.close();
    rnd.SaveSeed();
    
    cout << "✅ Dati salvati in output_files/RW_lattice.dat" << endl;
    cout << "✅ Dati salvati in output_files/RW_continuum.dat" << endl;

   return 0;

}




#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <vector>
#include <numeric>
#include <iomanip>
#include "../random/random.h"
#include "Simulated_annealing.h"


using namespace std;




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

   // -----------------------------------------------------------------------------------------------
   // ------------ FASE PRELIMINARE -----------------------------------------------------------------
   // -----------------------------------------------------------------------------------------------

   // --- PARAMETRI 8.1 ---
   int M = 100000;                           // numero di simulazioni totale    
   int N = 200;                              // numero blocchi
   double step_metro = 1;                    // passo del Metropolis che devo regolare per avere un tasso di accettazione del 50%
   double x = 0.;                            // coordinata iniziale

   // --- PARAMETRI 8.2 ---
   double step_mu = 0.15;                    // step metropolis per mu
   double step_sigma = 0.15;                 // step metropolis per sigma
   int accepted = 0;                         // contatore accettazione metropolis SA
   int total_attempts = 0;                   // Per calcolare la percentuale reale di accettazione SA

   // --- Gestione dei Cicli di Temperatura ---
   const int cicli_lineari = 200;            // Durata della prima fase (discesa lineare)
   const int cicli_esponenziali = 100;       // Durata della seconda fase (discesa esponenziale)
   const int numero_cicli_Metro = cicli_lineari + cicli_esponenziali; // Durata totale del SA (300)

   // --- Range delle Temperature ---
   const double T_iniziale = 2.0;            // Temperatura di partenza a inizio simulazione
   const double T_snodo = 0.01;              // Temperatura finale della fase lineare (e iniziale della exp)
   const double T_finale = 0.001;            // Temperatura minima target alla fine di tutto il SA

   // --- Parametri per la discesa Esponenziale (Fase 2) ---
   // T(t) = A * exp(-B * t). Voglio che a t=0 valga T_snodo e a t=cicli_esponenziali valga T_finale
   double A_exp = T_snodo; 
   double B_exp = (1.0 / double(cicli_esponenziali)) * log(T_snodo / T_finale); 
   // --- Parametri per l'incremento dei passi Metropolis interni n[t] ---

   // n[t] cresce esponenzialmente per esplorare meglio i minimi quando il sistema si congela
   const double n_iniziale = 1.0;           // Passi Metropolis effettuati al primo ciclo (T=2.0)
   const double n_finale = 50.0;            // Passi Metropolis effettuati all'ultimo ciclo (T=0.001)
   double fattore_crescita_n = log(n_finale / n_iniziale) / double(numero_cicli_Metro - 1);

   // --- Vettori del SA ---
   vector<double> T(numero_cicli_Metro);     // Profilo di temperature per ogni ciclo macroscopico
   vector<int> n(numero_cicli_Metro);        // Numero di mosse Metropolis da fare a ciascuna temperatura
   
   double pendenza_lineare = (T_iniziale) / double(cicli_lineari);

   for(int i = 0; i < numero_cicli_Metro; i++){
      
      // 1. Calcolo dinamico di n[i]: cresce da n_iniziale a n_finale
      n[i] = int(n_iniziale * exp(fattore_crescita_n * i));

      // 2. Calcolo del profilo di Temperatura T[i]
      if(i < cicli_lineari){
         // FASE 1: Discesa lineare da T_iniziale fino a T_snodo
         T[i] = T_iniziale - pendenza_lineare * double(i);                  
      }
      else{
         // FASE 2: Discesa esponenziale da T_snodo fino a T_finale
         int step_fase2 = i - cicli_lineari + 1; // Contatore che riparte da 0 per la seconda fase
         T[i] = A_exp * exp(-B_exp * step_fase2);    
      }
   }

   // -----------------------------------------------------------------------------------------------
   // ------------ FINE FASE PRELIMINARE ------------------------------------------------------------
   // -----------------------------------------------------------------------------------------------

   double mu = 1.;                           // valore iniziale parametro mu
   double sigma = 0.5;                       // valore iniziale parametro sigma

   // double H_T = H(mu, sigma, step_metro, M, N, x);
   vector<double> H_vec_current = H(rnd, mu, sigma, step_metro, M, N, x, false); // calcolo una sola volta
   double H_current     = H_vec_current[0];          // La media
   double H_current_err = H_vec_current[1];          // L'errore 

   ofstream out_SA("output_files/S_A_trajectory.dat");
   ofstream out_best("output_files/S_A_best_parameters.dat");

   // ciclo simulated annealing
   for(int t = 0; t < numero_cicli_Metro; t++){

      cout << "Temperatura corrente: " << setw(5) << T[t] << " | Energia: " << setw(7) << setprecision(4) << H_current << " | mu: " << setw(7) << mu << " | sigma: " << setw(7) << sigma << endl;

      for(int i=0; i<n[t]; i++){

         total_attempts++;

         if(t == numero_cicli_Metro - 1){out_best << T[t] << "\t" << mu << "\t" << sigma << "\t" << H_current << "\t" << H_current_err << "\n";}

         // Mossa Metropolis
         double mu_new = mu + rnd.Rannyu(-step_mu, step_mu);
         double sigma_new = sigma + rnd.Rannyu(-step_sigma, step_sigma);

         if(sigma_new <= 0 || mu_new <= 0) continue;    // controllo su mu e sigma

         vector<double> H_vec_new = H(rnd, mu_new, sigma_new, step_metro, M, N, x, false); // calcolo una sola volta
         double H_new     = H_vec_new[0];          // La media
         double H_new_err = H_vec_new[1];          // L'errore 

         double delta_H = H_new - H_current;
         double alpha = std::min(1.0, exp(- delta_H / T[t]));

         if (rnd.Rannyu() <= alpha){
            mu = mu_new;
            sigma = sigma_new;
            H_current = H_new;
            H_current_err = H_new_err;
            accepted++;
         }



      }  

      out_SA << T[t] << "\t" << mu << "\t" << sigma << "\t" << H_current << "\t" << H_current_err << "\n";


   }

   vector<double> stampa = H(rnd, mu, sigma, step_metro, M, N, x, true);      // serve solo a stampare il file

   // Stampe finali dei parametri ottimizzati
   cout << "\n--- OTTIMIZZAZIONE COMPLETATA ---" << endl;
   cout << "Parametri ottimali trovati:" << endl;
   cout << "mu finale: " << mu << endl;
   cout << "sigma finale: " << sigma << endl;
   cout << "Energia minima stimata: " << H_current << endl;
   cout << "Tasso di accettazione globale del SA: " << (double)accepted / total_attempts << endl << endl;


   return 0;

}




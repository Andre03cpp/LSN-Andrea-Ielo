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

int main(){

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

    // --- ISTROGRAMMA FINALE ---
    // Campiono la distribuzione di probabilità con i parametri finali
    ofstream out_histo("output_files/histo.dat");
    
    int M = 10000000;                         // numero di simulazioni totale    
    double step_metro = 1;                    // passo del Metropolis che devo regolare per avere un tasso di accettazione del 50%
    double x = 0.;                            // coordinata iniziale
    double mu = 0.7989;                       // valore iniziale parametro mu
    double sigma = 0.6137;                    // valore iniziale parametro sigma
    int accepted = 0;                         // contatore accettazione metropolis SA

   
    for(int j=0; j<M; j++){                   // ciclo sulle simulazioni in un blocco

        // Mossa Metropolis
        double x_new = x + rnd.Rannyu(-step_metro, step_metro);
        double alpha = std::min(1.0, P_T(x_new, mu, sigma) / P_T(x, mu, sigma));

        if (rnd.Rannyu() <= alpha){
        x = x_new;
        accepted++;
        } 

        out_histo << x << endl;

    }  

   cout << endl << "file istogramma salvato ✅ \n" << endl;

   return 0;

}

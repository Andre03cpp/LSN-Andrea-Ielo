#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>
#include <iomanip>
#include "../random/random.h"


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

    ofstream out_uniform("output_files/uniform.dat");
    ofstream out_exponential("output_files/exponential.dat");
    ofstream out_lorentzian("output_files/lorentzian.dat");

    out_uniform << "N = 1 \t\tN = 2\t\tN = 10\t\tN = 100" << endl;
    out_exponential << "N = 1 \t\tN = 2\t\tN = 10\t\tN = 100" << endl;
    out_lorentzian << "N = 1 \t\tN = 2\t\tN = 10\t\tN = 100" << endl;

    // --- Parametri ---
    vector<int> N{1, 2, 10, 100};    // vettore di N
    // vector<double> S[N,0];        // vettore di S
    int n = 10000;                   // numero lanci per ogni simulazione
    double lambda = 1.;              // parametro distrib. exp
    double mu = 0.;                  // parametro distrib Lorentz
    double Gamma = 1.;               // parametro distrib Lorentz



    

    /*

    Schemino
    
    i               |  0  |  1  |  2  | 3   | 
    N[i]            |  1  |  2  |  10 | 100 |
    len(rand_vec)   |  1  |  2  |  10 | 100 |
    S_N             |  S1 |  S2 | S10 | S100|


    */

    for (int k = 0; k < n; k++){                             // ciclo su n

        for (int i = 0; i<int(N.size()); i++){               // ciclo su N

            double uniform = 0.;
            double exponential = 0.;
            double lorentzian = 0.;
            // genero N numeri casuali tra 0 e 1, exp e lorentz
            for (int j = 0; j<N[i]; j++){           // ciclo sulla grandezza di N
                uniform += rnd.Rannyu();
                exponential += rnd.Exponential(lambda);
                lorentzian += rnd.Lorentzian(Gamma, mu);
            }

            double S_N_uniform = uniform/double(N[i]);              // cast necessario perchè N è int
            double S_N_exponential = exponential/double(N[i]);      // cast necessario perchè N è int
            double S_N_lorentzian = lorentzian/double(N[i]);        // cast necessario perchè N è int

            // Stampo il valore e un tabulatore, tranne che all'ultimo elemento
            out_uniform << S_N_uniform << (i == int(N.size()) - 1 ? "" : "\t");
            out_exponential << S_N_exponential << (i == int(N.size()) - 1 ? "" : "\t");
            out_lorentzian << S_N_lorentzian << (i == int(N.size()) - 1 ? "" : "\t");

        }

        out_uniform << endl;     // Vado a capo dopo aver scritto i 4 valori
        out_exponential << endl; // Vado a capo dopo aver scritto i 4 valori
        out_lorentzian << endl;  // Vado a capo dopo aver scritto i 4 valori

    }

    cout << "✅ Dati salvati in files output uniform.dat, exponential.dat e lorentzian.dat" << endl;


    return 0;

}




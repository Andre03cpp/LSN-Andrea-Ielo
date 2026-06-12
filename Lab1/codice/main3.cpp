#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>
#include <iomanip>
#include "../random/random.h"


using namespace std;

double sin_theta_rand(Random &rnd){     // passo l'oggetto per riferimento
   double x, y, r;
   do {
      x = rnd.Rannyu();         // Genero x in [0, 1)
      y = rnd.Rannyu();         // Genero y in [0, 1)
      r = x*x + y*y;            // Calcolo la distanza al quadrato
   } while (r > 1.0 or r == 0); // Rigetto se fuori dal cerchio o nell'origine

   return y / sqrt(r);          // Restituisco il seno: cateto opposto / ipotenusa
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


    // salvo i semi del generatore, in modo da poter riprodurre la stessa sequenza di numeri casuali in futuro
    rnd.SaveSeed();

    //------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //--------------Inizio dell'esercizio vero e proprio----------------------------------------------

    ofstream out_pi("output_files/pi.dat");



    // --- Parametri ---
    double L = 0.05;            // 5cm
    double d = 0.10;            // 10cm
    int M = 100000000;          // lanci totali
    int N = 500;               // numero blocchi
    int L_blocco = M/N;         // lanci per blocco
    double sum1 = 0.;
    double sum2 = 0.;


    for (int i = 0; i < N; i++){

        double pi;
        int N_thr = 0;
        int N_hit = 0;

        for (int j = 0; j < L_blocco; j++){

            double x = rnd.Rannyu(0., d/2.);
            double s = sin_theta_rand(rnd);

            N_thr++;
            
            if (x <= (L/2.0) * s)
            N_hit++;                              

        }

        pi = (2. * L * double(N_thr))/(double(N_hit) * d);
        sum1 += pi;
        sum2 += pi*pi;

        // 3. Calcolo medie progressive
        double ave = sum1 / double(i + 1);
        double ave2 = sum2 / double(i + 1);
        double error = 0.;
    
        if (i > 0) // L'errore si calcola dal secondo blocco in poi
        error = sqrt((ave2 - ave*ave) / i);

        // 4. Salvo su file: [media progressiva] [errore progressivo]
        out_pi << i << "\t\t" << pi << "\t\t" << ave << "\t\t" << error << endl;


    }

    cout << "✅ Dati salvati in files pi.dat" << endl;


    return 0;

}




#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <vector>
#include <numeric>
#include <iomanip>

using namespace std;

double psi_T(double x, double mu, double sigma){return exp( (-0.5) * pow(sigma, -2) * pow(x - mu, 2) ) + exp( (-0.5) * pow(sigma, -2) * pow(x + mu, 2) );}

double P_T(double x, double mu, double sigma){return pow(psi_T(x,mu,sigma), 2);}

double E_loc(double x, double mu, double sigma){
   return (- 1./(2. * pow(sigma, 4))) * ( - pow(sigma, 2) + pow(x, 2) + pow(mu, 2) - (2 * x * mu * std::tanh(x * mu / pow(sigma, 2))) ) + pow(x, 4) - (2.5*pow(x, 2));
}

double Boltzmann(double T, double H){return exp(-H/T);}

vector<double> H(Random &rnd, double mu, double sigma, double step_metro, int M, int N, double& x, bool stampa){

    double H_T;                               // valore aspettato hamiltoniana
    int accepted = 0;                         // Per calcolare l'accettazione
    int L = M/N;                              // Numero di simulazioni per blocco
    vector<double> x_vec(L);                  // vettore che contiene le x_i che genero con il Metropolis che si comportano come P = |psi|^2
    double sum_prog_H_T = 0.;                 // accumulatore data blocking
    double sum2_prog_H_T = 0.;                // accumulatore data blocking
    vector<double> result(2);                 // ritorno il valore finale in questa variabile
    ofstream out_H_T("output_files/H_T.dat"); // file di output per i valori della media progressiva della Hamiltoniana con l'errore progressivo

    for(int i = 0; i < 10000; i++){           // lo faccio girare a vuoto per 10000 iterazioni per farlo equilibrare
        double x_new = x + rnd.Rannyu(-step_metro, step_metro);
        double alpha = std::min(1.0, P_T(x_new, mu, sigma) / P_T(x, mu, sigma));
        if(rnd.Rannyu() <= alpha){x = x_new;}
    }

    for (int i=0; i<N; i++){                  // ciclo sui blocchi

        for(int j=0; j<L; j++){               // ciclo sulle simulazioni in un blocco

            // Mossa Metropolis
            double x_new = x + rnd.Rannyu(-step_metro, step_metro);
            double alpha = std::min(1.0, P_T(x_new, mu, sigma) / P_T(x, mu, sigma));

            if (rnd.Rannyu() <= alpha){
            x = x_new;
            accepted++;
            } 

            x_vec[j] = x;

        }  

        double somma_E_loc = 0;                // variabile di appoggio per calcolare l'integrale
        for(int k = 0; k < L; k++){somma_E_loc += E_loc(x_vec[k], mu, sigma);}
        H_T = 1./L * somma_E_loc;              // valore hamiltoniana

        // Aggiorno gli accumulatori progressivi
        sum_prog_H_T += H_T;
        sum2_prog_H_T += (H_T * H_T);

        // Calcolo media e errore progressivo
        double ave_prog_H_T = sum_prog_H_T / (i+1);
        double ave2_prog_H_T = sum2_prog_H_T / (i+1);
        double err_prog_H_T = 0.;

        if(i > 0){err_prog_H_T = sqrt((ave2_prog_H_T - ave_prog_H_T*ave_prog_H_T) / i);}

        // Stampa su file
        if(stampa == true){out_H_T << i << "\t\t" << ave_prog_H_T << "\t\t" << err_prog_H_T << endl;}
    }

    if(stampa == true){out_H_T.close();}

    // Stampa l'accettazione alla fine per controllare
    // cout << "Tasso di accettazione: " << (double)accepted/M << endl;

    double final_ave = sum_prog_H_T / double(N);
    double final_ave2 = sum2_prog_H_T / double(N);
    double final_err = 0.;
    final_err = sqrt((final_ave2 - final_ave * final_ave) / double(N - 1));

    // out_H_T.close();

    // Stampa l'accettazione alla fine per controllare
    // cout << "Tasso di accettazione: " << (double)accepted/M << endl;

    // Salviamo i risultati nel vettore da ritornare
    result[0] = final_ave;  // Media finale dell'energia
    result[1] = final_err;  // Errore statistico finale
    
    return result;
}


// vector<int> search_best(double H_new, double mu_new, double sigma_new){

// }
#include <iostream>
#include <iomanip>
#include "Popolazione.h"
#include "Dominio.h"

using namespace std;

int main(int argc, char *argv[]) {

  int n_individui = 100;
  int n_citta = 34;
  int tipo_di_dominio = 1;      // 0 per cerchio, 1 per quadrato
  int parametro_p = 2;
  double probabilita_pair_permutation = 0.1;
  double probabilita_block_permutation = 0.1;
  double probabilita_block_shift = 0.1;
  double probabilita_block_inversion = 0.1;
  double probabilita_crossover = 0.5;

  // ------------------------------------------------------------------------------------------------------------------
  // -------------------------PARTE GENERATORE RANDOM------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------

  Random rnd;
    int p1, p2; // Read from ../INPUT/Primes a pair of numbers to be used to initialize the RNG
    ifstream Primes("../INPUT/Primes");
    Primes >> p1 >> p2 ;
    Primes.close();
    int seed[4]; // Read the seed of the RNG
    ifstream Seed("../INPUT/seed.in");
    Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
    rnd.SetRandom(seed,p1,p2);
  
  
  // ------------------------------------------------------------------------------------------------------------------
  // -------------------------GENERO UNA POPOLAZIONE-------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------

  Popolazione P(n_citta, n_individui);
  P.inizializza_popolazione(rnd);
  vector<int> appo;

  // ------------------------------------------------------------------------------------------------------------------
  // -------------------------GENERO UN DOMINIO------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------

  Dominio D(n_citta, tipo_di_dominio, rnd);
  D.print_coordinates(tipo_di_dominio);

  // ------------------------------------------------------------------------------------------------------------------
  // -------------------------ORDINO LA POPOLAZIONE IN BASE ALLA LOSS--------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------

  D.sort_L1(P);

  // ------------------------------------------------------------------------------------------------------------------
  // -------------------------SELEZIONO 2 GENITORI-------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------

  // for (int i = 0; i < n_individui; i++){
  //   cout << "Individuo [" << i << "] \t --> \t" <<D.get_distance(P.get_individuo(i)) << endl;
  // }

  // file output misura L1 della metà migliore della popolazione in funzione del numero di iterazioni
  ofstream out_L1_mean("../OUTPUT/Dati_L1(iterazioni).dat");
  ofstream out_L1_best("../OUTPUT/Dati_best(iterazioni).dat");

  int n_iterazioni = 20000;
  for (int j = 0; j < n_iterazioni; j++){

    Popolazione P_new(n_citta, n_individui);

    D.sort_L1(P);

    out_L1_mean << D.mean_distance_best_half(P) << endl;           // a ogni iterazione DOPO AVER ORDINATO stampo la distanza media della migliore metà
    out_L1_best << D.get_distance(P.get_individuo(0)) << endl;     // a ogni iterazione DOPO AVER ORDINATO stampo la distanza migliore

    for (int i = 0; i < P.get_numero_individui(); i+=2){


      int index_mamma = D.selection_operator(P, parametro_p, rnd);
      int index_papa = D.selection_operator(P, parametro_p, rnd);

      Individuo mamma = P.get_individuo(index_mamma);
      Individuo papa = P.get_individuo(index_papa);

      double r_pair_permutation_mamma   = rnd.Rannyu();
      double r_block_permutation_mamma  = rnd.Rannyu();
      double r_block_shift_mamma        = rnd.Rannyu();
      double r_block_inversion_mamma    = rnd.Rannyu();

      if (r_pair_permutation_mamma  < probabilita_pair_permutation){mamma.mutation_pair_permutation(rnd);}
      if (r_block_permutation_mamma < probabilita_block_permutation){mamma.mutation_block_permutation(rnd, 3, 5);}
      if (r_block_shift_mamma       < probabilita_block_shift){mamma.mutation_block_shift(rnd, 3, 5);}
      if (r_block_inversion_mamma   < probabilita_block_inversion){mamma.mutation_block_inversion(rnd, 3);}

      double r_pair_permutation_papa   = rnd.Rannyu();
      double r_block_permutation_papa  = rnd.Rannyu();
      double r_block_shift_papa        = rnd.Rannyu();
      double r_block_inversion_papa    = rnd.Rannyu();

      if (r_pair_permutation_papa  < probabilita_pair_permutation){papa.mutation_pair_permutation(rnd);}
      if (r_block_permutation_papa < probabilita_block_permutation){papa.mutation_block_permutation(rnd, 3, 5);}
      if (r_block_shift_papa       < probabilita_block_shift){papa.mutation_block_shift(rnd, 3, 5);}
      if (r_block_inversion_papa   < probabilita_block_inversion){papa.mutation_block_inversion(rnd, 3);}

      double r_crossover = rnd.Rannyu();

      if (r_crossover < probabilita_crossover){Individuo::Crossover(rnd, mamma, papa);}

      P_new.set_individuo(mamma, i);
      P_new.set_individuo(papa, i+1);

    }

    P = P_new;
  }

  D.sort_L1(P);

  out_L1_mean << D.mean_distance_best_half(P);                  // così prendo anche l'ultima
  out_L1_best << D.get_distance(P.get_individuo(0)) << endl;    // così prendo anche l'ultima

  for (int i = 0; i < n_individui; i++){
    cout << "Individuo [" << i << "] \t --> \t" <<D.get_distance(P.get_individuo(i)) << endl;
  }

  // prendo la configurazione del primo individuo dell'ultima popolazione
  vector<int> migliore(n_citta);
  migliore = P.get_individuo(0).get_coordinates();

  // traduco il vettore di città in coordinate
  vector<Coordinata> Coordinate_migliori(n_citta);

  ofstream out_file("../OUTPUT/configurazione_finale.dat");

  for (int i = 0; i < n_citta; i++){
    Coordinate_migliori[i] = D.get_coordinate(migliore[i]);
    out_file << Coordinate_migliori[i].get_x() << " " << Coordinate_migliori[i].get_y() << endl;
  }
  out_file << Coordinate_migliori[0].get_x() << " " << Coordinate_migliori[0].get_y() << endl;
  
  return 0;
}

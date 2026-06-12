#include <iostream>
#include <iomanip>
#include "Popolazione.h"
#include "Dominio.h"

vector<Coordinata> leggi_da_file(string filename){
    vector<Coordinata> temp;
    ifstream in(filename);
    double x, y;
    while(in >> x >> y) {
        temp.push_back(Coordinata(x, y));
    }
    return temp;
}

using namespace std;

int main(int argc, char *argv[]) {

  int n_individui = 100;
  int n_citta = 110;
  int parametro_p = 2;
  double probabilita_pair_permutation = 0.08;
  double probabilita_block_permutation = 0.05;
  double probabilita_block_shift = 0.09;
  double probabilita_block_inversion = 0.09;
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
  
  vector<Coordinata> mappa_italia = leggi_da_file("cap_prov_ita.dat");
  Dominio D(mappa_italia);

  // ------------------------------------------------------------------------------------------------------------------
  // -------------------------ORDINO LA POPOLAZIONE IN BASE ALLA LOSS--------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------

  D.sort_L1(P);

  // ------------------------------------------------------------------------------------------------------------------
  // -------------------------SELEZIONO 2 GENITORI-------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------------------------------

  int n_iterazioni = 10000;
  for (int j = 0; j < n_iterazioni; j++){

    Popolazione P_new(n_citta, n_individui);

    D.sort_L1(P);

    

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
      if (r_block_permutation_mamma < probabilita_block_permutation){mamma.mutation_block_permutation(rnd, 2, 6);}
      if (r_block_shift_mamma       < probabilita_block_shift){mamma.mutation_block_shift(rnd, 2, 8);}
      if (r_block_inversion_mamma   < probabilita_block_inversion){mamma.mutation_block_inversion(rnd, 5);}

      double r_pair_permutation_papa   = rnd.Rannyu();
      double r_block_permutation_papa  = rnd.Rannyu();
      double r_block_shift_papa        = rnd.Rannyu();
      double r_block_inversion_papa    = rnd.Rannyu();

      if (r_pair_permutation_papa  < probabilita_pair_permutation){papa.mutation_pair_permutation(rnd);}
      if (r_block_permutation_papa < probabilita_block_permutation){papa.mutation_block_permutation(rnd, 2, 6);}
      if (r_block_shift_papa       < probabilita_block_shift){papa.mutation_block_shift(rnd, 2, 8);}
      if (r_block_inversion_papa   < probabilita_block_inversion){papa.mutation_block_inversion(rnd, 5);}

      double r_crossover = rnd.Rannyu();

      if (r_crossover < probabilita_crossover){Individuo::Crossover(rnd, mamma, papa);}

      P_new.set_individuo(mamma, i);
      P_new.set_individuo(papa, i+1);

    }

    P = P_new;
  }

  D.sort_L1(P);

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

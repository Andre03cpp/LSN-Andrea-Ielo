#include <iostream>
#include <iomanip>
#include "Popolazione.h"
#include "Dominio.h"
#include "mpi.h"

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

  // Creo due variabili intere che ogni singolo nodo compilerà con i propri dati
  int size, rank; 

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // parametri
  int n_individui = 150;                       
  int n_citta = 110;
  int parametro_p = 2;
  int N_migr = 400;                            
  double probabilita_pair_permutation = 0.05; 
  double probabilita_block_permutation = 0.05; 
  double probabilita_block_shift = 0.0;      
  double probabilita_block_inversion = 0.25;  
  double probabilita_crossover = 0.6;        
  int n_iterazioni = 1000;
  bool migrazione_attiva = true;


  // parte generatore random
  Random rnd;
  int p1, p2;                       
  ifstream Primes("../INPUT/Primes");
  for(int i = 0; i < rank; i++){
    int salta_1, salta_2;             
    Primes >> salta_1 >> salta_2;   
  }
  Primes >> p1 >> p2;               
  Primes.close();
  int seed[4];                       
  ifstream Seed("../INPUT/seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  Seed.close(); 
  // seed[0] += rank;       
  rnd.SetRandom(seed,p1,p2);
  
  // genero una popolazione
  Popolazione P(n_citta, n_individui);
  P.inizializza_popolazione(rnd);
  
  // leggo il dominio
  vector<Coordinata> mappa_italia = leggi_da_file("cap_prov_ita.dat");
  Dominio D(mappa_italia);

  // ordino in base alla loss
  D.sort_L1(P);

  ofstream out_L1_mean, out_L1_best;    // Solo il rank 0 scrive il log principale
  if (rank == 0) {out_L1_mean.open("../OUTPUT/Dati_L1(iterazioni).dat"); out_L1_best.open("../OUTPUT/Dati_best(iterazioni).dat");}

  // ciclo genetico
  for (int j = 0; j < n_iterazioni; j++){

    Popolazione P_new(n_citta, n_individui);

    D.sort_L1(P);
    

    for (int i = 0; i < P.get_numero_individui(); i+=2){


      int index_mamma = D.selection_operator(P, parametro_p, rnd);
      int index_papa = D.selection_operator(P, parametro_p, rnd);

      Individuo mamma = P.get_individuo(index_mamma);
      Individuo papa = P.get_individuo(index_papa);

      // numeri casuali legati alle probabilità di mutazione
      double r_pair_permutation_mamma   = rnd.Rannyu();
      double r_block_permutation_mamma  = rnd.Rannyu();
      double r_block_shift_mamma        = rnd.Rannyu();
      double r_block_inversion_mamma    = rnd.Rannyu();

      // numeri casuali legati ai parametri delle funzioni
      int parameter1 = int(rnd.Rannyu(2, 16));
      int parameter2 = int(rnd.Rannyu(1, n_citta / 2));
      int parameter3 = int(rnd.Rannyu(2, 10));
      int parameter4 = int(rnd.Rannyu(1, 6));
      int parameter5 = int(rnd.Rannyu(2, n_citta / 4 + 1));

      if (r_pair_permutation_mamma  < probabilita_pair_permutation){mamma.mutation_pair_permutation(rnd);}
      if (r_block_permutation_mamma < probabilita_block_permutation){mamma.mutation_block_permutation(rnd, parameter1, parameter2);}
      if (r_block_shift_mamma       < probabilita_block_shift){mamma.mutation_block_shift(rnd, parameter3, parameter4);}
      if (r_block_inversion_mamma   < probabilita_block_inversion){mamma.mutation_block_inversion(rnd, parameter5);}

      // numeri casuali legati alle probabilità di mutazione
      double r_pair_permutation_papa   = rnd.Rannyu();
      double r_block_permutation_papa  = rnd.Rannyu();
      double r_block_shift_papa        = rnd.Rannyu();
      double r_block_inversion_papa    = rnd.Rannyu();

      if (r_pair_permutation_papa  < probabilita_pair_permutation){papa.mutation_pair_permutation(rnd);}
      if (r_block_permutation_papa < probabilita_block_permutation){papa.mutation_block_permutation(rnd, parameter1, parameter2);}
      if (r_block_shift_papa       < probabilita_block_shift){papa.mutation_block_shift(rnd, parameter3, parameter4);}
      if (r_block_inversion_papa   < probabilita_block_inversion){papa.mutation_block_inversion(rnd, parameter5);}


      double r_crossover = rnd.Rannyu();

      if (r_crossover < probabilita_crossover){Individuo::Crossover(rnd, mamma, papa);}

      P_new.set_individuo(mamma, i);
      P_new.set_individuo(papa, i+1);

    }

    P = P_new;

    D.sort_L1(P);                                // Ordino la popolazione aggiornata

    if (rank == 0){
      out_L1_mean << D.mean_distance_best_half(P) << endl;
      out_L1_best << D.get_distance(P.get_individuo(0)) << endl;
    }

    if (migrazione_attiva && j % N_migr == 0 && j != 0){

      D.sort_L1(P);                             // Ordino la popolazione per scambiare l'individuo migliore
      vector<int> I_migliore = P.get_individuo(0).get_coordinates();
      vector<int> migratore(n_citta);

      // Calcola i vicini per l'anello
      int target = (rank + 1) % size;         
      int source = (rank - 1 + size) % size; 

      // Scambio simultaneo
      MPI_Sendrecv(&I_migliore[0], n_citta, MPI_INTEGER, target, 1, &migratore[0], n_citta, MPI_INTEGER, source, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // Prendo l'oggetto Individuo peggiore (l'ultimo, in posizione n_individui - 1)
      Individuo peggiore = P.get_individuo(n_individui - 1);

      // Aggiorno solo il suo vettore interno di città con quello del migratore
      peggiore.set_coordinates(migratore);

      D.sort_L1(P);

      // Rimetto l'individuo modificato dentro la popolazione
      P.set_individuo(peggiore, n_individui - 1);
    }

  }

  // ordino in base alla loss
  D.sort_L1(P);

  double mia_distanza_migliore = D.get_distance(P.get_individuo(0));

  // array sul rank 0 per raccogliere i risultati di tutti i continenti
  vector<double> distanze_globali(size); // Creo un vettore grande quanto il numero di processi
  MPI_Gather(&mia_distanza_migliore, 1, MPI_DOUBLE, distanze_globali.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Variabili per stabilire il vincitore globale
  int rank_vincitore = 0;

  // il rank 0 decide chi ha vinto
  if (rank == 0){
    double min_dist = distanze_globali[0];

    for (int i = 1; i < size; i++){

      if (distanze_globali[i] < min_dist){
        min_dist = distanze_globali[i];
        rank_vincitore = i;
      }

    }

    cout << "\n=============================================" << endl;
    cout << "Il continente migliore è il Rank: " << rank_vincitore << endl;
    cout << "Distanza minima assoluta trovata: " << min_dist << endl;
    cout << "=============================================\n" << endl;
  }

  // Comunicoa tutti i rank chi è il vincitore usando un Broadcast
  MPI_Bcast(&rank_vincitore, 1, MPI_INT, 0, MPI_COMM_WORLD);

  vector<int> migliore(n_citta);

  // Se rank è il vincitore
  if (rank == rank_vincitore) {
    migliore = P.get_individuo(0).get_coordinates();
  }

  // Il vincitore manda il suo tragitto a tutti
  MPI_Bcast(&migliore[0], n_citta, MPI_INTEGER, rank_vincitore, MPI_COMM_WORLD);

  // Solo il rank 0 si occupa di scrivere fisicamente sul disco
  if (rank == 0) {
    // Traduco il vettore di città in coordinate
    vector<Coordinata> Coordinate_migliori(n_citta);
    ofstream out_file("../OUTPUT/configurazione_finale.dat");

    if (!out_file) {cerr << "Errore" << endl;} 
    else {
      // Stampo il percorso nel file
      for (int i = 0; i < n_citta; i++) {
        Coordinate_migliori[i] = D.get_coordinate(migliore[i]);
        out_file << Coordinate_migliori[i].get_x() << " " << Coordinate_migliori[i].get_y() << endl;
      }
      // Chiudo l'anello tornando alla prima città
      out_file << Coordinate_migliori[0].get_x() << " " << Coordinate_migliori[0].get_y() << endl;
      out_file.close();
      cout << "File configurazione_finale.dat scritto con successo dal rank 0!" << endl;
    }
  }

  MPI_Finalize();
  
  return 0;
}

#ifndef __Individuo__
#define __Individuo__

#include "random.h"
#include <vector>

using namespace std;

class Individuo {

  private:
    vector<int> _elemento;
    int _numero_citta;

  public:
    Individuo();
    Individuo(int numero_citta);
    void inizializza_individuo(Random& rnd);
    void print_individuo();
    const vector<int>& get_coordinates() const;
    void mutation_pair_permutation(Random& rnd);
    void mutation_block_permutation(Random& rnd, int block_size, int lunghezza_spostamento);
    void mutation_block_shift(Random& rnd, int block_size, int lunghezza_spostamento);
    void mutation_block_inversion(Random& rnd, int block_size);
    void check_function();

    static void Crossover(Random& rnd, Individuo& mamma, Individuo& papa); // metodo statico
};

#endif // __Individuo__

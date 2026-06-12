#ifndef __Popolazione__
#define __Popolazione__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "Individuo.h"
#include "random.h"

using namespace std;

class Popolazione {

private:
  int _numero_individui;
  vector <Individuo> _pop;

public:
  Popolazione(int numero_citta, int numero_individui);
  int get_numero_individui();
  void inizializza_popolazione(Random& rnd);
  void initialize_random(Random& rnd);
  const Individuo& get_individuo(int i) const;
  Individuo& get_individuo(int i);
  vector<Individuo>& get_vettore_individui();
  const vector<Individuo>& get_vettore_individui() const;
  void replace(Individuo& mamma, Individuo& papa, int index);
  void set_individuo(const Individuo& I, int index);
  Popolazione& operator=(const Popolazione& other);
  void best_half();

};

#endif // __Popolazione__

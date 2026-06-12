#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include "Popolazione.h"

using namespace std;

Popolazione :: Popolazione(int numero_citta, int numero_individui){
  _numero_individui = numero_individui;
  _pop.resize(_numero_individui);

  for(int i=0; i<_numero_individui; i++){
    _pop[i] = Individuo(numero_citta); 
  }
}


int Popolazione :: get_numero_individui(){
  return _numero_individui;
}

void Popolazione :: inizializza_popolazione(Random& rnd){
  for(int i=0; i<_numero_individui; i++){
    _pop[i].inizializza_individuo(rnd);
  }
}

void Popolazione :: set_individuo(const Individuo& I, int index){
  _pop[index] = I;
}


// a quanto pare devo mettere due vesioni, una const invariata e una non const da poter modificare con i setter
const Individuo& Popolazione::get_individuo(int i) const{
  return _pop[i];
}

Individuo& Popolazione::get_individuo(int i){
  return _pop[i];
}


vector<Individuo>& Popolazione:: get_vettore_individui(){
  return _pop;
}

const vector<Individuo>& Popolazione:: get_vettore_individui() const{
  return _pop;
}

void Popolazione :: replace(Individuo& mamma, Individuo& papa, int index){
  _pop[index] = mamma;
  _pop[index + 1] = papa;
}

Popolazione& Popolazione::operator=(const Popolazione& other) {
  if (this == &other) return *this; 

  _numero_individui = other._numero_individui;
  _pop = other._pop;

  return *this;
}


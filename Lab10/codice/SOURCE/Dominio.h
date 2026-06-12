#ifndef __Dominio__
#define __Dominio__

#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "random.h"
#include "Individuo.h"
#include "Popolazione.h"

using namespace std;

class Coordinata {
  private: 
    double _x;
    double _y;

  public:
    Coordinata() = default;
    Coordinata(double x, double y) : _x(x), _y(y) {}
    void set_x(double x){_x = x;};
    void set_y(double y){_y = y;};
    double get_x() const {return _x;};
    double get_y() const {return _y;};
   
};

class Dominio {

  private:
    int _numero_citta;
    int _tipo_di_dominio;
    vector<Coordinata> _citta;

  public:
    Dominio();
    Dominio(vector<Coordinata> mappa); // questo serve per leggere una configurazione input
    Dominio(int numero_citta, int tipo_di_dominio, Random& rnd);
    void print_coordinates(int tipo_di_dominio);
    double get_distance(const Individuo& I);
    Coordinata get_coordinate(int index);
    void sort_L1(Popolazione& P);
    int selection_operator(Popolazione& P_ordered, int parametro_p, Random& rnd);
    double mean_distance_best_half(Popolazione& P);

};

#endif // __Dominio__

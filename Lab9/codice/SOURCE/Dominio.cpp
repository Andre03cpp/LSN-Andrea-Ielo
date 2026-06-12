#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <numeric>
#include "random.h"
#include "Dominio.h"
#include "Individuo.h"

using namespace std;

double distanza(Coordinata C1, Coordinata C2){
    return fabs(C1.get_x() - C2.get_x()) + fabs(C1.get_y() - C2.get_y());
}

// costruttore di default
Dominio :: Dominio(){
    _numero_citta = 0;
}

Dominio :: Dominio(vector<Coordinata> mappa){
    _citta = mappa;
    _numero_citta = mappa.size();
}

// costruttore vero e proprio
Dominio::Dominio(int numero_citta, int tipo_di_dominio, Random& rnd){

    _numero_citta = numero_citta;
    _tipo_di_dominio = tipo_di_dominio;
    _citta.resize(_numero_citta);

    if (_tipo_di_dominio == 0){ // 0 significa dominio su una circonferenza
        double raggio = 5.;
        for (int i = 0; i < _numero_citta; i++){
            double theta = rnd.Rannyu(0, 2*M_PI);
            _citta[i].set_x(raggio * cos(theta));
            _citta[i].set_y(raggio * sin(theta));
        }
    }

    else if(_tipo_di_dominio == 1){ 
        double lato = 10.;
        for (int i = 0; i < _numero_citta; i++) {
            _citta[i].set_x(rnd.Rannyu(-5.0, 5.0));
            _citta[i].set_y(rnd.Rannyu(-5.0, 5.0));
        }
    }    
}


void Dominio :: print_coordinates(int tipo_di_dominio){
    if (tipo_di_dominio == 0){
        ofstream out_file("../OUTPUT/cerchio.dat");
        for (int i = 0; i < _numero_citta; i++)
        out_file << _citta[i].get_x() << " " << _citta[i].get_y() << endl;
    }
    else if (tipo_di_dominio == 1){
        ofstream out_file("../OUTPUT/quadrato.dat");
        for (int i = 0; i < _numero_citta; i++)
        out_file << _citta[i].get_x() << " " << _citta[i].get_y() << endl;
    }

}

Coordinata Dominio :: get_coordinate(int index){
    return _citta[index];
}


double Dominio :: get_distance(const Individuo& I){
    // estraggo il percorso da individuo
    vector<int> v;
    v = I.get_coordinates();

    double dist = 0.;

    for (int i = 0; i < _numero_citta; i++){
        if (i == _numero_citta - 1){dist += distanza(_citta[v[i]], _citta[v[0]]);}
        else{dist += distanza(_citta[v[i]], _citta[v[i + 1]]);}
    }

    return dist;
}

void Dominio :: sort_L1(Popolazione& P){
    auto& v = P.get_vettore_individui();
    std::sort(v.begin(), v.end(), [this](const Individuo& a, const Individuo& b) {return this->get_distance(a) < this->get_distance(b);});
}


int Dominio :: selection_operator(Popolazione& P_ordered, int parametro_p, Random& rnd){
    double r = rnd.Rannyu();
    int index = int(P_ordered.get_numero_individui() * pow(r, parametro_p)); // tolgo il "+ 1" perchè in C++ i vettori vanno da 0 a size - 1
    return index;
}


double Dominio :: mean_distance_best_half(Popolazione& P){

    int numero_individui = P.get_numero_individui();
    vector<double> v(numero_individui/2);

    for(int i = 0; i < numero_individui/2; i++){
        v[i] = get_distance(P.get_individuo(i));
    }

    double mean = std::accumulate(v.begin(), v.end(), 0.0) / v.size();

    return mean;

}
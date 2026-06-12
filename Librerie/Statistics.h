#ifndef __Statistics_h__
#define __Statistics_h__

#include <cmath>
#include <vector>

using namespace std;

template <typename T> T somma(const vector<T> &, int);
template <typename T> T somma(const vector<T> &);
template <typename T> T media(const vector<T> &);
template <typename T> T varianza(const vector<T> &);
template <typename T> T devstd(const vector<T> &);
template <typename T> vector<T> sumvec(const vector<T> &, int);
template <typename T> T CoefCorrel(const vector<T> &, const vector<T> &);





//somma le componenti di un vector fino alla componente n
template <typename T>
T somma(const vector<T> & v, int n){
    T conta = 0.;
    for(int i=0; i<n; i++)
    conta += v[i];

    return conta;
}

//sommma tutte le componenti di un vector
template <typename T>
T somma(const vector<T> & v){return somma<T>(v,v.size());}

//media delle componenti di un vector
template <typename T>
T media(const vector<T> & v){return somma<T>(v)/v.size();}

//varianza delle componenti di un vector
template <typename T>
T varianza(const vector<T> & v){
    T conta = 0.;
    T mean = media<T>(v);
    for(unsigned int i=0; i<v.size(); i++)
    conta += pow(v[i]-mean,2);

    return conta/v.size();
}

//deviazione standard delle componenti di un vector
template <typename T>
T devstd(const vector<T> & v){return sqrt(varianza<T>(v));}

//questa funzione somma le componenti di un vettore a n a n
template <typename T>
vector<T> sumvec(const vector<T> & v, int n){
    vector<T> x(v.size()/n);
    double accu = 0.;
    for(int i=0; i<v.size()/n; i++){
        for(int j=0; j<n; j++){
            accu += v[(n*i) + j];
        }
        x[i] = accu;
        accu = 0.;
    }
    return x;
}

template <typename T>
T CoefCorrel(const vector<T> & x, const vector<T> & y){
    vector<T> xy(x.size());
    for(unsigned int i=0; i<x.size(); i++)
    xy[i] = x[i]*y[i];

    return (media<T>(xy) - (media<T>(x)*media<T>(y)))/(devstd<T>(x)*devstd<T>(y));
}

#endif
#ifndef __AlgebraVettoriale_h__
#define __AlgebraVettoriale_h__

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>  // se se vogliono usare algoritmi STL
#include <numeric>    // per inner_product

using namespace std;



//**************SOMMA DI DUE VETTORI********************
template <typename T> 
vector<T> operator+(const vector<T> &a, const vector<T> &b){
    if(a.size()!= b.size()){
        cerr << "Trying to sum vectors with differen size, exiting" << endl;
        exit(-1);
    }

    vector<T> result(a.size());

    for(int i=0; i<int(a.size()); i++){
    result[i] = a[i] + b[i];
    }

    return result;
}

//**************DIFFERENZA DI DUE VETTORI***************
template <typename T> 
vector<T> operator-(const vector<T> &a, const vector<T> &b){
    if(a.size()!= b.size()){
        cerr << "Trying to subtract vectors with differen size, exiting" << endl;
        exit(-1);
    }

    vector<T> result(a.size());

    for(int i=0; i<int(a.size()); i++){
    result[i] = a[i] - b[i];
    }

    return result;
}

//**************PRODOTTO SCALARE TRA DUE VETTORI********
// template <typename T> 
// T operator*(const vector<T> &a, const vector<T> &b){
//     if(a.size()!= b.size()){
//         cerr << "Trying to do scalar product between vectors with differen size, exiting" << endl;
//         exit(-1);
//     }

//     T sum=0;

//     for(int i=0; i<int(a.size()); i++){
//     sum += a[i]*b[i];
//     }

//     return sum;
// }
template <typename T> 
T dot(const vector<T> &a, const vector<T> &b){
    T sum = 0;
    for(int i = 0; i < int(a.size()); i++) sum += a[i] * b[i];
    return sum;
}

//**************PRODOTTO TRA UNO SCALARE E UN VETTORE***
template <typename T> 
vector<T> operator*(T a, const vector<T> &b){

    vector<T> result(b.size());

    for(int i=0; i<int(b.size()); i++){
    result[i] = a*b[i];
    }

    return result;
}

//**************PRODOTTO TRA UN VETTORE E UNO SCALARE***
template <typename T> 
vector<T> operator*(const vector<T> &b, T a){

    vector<T> result(b.size());

    for(int i=0; i<int(b.size()); i++){
    result[i] = a*b[i];
    }

    return result;
}

//**************DIVISIONE TRA UN VETTORE E UNO SCALARE**
template <typename T> 
vector<T> operator/(const vector<T> &b, T a){

    vector<T> result(b.size());

    for(int i=0; i<int(b.size()); i++){
    result[i] = b[i]/a;
    }

    return result;
}

//**************DEFINISCO OPERATORE +=*****************
template <typename T> 
vector<T>& operator+=(vector<T> &a, const vector<T> &b) {
    // Verifichiamo che i vettori abbiano la stessa dimensione
    if(a.size() != b.size()){
        cerr << endl << "I vettori non hanno la stessa dimensione" << endl; 
        return a;
    }

    for(int i = 0; i < int(a.size()); i++) {
        a[i] += b[i]; // Sommiamo b dentro a
    }
    return a; // Restituiamo il riferimento ad a
}

//**************SQRT**********************************
template <typename T> 
vector<T> sqrt(const vector<T> &b){

    vector<T> result(b.size());

    for(int i=0; i<int(b.size()); i++){
    result[i] = sqrt(b[i]);
    }

    return result;
}

template <typename T>
vector<T> operator*(const vector<T> &a, const vector<T> &b) {
    if(a.size() != b.size()){
        cerr << "ERRORE: Dimensioni incompatibili per il prodotto vettoriale!" << endl;
        return a; 
    }
    
    vector<T> result(a.size());
    for(int i = 0; i < int(a.size()); i++) {
        result[i] = a[i] * b[i];
    }
    return result;
}

//**************STAMPA VETTORE**************************
template <typename T> 
void Print(const vector<T> &a){

    for(int i=0; i<int(a.size()); i++)
    cout << a[i] << endl;
}


#endif
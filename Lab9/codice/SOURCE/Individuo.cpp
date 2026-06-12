#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>
#include "random.h"
#include "Individuo.h"

using namespace std;

// funzione che mi aiuta per le pbc con gli indici
int pbc_index(int index, int numero_citta) {
   int indici_citta = numero_citta - 1;
   return ((index - 1) % indici_citta) + 1;
}


// costruttore di default
Individuo :: Individuo(){
   _numero_citta = 0;
}

// costruttore vero e proprio
Individuo::Individuo(int numero_citta){
   _numero_citta = numero_citta;
   _elemento.resize(_numero_citta); 
}

void Individuo :: inizializza_individuo(Random& rnd){
   if(_elemento.size() == 0){
      cerr << "ERRORE: Tentativo di inizializzare un individuo vuoto!" << endl;
      return; 
   }
   // riempio elemento con città ordinate
   for (int i = 0; i < _numero_citta; i++)
   _elemento[i] = i;

   // ora lo riempio scambiando randomicamente gli indici
   for (int i = _numero_citta - 1; i > 1 ; i--){ // Algoritmo di Fisher - Yates con prima città fissata (i > 1)
      int j = int(rnd.Rannyu(1, i + 1));         // Anche qui parto dall'indice 1 e non 0
      swap(_elemento[i], _elemento[j]);      
   }

   this-> check_function();

   return;
}


const vector<int>& Individuo::get_coordinates() const{
   return _elemento;
}

void Individuo :: print_individuo(){

   cout << "==================================================================================================================================" << endl;

   for (int i = 0; i < _numero_citta; i++) {
      cout << setw(2) << "|" <<_elemento[i] << "|";

      if (i < _numero_citta - 1) {
         cout << " ->";
      }

      if ((i + 1) % 17 == 0) {
         cout << endl;
      }
   }

   cout << endl << "==================================================================================================================================" << endl;
}

void Individuo :: mutation_pair_permutation(Random& rnd){
   int index_1 = int(rnd.Rannyu(1, _numero_citta));   // scrivo fino a numero_città perchè così prende indici da 1 a numero_città (da cosiderare conversione int e rnd)
   int index_2 = int(rnd.Rannyu(1, _numero_citta));
   int appo;

   if (index_1 != index_2){
      swap(_elemento[index_1], _elemento[index_2]);
   }

}

void Individuo :: mutation_block_permutation(Random& rnd, int block_size, int lunghezza_spostamento){
   int start_index = int(rnd.Rannyu(1, _numero_citta));
   for (int i = 0; i < block_size; i++){ // così va fino a block_size - 1
      swap( _elemento[ pbc_index(start_index + i, _numero_citta) ] , _elemento[ pbc_index(start_index + i + lunghezza_spostamento, _numero_citta) ]);
   }
}

void Individuo :: mutation_block_shift(Random& rnd, int block_size, int lunghezza_spostamento){
   int start_index = int(rnd.Rannyu(1, _numero_citta));
   vector<int> copy(block_size);
   for (int i = 0; i < block_size; i++){copy[i] = _elemento[pbc_index(start_index + i, _numero_citta)];}
   for (int i = 0; i < lunghezza_spostamento; i++){_elemento[pbc_index(start_index + i, _numero_citta)] = _elemento[pbc_index(start_index + i + block_size, _numero_citta)];}
   for (int i = 0; i < block_size; i++){_elemento[pbc_index(start_index + i + lunghezza_spostamento, _numero_citta)] = copy[i];}
   
}

void Individuo :: mutation_block_inversion(Random& rnd, int block_size){
   int start_index = int(rnd.Rannyu(1, _numero_citta));
   vector<int> copy(block_size);
   for (int i = 0; i < block_size; i++){copy[i] = _elemento[pbc_index(start_index + i, _numero_citta)];}
   for (int i = 0; i < block_size; i++){_elemento[pbc_index(start_index + i, _numero_citta)] = copy[(block_size -1) - i];}
}

void Individuo :: Crossover(Random& rnd, Individuo& mamma, Individuo& papa) {
   int cut = int(rnd.Rannyu(1, mamma._numero_citta));
   
   vector<int> v_mamma = mamma._elemento; 
   vector<int> v_papa = papa._elemento;

   int pos_scrittura = cut;
   for (int i = 0; i < mamma._numero_citta; i++) {
      int citta_candidata = v_papa[i];
      bool gia_presente = false;
      
      for (int j = 0; j < cut; j++) {
         if (mamma._elemento[j] == citta_candidata) {
            gia_presente = true;
            break;
         }
      }
      
      if (!gia_presente) {
         mamma._elemento[pos_scrittura] = citta_candidata;
         pos_scrittura++;
      }
   }

   pos_scrittura = cut;
   for (int i = 0; i < papa._numero_citta; i++) {
      int citta_candidata = v_mamma[i];
      bool gia_presente = false;
      
      for (int j = 0; j < cut; j++) {
         if (papa._elemento[j] == citta_candidata) {
            gia_presente = true;
            break;
         }
      }
      
      if (!gia_presente) {
         papa._elemento[pos_scrittura] = citta_candidata;
         pos_scrittura++;
      }
   }
}

void Individuo::check_function() {
   bool errore_trovato = false;

   for (int i = 0; i < _numero_citta; i++) {
      if (_elemento[i] >= _numero_citta || _elemento[i] < 0) {
         cerr << "[ERROR] Valore fuori range alla posizione " << i << ": " << _elemento[i] << endl;
         errore_trovato = true;
      }

      for (int j = i + 1; j < _numero_citta; j++) {
         if (_elemento[i] == _elemento[j]) {
            cerr << "[ERROR] Duplicato trovato: la città " << _elemento[i] 
               << " appare alle posizioni " << i << " e " << j << endl;
            errore_trovato = true;
         }
      }
   }

   if (errore_trovato) {
   cerr << "======== errore trovato nell'individuo ========" << endl << endl;
   }
}



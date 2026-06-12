/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "random.h"
#include "../Librerie/Posizione.h"

using namespace std;

Random :: Random(){}
// Default constructor, does not perform any action

Random :: ~Random(){}
// Default destructor, does not perform any action

void Random :: SaveSeed(){
   // This function saves the current state of the random number generator to a file "seed.out"
   ofstream WriteSeed;
   WriteSeed.open("../random/seed.out");
   if (WriteSeed.is_open()){
      WriteSeed << "RANDOMSEED	" << l1 << " " << l2 << " " << l3 << " " << l4 << endl;;
   } else cerr << "PROBLEM: Unable to open random.out" << endl;
  WriteSeed.close();
  return;
}

double Random :: Gauss(double mean, double sigma) {
   // This function generates a random number from a Gaussian distribution with given mean and sigma
   double s=Rannyu();
   double t=Rannyu();
   double x=sqrt(-2.*log(1.-s))*cos(2.*M_PI*t);
   return mean + x * sigma;
}

double Random :: Exponential(double lambda) {
   // This function generates a random number from a Exponential distribution with given exponent lambda
   double y=Rannyu();
   return -1./lambda * log(1.-y);
}

double Random :: Lorentzian(double Gamma, double mu) {
   // This function generates a random number from a Lorentzian distribution with given parameter Gamma and mu
   double y=Rannyu();
   return mu + Gamma*tan(M_PI*(y-0.5));
}

double Random :: Es2_1() {
   // This function generates a random number from a 1 - sqrt(1 - y) distribution
   double y=Rannyu();
   return 1. - sqrt(1. - y);
}

double Random :: Random_angle(){
   double x, y, r;

   // 1. Ciclo di campionamento (Metodo del rigetto)
   do {
      x = Rannyu(-1., 1.); 
      y = Rannyu(-1., 1.); 
      r = sqrt(x*x + y*y); 
   } while(r >= 1.0 || r == 0.0); // RIPETI se sei fuori dal cerchio o nell'origine (r=0)

   // 2. Calcolo l'angolo SOLO per il punto che è stato accettato
   double theta;
   if (y >= 0) {
      theta = acos(x / r);
   } else {
      theta = 2 * M_PI - acos(x / r);
   } 

   return theta; 
} 


Posizione Random :: RandomWalk_lattice(Posizione &pos, double a){    // a è il passo del reticolo
   // P(x-->y) = 1/(2d)
   //estraggo un numero intero tra e 2d e associo a ogni numero una traslazione 
   /* n = estrazione intera nell'intervallo [0, 6)
      a = passo del reticolo
   
   Estrazione (n) | Direzione | Spostamento (dx, dy, dz)
   -----------------------------------------------------
        0         |    +x     |    (+a,  0,  0)
        1         |    -x     |    (-a,  0,  0)
        2         |    +y     |    ( 0, +a,  0)
        3         |    -y     |    ( 0, -a,  0)
        4         |    +z     |    ( 0,  0, +a)
        5         |    -z     |    ( 0,  0, -a)
   -----------------------------------------------------
*/

   int r = int(Rannyu(0, 6)); // prendo sei perchè 5.99584 --> 5; 4.2525 --> 4; 3.123423 --> 3 ...

   if(r == 0){pos.sposta(a, 0., 0.);}
   else if(r == 1){pos.sposta(-a, 0., 0.);}
   else if(r == 2){pos.sposta(0, a, 0.);}
   else if(r == 3){pos.sposta(0, -a, 0.);}
   else if(r == 4){pos.sposta(0, 0., a);}
   else if(r == 5){pos.sposta(0, 0., -a);}

   return pos;
}


Posizione Random :: RandomWalk_continuum(Posizione &pos, double a) {
   // 1. Campionamento uniforme sull'angolo solido
   double phi = Rannyu(0., 2.*M_PI);
   double costheta = Rannyu(-1., 1.); // Estrarre il coseno garantisce l'uniformità
   double sintheta = sqrt(1. - costheta*costheta); // Relazione fondamentale

   // 2. Calcolo degli spostamenti
   double dx = a * sintheta * cos(phi);
   double dy = a * sintheta * sin(phi);
   double dz = a * costheta;

   // 3. Aggiornamento posizione
   pos.sposta(dx, dy, dz);

   return pos;
}


double Random :: Rannyu(double min, double max){
   // This function generates a random number in the range [min, max)
   return min+(max-min)*Rannyu();
}


double Random :: Rannyu(void){
  // This function generates a random number in the range [0,1)
  const double twom12=0.000244140625;
  int i1,i2,i3,i4;
  double r;

  i1 = l1*m4 + l2*m3 + l3*m2 + l4*m1 + n1;
  i2 = l2*m4 + l3*m3 + l4*m2 + n2;
  i3 = l3*m4 + l4*m3 + n3;
  i4 = l4*m4 + n4;
  l4 = i4%4096;
  i3 = i3 + i4/4096;
  l3 = i3%4096;
  i2 = i2 + i3/4096;
  l2 = i2%4096;
  l1 = (i1 + i2/4096)%4096;
  r=twom12*(l1+twom12*(l2+twom12*(l3+twom12*(l4))));

  return r;
}

void Random :: SetRandom(int * s, int p1, int p2){
  // This function sets the seed and parameters of the random number generator
  m1 = 502;
  m2 = 1521;
  m3 = 4071;
  m4 = 2107;
  l1 = s[0];
  l2 = s[1];
  l3 = s[2];
  l4 = s[3];
  n1 = 0;
  n2 = 0;
  n3 = p1;
  n4 = p2;

  return;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

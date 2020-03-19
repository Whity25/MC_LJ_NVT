#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

using std::cout;
using std::endl;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using Clock = std::chrono::high_resolution_clock;
//set parameters
#define NUMBER_OF_ATOMS 5000
const static int NUMBER_OF_CONFIGURATIONS = 15;
const static double box_length = 500;
const static double sigma = 1.; //distance at which inter-particle potential is zero
const static double epsilon = 1.; //well depth



struct Atom {
  double x,y,z;
};

//distance between 2 atoms
double distance (const Atom& atom1, const Atom& atom2)
{
  return sqrt(pow((atom1.x - atom2.x),2) + pow((atom1.y - atom2.y),2) + pow((atom1.z * atom2.z),2));
}

//Lennard jones potential energy between 2 atoms
double LJ_energy (const Atom& atom1, const Atom& atom2)
{
  return 4*epsilon*(pow((sigma/distance(atom1,atom2)),12) - pow((sigma/distance(atom1,atom2)),6));
}

//total energy between a list of atoms
double total_energy(Atom (&atoms) [NUMBER_OF_ATOMS])
{
  double energy = 0;
  for(int i=0; i < NUMBER_OF_ATOMS; i++){
    for(int j = 0; j < NUMBER_OF_ATOMS; j++){
      if(i != j){
        energy += (LJ_energy(atoms[i], atoms[j]))/2;
      }
    }
  }
  return energy;
}





int main(){

  // initialize RNG
  std::uniform_real_distribution<double> dist(0., box_length);
  std::mt19937 rng;
  rng.seed(446464); //set RNG seed

  // initialize atoms in box
  auto start = Clock::now();
  Atom atoms [NUMBER_OF_ATOMS];
  for (int i=0; i < NUMBER_OF_ATOMS; i++){
    atoms[i].x = dist(rng);
    atoms[i].y = dist(rng);
    atoms[i].z = dist(rng);
  }
  auto end = Clock::now();
  auto duration = std::chrono::duration_cast<microseconds>(end -start);
  cout << "initializing atoms took " << duration.count() << "microseconds" << endl;


  start = Clock::now();
  double test_total_energy = total_energy(atoms);
  end = Clock::now();
  auto duration_ms = std::chrono::duration_cast<milliseconds>(end-start);
  cout << "Calculation took " << duration_ms.count()<< "ms" << endl;
  cout << test_total_energy << endl;
  return 0;
}

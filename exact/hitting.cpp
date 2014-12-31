#include <ctime>
#include <string>
#include <sstream>

using namespace std;

#include "vectorFunctions.cpp"
//#include <vector>

// models a sum of binomial rv's with success prob p, so mu = p - q = 2*p - 1
// sum begins at 0. If sum hits lower_bound or upper_bound it is absorbed.
// otherwise we find the next prob vector at t+1 as P(x, t+1) = p*P(x-1,t) + q*P(x+1,t)
struct Hitting{

  double p, q;
  int lb, ub;
  int n, origin;
  vector<double> P;

  vector<double> lb_abs; // absorbed probablities that went beyond the bounds as a function of time
  vector<double> ub_abs;
  
  Hitting(double p, int lb, int ub) : p(p), q(1-p), lb(lb), ub(ub){
    n = ub - lb + 1;
    origin = -lb;
    P = vector<double>(n);
    P[origin] = 1;
  }

  void printCurrent(){
    for (int v = ub; v >= lb; v--)
      cout << "P(" << v << ") : " << P[v + origin] << endl;
  }
  
  void printLowerAbs(){
    for (double d : lb_abs)
      cout << d << endl;
  }

  void printUpperAbs(){
    for (double d : ub_abs)
      cout << d << endl;
  }

  // evolve probablities one step forward in time
  void step(){
    vector<double> temp = vector<double>(n);

    // absorptions
    lb_abs.push_back(q*P[0]);
    ub_abs.push_back(p*P[n-1]);

    // boundaries don't get a contibution from the absorbed probs
    temp[0] = q*P[1];
    temp[n-1] = p*P[n-2];

    // inner points 
    for (int i = 1; i < n - 1; i++)
      temp[i] = p*P[i-1] + q*P[i+1];

    P = temp;
  }

  void stepUntil(double tol, int maxIter = 1000000){

    int iter = 0;
    while (vSum(P) > tol && iter++ < maxIter)
      step();

  }
  
};

int main(int argc, char** argv){

  stringstream ss;

  double p;
  ss << argv[1];
  ss >> p;

  int lb;
  ss.str(""); ss.clear();
  ss << argv[2];
  ss >> lb;

  int ub;
  ss.str(""); ss.clear();
  ss << argv[3];
  ss >> ub;
  
  double tol;
  ss.str(""); ss.clear();  
  ss << argv[4];
  ss >> tol;

  Hitting h = Hitting(p, lb, ub);
  
  h.stepUntil(tol);

  h.printLowerAbs();
  
}

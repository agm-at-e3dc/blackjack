#ifndef PROBCPP
#define PROBCPP

#include <cmath>

// Since probabilities are probability density, when constructing we demand that the vector sum to approximately 1.
template <typename T>
struct Prob {

  vector<T> vals;
  vector<double> pdf; 
  vector<double> cdf; 
  int n;

  Prob(){}
  
  Prob(vector<T> vals, vector<double> pdf) : vals(vals), pdf(pdf){
    n = (int) pdf.size();
    //checkSum();
    makeCDF();
  }

  // void checkSum(double CHECKTOL = 1e-10){
  //   double tot = 0;
  //   for (double d : pdf)
  //     tot += d;
  //   if (abs(tot - 1) > CHECKTOL){
  //     cout << "pdf failed to sum to 1, was: " << tot << endl;
  //     exit(0);
  //   }
  // }

  void makeCDF(){
    cdf = vector<double>();
    cdf.push_back(pdf[0]);
    for (int i=1; i<n; i++)
      cdf.push_back(cdf[i-1] + pdf[i]);
  }

  int random(){
    double r = double(rand())/double(RAND_MAX);
    for (int i=0; i<n-1; i++)
      if (r < cdf[i])
    	return vals[i];
    return vals[n-1];
  }

  // adds, blindly assumes vals are same
  Prob operator+(const Prob& P) const{
    vector<double> newPdf = pdf;
    for (int i=0; i<n; i++)
      newPdf[i] += P.pdf[i];
    return Prob(vals, newPdf);
  }

  // subtracts, blindly assumes vals are same
  Prob operator-(const Prob& P) const{
    vector<double> newPdf = pdf;
    for (int i=0; i<n; i++)
      newPdf[i] -= P.pdf[i];
    return Prob(vals, newPdf);
  }
  
  // multiplies probs by scalar
  Prob operator*(double d) const{
    vector<double> newPdf = pdf;
    for (int i=0; i<n; i++)
      newPdf[i] *= d;
    return Prob(vals, newPdf);
  }

  // multiplies probs by scalar
  Prob operator/(double d) const{
    vector<double> newPdf = pdf;
    for (int i=0; i<n; i++)
      newPdf[i] /= d;
    return Prob(vals, newPdf);
  }

  void operator+=(const Prob& P){
    for (int i=0; i<n; i++)
      pdf[i] += P.pdf[i];
  }

  void operator*=(double d){
    for (int i=0; i<n; i++)
      pdf[i] *= d;
  }

};

template <typename T>
ostream& operator<<(ostream& out, const Prob<T>& P){
  // for (int i=0; i<P.n-1; i++)
  //   out << "value : " << P.vals[i] << " | probability : " << P.pdf[i] << endl;
  // out << "value : " << P.vals[P.n-1] << " | probability : " << P.pdf[P.n-1];
  // return out;

  out << P.n << endl; // so we know how many to read
  out << P.vals << endl;
  out << P.pdf;
  return out;
}

template <typename T>
istream& operator>>(istream& in, Prob<T>& P){
  in >> P.n;
  P.vals = vector<T>(P.n);
  for (int i=0; i<P.n; i++)
    in >> P.vals[i];
  P.pdf = vector<double>(P.n);
  for (int i=0; i<P.n; i++)
    in >> P.pdf[i];
  P.makeCDF();
  return in;
}

template <typename T>
Prob<T> operator*(double d, const Prob<T>& P){
  return P*d;
}

#endif


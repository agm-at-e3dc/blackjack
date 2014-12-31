#ifndef VECTORFUNCTIONSCPP
#define VECTORFUNCTIONSCPP

#ifdef DEBUG
#include <typeinfo>
#endif

#include <vector>
#include <typeinfo>
#include <iostream>

using namespace std;

#include <cmath>

#define REAL double
#define VREAL vector<double>

/////////////////////////////// Binary Vector operators ////////////////////////////////

template <typename T, typename U>
vector<decltype(T() * U())> operator*(const vector<T>& A, const vector<U>& B){
  #ifdef DEBUG
  uint n = A.size();
  if (n  !=  B.size()){
    ostringstream oss (ostringstream::out);
    oss << "Size mismatch in operator*(" << typeid(A).name() << "," <<  typeid(B).name()
	<< ", sizes: (" << (int) A.size() << ',' << (int) B.size() << "), crashing!";
    throw(oss.str());
    }
  #endif
  vector<decltype(T() * U())> C;
  C.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  typename vector<U>::const_iterator b = B.begin();
  while (a != A.end()){
    C.push_back((*a) * (*b));
    ++a; ++b;
  }
  return C;
}

template <typename T, typename U>
vector<decltype(T() + U())> operator+(const vector<T>& A, const vector<U>& B){
  #ifdef DEBUG
  uint n = A.size();
  if (n  !=  B.size()){
    ostringstream oss (ostringstream::out);
    oss << "Size mismatch in operator+(" << typeid(A).name() << "," <<  typeid(B).name()
	<< ", sizes: (" << (int) A.size() << ',' << (int) B.size() << "), crashing!";
    throw(oss.str());
    }
  #endif
  vector<decltype(T() + U())> C;
  C.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  typename vector<U>::const_iterator b = B.begin();
  while (a != A.end()){
    C.push_back((*a) + (*b));
    ++a; ++b;
  }
  return C;
}

template <typename T, typename U>
vector<decltype(T() - U())> operator-(const vector<T>& A, const vector<U>& B){
  #ifdef DEBUG
  uint n = A.size();
  if (n  !=  B.size()){
    ostringstream oss (ostringstream::out);
    oss << "Size mismatch in operator-(" << typeid(A).name() << "," <<  typeid(B).name()
	<< ", sizes: (" << (int) A.size() << ',' << (int) B.size() << "), crashing!";
    throw(oss.str());
    }
  #endif
  vector<decltype(T() - U())> C;
  C.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  typename vector<U>::const_iterator b = B.begin();
  while (a != A.end()){
    C.push_back((*a) - (*b));
    ++a; ++b;
  }
  return C;
}

template <typename T, typename U>
vector<decltype(T() / U())> operator/(const vector<T>& A, const vector<U>& B){
  #ifdef DEBUG
  uint n = A.size();
  if (n  !=  B.size()){
    ostringstream oss (ostringstream::out);
    oss << "Size mismatch in operator/(" << typeid(A).name() << "," <<  typeid(B).name()
	<< ", sizes: (" << (int) A.size() << ',' << (int) B.size() << "), crashing!";
    throw(oss.str());
    }
  #endif
  vector<decltype(T() / U())> C;
  C.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  typename vector<U>::const_iterator b = B.begin();
  while (a != A.end()){
    C.push_back((*a) / (*b));
    ++a; ++b;
  }
  return C;
}

template <typename T, typename U>
vector<decltype(T() % U())> operator%(const vector<T>& A, const vector<U>& B){
  #ifdef DEBUG
  uint n = A.size();
  if (n  !=  B.size()){
    ostringstream oss (ostringstream::out);
    oss << "Size mismatch in operator%(" << typeid(A).name() << "," <<  typeid(B).name()
	<< ", sizes: (" << (int) A.size() << ',' << (int) B.size() << "), crashing!";
    throw(oss.str());
    }
  #endif
  vector<decltype(T() % U())> C;
  C.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  typename vector<U>::const_iterator b = B.begin();
  while (a != A.end()){
    C.push_back((*a) % (*b));
    ++a; ++b;
  }
  return C;
}

///////////////////// Vector-Object operators //////////////////////////////

template <typename T, typename U>
vector<decltype(T()*U())> operator+(const vector<T>& A, const U t){
  vector<decltype(T()*U())> B;
  B.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back((*a) + t);
    ++a;}
  return B;
}

template <typename T, typename U>
vector<decltype(T()*U())> operator-(const vector<T>& A, const U t){
  vector<decltype(T()*U())> B;
  B.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back((*a) - t);
    ++a;}
  return B;
}

template <typename T, typename U>
vector<decltype(T()*U())> operator*(const vector<T>& A, const U t){
  vector<decltype(T()*U())> B;
  B.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back((*a) * t);
    ++a;}
  return B;
}

template <typename T, typename U>
vector<decltype(T()/U())> operator/(const vector<T>& A, const U t){
  vector<decltype(T()/U())> B;
  B.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back((*a) / t);
    ++a;}
  return B;
}

template <typename T, typename U>
void operator+=(vector<T>& A, const U t){
  typename vector<T>::iterator a = A.begin();
  while (a != A.end()){
    *a += t;
    ++a;
  }
}

template <typename T, typename U>
void operator-=(vector<T>& A, const U t){
  typename vector<T>::iterator a = A.begin();
  while (a != A.end()){
    *a -= t;
    ++a;
  }
}

template <typename T, typename U>
void operator*=(vector<T>& A, const U t){
  typename vector<T>::iterator a = A.begin();
  while (a != A.end()){
    *a *= t;
    ++a;
  }
}

template <typename T, typename U>
void operator/=(vector<T>& A, const U t){
  typename vector<T>::iterator a = A.begin();
  while (a != A.end()){
    *a /= t;
    ++a;
  }
}

///////////////////// Object-Vector operators ///////////////////////////////


template <typename T, typename U>
vector<decltype(T()*U())> operator*(const T& t, const vector<U>& A){
  vector<decltype(T()*U())> B;
  B.reserve(A.size());
  typename vector<U>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back(t * (*a));
    ++a;}
  return B;
}

template <typename T, typename U>
vector<decltype(T()/U())> operator/(const T& t, const vector<U>& A){
  vector<decltype(T()/U())> B;
  B.reserve(A.size());
  typename vector<U>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back(t / (*a));
    ++a;}
  return B;
}


///////////////////// Vector-Scalar operators ///////////////////////////////

template <typename T>
vector<T> operator*(const vector<T>& A, REAL r){
  vector<T> B;
  B.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back((*a)*r);
    ++a;}
  return B;
}

template <typename T>
vector<T> operator*(REAL r, const vector<T>& A){
  vector<T> B;
  B.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back(r*(*a));
    ++a;}
  return B;
}

template <typename T>
vector<T> operator/(const vector<T>& A, REAL r){
  vector<T> B;
  B.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back((*a)/r);
    ++a;}
  return B;
}

///////////////////// Unary Vector operators //////////////////////////////

template <typename T>
vector<T> operator-(const vector<T>& A){
  vector<T> B;
  B.reserve(A.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    B.push_back(-(*a));
    ++a;}
  return B;
}
  
////////////////// In-Place operators ////////////////////////////////////

template <typename T>
void operator+=(vector<T>& A, const vector<T>& B){
  typename vector<T>::iterator a = A.begin();
  typename vector<T>::const_iterator b = B.begin();
  while (a != A.end()){
    (*a) += (*b);
    ++a; ++b;}
}

template <typename T>
void operator-=(vector<T>& A, const vector<T>& B){
  typename vector<T>::iterator a = A.begin();
  typename vector<T>::const_iterator b = B.begin();
  while (a != A.end()){
    (*a) -= (*b);
    ++a; ++b;}
}

template <typename T>
void operator*=(vector<T>& A, const vector<T>& B){
  typename vector<T>::iterator a = A.begin();
  typename vector<T>::const_iterator b = B.begin();
  while (a != A.end()){
    (*a) *= (*b);
    ++a; ++b;}
}

template <typename T>
void operator/=(vector<T>& A, const vector<T>& B){
  typename vector<T>::iterator a = A.begin();
  typename vector<T>::const_iterator b = B.begin();
  while (a != A.end()){
    (*a) /= (*b);
    ++a; ++b;}
}

template <typename T>
void operator*=(vector<T>& A, REAL r){
  typename vector<T>::iterator a = A.begin();
  while (a != A.end()){
    (*a) *= r;
    ++a;}
}

template <typename T>
void operator/=(vector<T>& A, REAL r){
  typename vector<T>::iterator a = A.begin();
  while (a != A.end()){
    (*a) /= r;
    ++a;}
}

///////////////////////  Input-Output ///////////////////////////////////

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& A){
  typename vector<T>::const_iterator a = A.begin();
  if (A.size() > 0){
    while(a != A.end()-1){
      out << *a << endl;
      ++a;
    }
    out << *a;
  }
  return out;
}

////////////////////// VM3 calls //////////////////////////


// VM3 vT(const VM3& A){
//   VM3 B;
//   B.reserve(A.size());
//   typename VM3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).T());
//     ++a;}
//   return B;
// }

// VM3 vI(const VM3& A){
//   VM3 B;
//   B.reserve(A.size());
//   typename VM3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).I());
//     ++a;}
//   return B;
// }

// VREAL vTr(const VM3& A){
//   VREAL B;
//   B.reserve(A.size());
//   typename VM3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).tr());
//     ++a;}
//   return B;
// }

// VREAL vDet(const VM3& A){
//   VREAL B;
//   B.reserve(A.size());
//   typename VM3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).det());
//     ++a;}
//   return B;
// }

// VV3 vLog(const VM3& A){
//   VV3 B;
//   B.reserve(A.size());
//   typename VM3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).log());
//     ++a;
//   }
//   return B;
// }

// VREAL vF(const VM3& A){
//   VREAL B;
//   B.reserve(A.size());
//   typename VM3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).F());
//     ++a;}
//   return B;
// }

// VV3 vS(const VM3& A){
//   VV3 B;
//   B.reserve(A.size());
//   typename VM3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).S());
//     ++a;
//   }
//   return B;
// }

// VM3 vExp(const VV3& A, REAL tol = SMALLNUM){
//   VM3 B;
//   B.reserve(A.size());
//   typename VV3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back(exp(*a));
//     ++a;
//   }
//   return B;
// }

// VM3 vRandMatrix3(int n){
//   VM3 A(n);
//   generate(A.begin(), A.end(), randMatrix3);
//   return A;
// }

// VBOOL vIsOrthogonal(const VM3& A, REAL tol = SMALLNUM){
//   VBOOL B;
//   B.reserve(A.size());
//   typename VM3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).isOrthogonal());
//     ++a;
//   }
//   return B;
// }

// VM3 vRotDiff(const VM3& A){
//   uint n = A.size();
//   VM3 B;
//   B.reserve(n-1);
//   for (uint i=1; i<n; i++)
//     B.push_back(A[i]*(A[i-1].T()));
//   return B;
// }

//////////////////////////// VV3 calls //////////////////////////

// VV3 vUnitize(const VV3& A){
//   VV3 B;
//   B.reserve(A.size());
//   typename VV3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).unitize());
//     ++a;
//   }
//   return B;
// }

// VREAL vLength(const VV3& A){
//   VREAL B;
//   B.reserve(A.size());
//   typename VV3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).length());
//     ++a;
//   }
//   return B;
// }

// VBOOL vIsUnit(const VV3& A, REAL tol = SMALLNUM){
//   VBOOL B;
//   B.reserve(A.size());
//   typename VV3::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back((*a).isUnit());
//     ++a;
//   }
//   return B;
// }

// // VV3 vRandVector3(int n, REAL mu, REAL sigma){
// //   VV3 A(n);
// //   for (int i=0; i<n; i++)
// //     A[i] = randVector3(mu, sigma);
// //   return A;
// // }

// // VV3 vRandVector3(int n){
// //   return vRandVector3(n, 0, 1);
// //   //VV3 A(n);
// //   //generate(A.begin(), A.end(), randVector3);
// //   //return A;
// // }

// VV3 vRandXOnly(int n){
//   VV3 A(n);
//   generate(A.begin(), A.end(), randXOnly);
//   return A;
// }


 
// /////////////////////// REAL calls /////////////////////////////////

// VREAL vSqrt(const VREAL& A){
//   VREAL B;
//   B.reserve(A.size());
//   typename VREAL::const_iterator a = A.begin();
//   while (a != A.end()){
//     B.push_back(sqrt(*a));
//     ++a;
//   }
//   return B;
// }

/////////////////////// discrete operators /////////////////////////

template <class T>
vector<T> vDiff(const vector<T>& A){
  uint n = A.size();
  vector<T> B;
  B.reserve(n-1);
  for (uint i=1; i<n; i++)
    B.push_back(A[i] - A[i-1]);
  return B;
}

template <class T>
vector<T> vDelta(const vector<T>& A){
  uint n = A.size();
  vector<T> B;
  B.reserve(n+1);
  B.push_back(A[0]);
  for (uint i=1; i<n; i++)
    B.push_back(A[i] - A[i-1]);
  B.push_back(-A[n-1]);
  return B;
}

template <class T>
vector<T> vMidAvg(const vector<T>& A){
  vector<T> B;
  uint n = A.size();
  B.reserve(n+1);
  B.push_back(A[0]/2);
  for (uint i=1; i<n; i++)
    B.push_back((A[i]+A[i-1])/2);
  B.push_back(A[n-1]/2);
  return B;
}

template <class T>
vector<T> vMidAvgInterior(const vector<T>& A){
  vector<T> B;
  uint n = A.size();
  B.reserve(n-1);
  for (uint i=1; i<n; i++)
    B.push_back((A[i]+A[i-1])/2);
  return B;
}

template <class T>
T vSum(const vector<T>& A){
  typename vector<T>::const_iterator a = A.begin();
  T b = *a;
  ++a;
  while(a != A.end()){
    b += *a;
    ++a; 
  }
  return b;
}

//  returns the sums of A[start] through A[end-1]
template <class T>
T vPartialSum(const vector<T>& A, int start, int end){
  T result = T();
  for (int i=start; i<end; i++)
    result += A[i];
  return result;
}

// returns the cumulative sum vector of a vector
template <class T>
vector<T> vCumSum(const vector<T>& A){
  vector<T> B;
  uint n = A.size();
  B.reserve(n+1);
  B.push_back(A[0]);
  for (uint i=1; i<n; i++)
    B.push_back(A[i]+B[i-1]);
  return B;
}

template <class T>
T vMean(const vector<T>& A){
  return vSum(A)/A.size();
}

template <class T>
T vMax(const vector<T>& A){
  typename vector<T>::const_iterator a = A.begin();
  T b = *a;
  ++a;
  while(a != A.end()){
    if (*a > b)
      b = *a;
    ++a;
  }
  return b;
}

template <class T>
T vMin(const vector<T>& A){
  typename vector<T>::const_iterator a = A.begin();
  T b = *a;
  ++a;
  while(a != A.end()){
    if (*a < b)
      b = *a;
    ++a;
  }
  return b;
}


VREAL vAbs(const VREAL& A){
  VREAL B;
  B.reserve(A.size());
  typename VREAL::const_iterator a = A.begin();
  while(a != A.end()){
    B.push_back(fabs(*a));
    ++a;
  }
  return B;
}
  

///////////////////// discrete generators ////////////////////////////

template <class T>
vector<T> vRange(T start, T end, int steps){
  vector<T> A;
  A.reserve(steps);
  if (steps>1){
    T stepSize = (end-start)/(steps-1);
    for (int i=0; i<steps; i++)
      A.push_back(start + stepSize*i);
    }
  else if (steps == 1)
    A.push_back(start);
  else
    throw("vRange must be called with int steps > 0");
  return A;
}

//////////////////// recombination operators ////////////////////////////

template <class T>
vector<T> vCat(const vector<T>& A, const vector<T>& B){
  vector<T> C;
  C.reserve(A.size() + B.size());
  typename vector<T>::const_iterator a = A.begin();
  while (a != A.end()){
    C.push_back(*a);
    a++;
  }
  typename vector<T>::const_iterator b = B.begin();
  while (b != B.end()){
    C.push_back(*b);
    b++;
  }
  return C;
}
 
#endif


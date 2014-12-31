#ifndef VECTORFUNCTIONSCPP
#define VECTORFUNCTIONSCPP

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

template <class T>
T vMean(const vector<T>& A){
  if (A.size() == 0)
    return 0;
  else
    return vSum(A)/A.size();
}

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

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& A){
  typename vector<T>::const_iterator a = A.begin();
  while(a != A.end()-1){
    out << *a << endl;
    ++a;
  }
  out << *a;
  return out;
}

#endif

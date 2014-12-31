#ifndef LOOKUPCPP
#define LOOKUPCPP

#include <vector>

// a simple linear lookup returning the first value whose corresponding key matches
template <typename K, typename V>
struct Lookup{

  vector<K> keys;
  vector<V> vals;
  int n;

  Lookup(){
    keys = vector<K>();
    vals = vector<V>();
    n = 0;
  }

  void add(K key, V val){
    keys.push_back(key);
    vals.push_back(val);
    n++;
  }

  V get(K key){
    for (int i=0; i<(int)keys.size(); i++)
      if (keys[i] == key)
	return vals[i];
    throw "couldn't find key, dying";
  }
  
};

template <typename K, typename V>
ostream& operator<<(ostream& out, const Lookup<K, V>& L){
  out << L.n << endl;
  out << L.keys << endl;
  out << L.vals;
  return out;
}

template <typename K, typename V>
istream& operator>>(istream& in, Lookup<K, V>& L){
  in >> L.n;
  L.keys = vector<K>(L.n);
  for (int i=0; i<L.n; i++)
    in >> L.keys[i];
  L.vals = vector<V>(L.n);
  for (int i=0; i<L.n; i++)
    in >> L.vals[i];
  return in;
}


#endif

// represents the state of a hand. A busted hand is represented by having value 22.
// soft above 10 makes no sense: soft means we add to the base value when ultimately comparing two hands
struct HandState{

  int value;
  bool soft; // if hand has an ace
  bool pair; // if hand is a pair of same-valued cards (and nothing else)
  bool bj;   // if hard is an ace and a 10-value (and nothing else), AND we DID NOT split

  HandState() : value(0), soft(false), pair(false), bj(false) {}

  HandState(int value, bool soft, bool pair, bool bj) : value(value), soft(soft), pair(pair), bj(bj) {}

  bool operator==(const HandState& hs) const{
    return (value == hs.value && soft == hs.soft && pair == hs.pair && bj == hs.bj);
  }

};

ostream& operator<<(ostream& out, const HandState& hs){
  // if (hs.pair)
  //   out << hs.value << "P";
  // else if (hs.soft)
  //   out << hs.value << "S";
  // else
  //   out << hs.value << "H";
  // return out;

  out << hs.value << endl;
  out << hs.soft << endl;
  out << hs.pair << endl;
  out << hs.bj;
  return out;
}

istream& operator>>(istream& in, HandState& hs){
  in >> hs.value;
  in >> hs.soft;
  in >> hs.pair;
  in >> hs.bj;
  return in;
}

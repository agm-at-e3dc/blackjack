// represents the state of a hand.
// in switch the case 22 is a special busted hand, as dealers push if end with 22 and player doesn't natural or bust
// hence here we represent 23 as the value representing all values >= 23
// soft above 10 makes no sense: soft means we add to the base value when ultimately comparing two hands
struct HandState{

  int value;
  bool soft; // if hand has an ace
  bool pair; // if hand is a pair of same-valued cards (and nothing else)

  HandState() : value(0), soft(false), pair(false){}

  HandState(int value, bool soft, bool pair) : value(value), soft(soft), pair(pair){}

  bool operator==(const HandState& hs) const{
    return (value == hs.value && soft == hs.soft && pair == hs.pair);
  }

};

ostream& operator<<(ostream& out, const HandState& hs){
  if (hs.pair)
    out << hs.value << "P";
  else if (hs.soft)
    out << hs.value << "S";
  else
    out << hs.value << "H";
  return out;
  // out << hs.value << endl;
  // out << hs.soft << endl;
  // out << hs.pair << endl;
  // return out;
}

istream& operator>>(istream& in, HandState& hs){
  in >> hs.value;
  in >> hs.soft;
  in >> hs.pair;
  return in;
}

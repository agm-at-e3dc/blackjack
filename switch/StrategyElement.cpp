#ifndef STRATEGYELEMENT
#define STRATEGYELEMENT

// Represents a single element in the Strategy matrix, saying what action to do and what the expected value is. 
struct StrategyElement{

  vector<double> values; // values corresponding to STAND, HIT, DOUBLE, SPLIT respectively. -999 if not applicable. 
  GameAction bestAction;
  double bestValue;

  StrategyElement(){}
  
  // choose best action hackily
  StrategyElement(vector<double> values) : values(values) {
    setBest();
  }

  void setBest(){
    if (values[0] >= values[1] && values[0] >= values[2] && values[0] >= values[3]){
      bestValue = values[0];
      bestAction = GameAction::STAND;
    }
    else if (values[1] >= values[0] && values[1] >= values[2] && values[1] >= values[3]){
      bestValue = values[1];
      bestAction = GameAction::HIT;
    }
    else if (values[2] >= values[0] && values[2] >= values[1] && values[2] >= values[3]){
      bestValue = values[2];
      bestAction = GameAction::DOUBLE;
    }
    else{
      bestValue = values[3];
      bestAction = GameAction::SPLIT;
    }
  }

};

ostream& operator<<(ostream& out, const StrategyElement& se){
  // cout << GameAction::STAND  << " " << se.values[0] << endl;
  // cout << GameAction::HIT    << " " << se.values[1] << endl;
  // cout << GameAction::DOUBLE << " " << se.values[2] << endl;
  // cout << GameAction::SPLIT  << " " << se.values[3] << endl;
  // cout << "Best : " << se.bestAction << " " << se.bestValue;
  // return out;

  out << se.values;
  return out;
}

istream& operator>>(istream& in, StrategyElement& se){
  se.values = vector<double>(4);
  in >> se.values[0];
  in >> se.values[1];
  in >> se.values[2];
  in >> se.values[3];
  se.setBest();
  return in;
}

#endif

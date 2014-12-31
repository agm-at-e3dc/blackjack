#ifndef GAMEACTIONCPP
#define GAMEACTIONCPP

enum struct GameAction{
  HIT, STAND, DOUBLE, SPLIT, SURRENDER, NA
};

ostream& operator<<(ostream& out, GameAction a){
  switch (a){
  case GameAction::HIT :
    out << "HIT";
    break;
  case GameAction::STAND :
    out << "STAND";
    break;
  case GameAction::DOUBLE :
    out << "DOUBLE";
    break;
  case GameAction::SPLIT :
    out << "SPLIT";
    break;
  case GameAction::SURRENDER :
    out << "SURRENDER";
    break;
  case GameAction::NA :
    out << "N/A";
    break;
  default :
    out << "FAIL!";
    break;
  }
  return out;
}

#endif

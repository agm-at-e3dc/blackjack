#ifndef GAMEACTIONCPP
#define GAMEACTIONCPP

enum struct GameAction{
  UNKNOWN, DONE, NA, HIT, STAND, DOUBLE, SPLIT
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
  case GameAction::DONE :
    out << "DONE";
    break;
  case GameAction::NA :
    out << "N/A";
    break;
  case GameAction::UNKNOWN :
    out << "UNKNOWN";
    break;
  default :
    out << "FAIL!";
    break;
  }
  return out;
}

#endif

#ifndef PLAYERCPP
#define PLAYERCPP

#include "Hand.cpp"
#include "GameAction.cpp"

// player is the general base class for both dealers and all types of strategies
struct Player{

  int count;
  int seenCards;
  double totalWager;
  int numDecks;

  Player() : count(0), seenCards(0), totalWager(0), numDecks(1) {}
  
  virtual void shuffling() {}

  virtual void endHand(Hand& myHand, Hand& opponentHand) {}

  virtual double chooseWager() { return 1;}
  
  virtual GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, bool canDouble, bool canSurrender) = 0;

  virtual bool takeInsurance() { return false;}

  virtual int getRealCount(){
    return (int) (count * 52./ (52 * numDecks - seenCards));
  }
  
};

struct Dealer : Player {

  Dealer() : Player() {}
  
  // dealer will hit soft 17, stand on hard 17+
  GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, bool canDouble, bool canSurrender){
    int myValue = myHand.value();
    if (myHand.hasAce()){
      if ((myValue + 10 > 17 && myValue + 10 <= 21) || myValue >= 17)
	  return GameAction::STAND;
      return GameAction::HIT;
    }
    else{
      if (myValue < 17)
	return GameAction::HIT;
      else
	return GameAction::STAND;
    }
  }
  
};

// if our prescribed action is legal, we do it, otherwise act like dealer
struct MatrixPlayer : Player {

  HandMatrix<GameAction> actionMatrix;
  HandMatrix<GameAction> highCountMatrix;

  MatrixPlayer(HandMatrix<GameAction>& actionMatrix, HandMatrix<GameAction>& highCountMatrix, int numDecks) :
    actionMatrix(actionMatrix), highCountMatrix(highCountMatrix) {
    this->numDecks = numDecks;
  }

  void shuffling(){
    count = 0;
    seenCards = 0;
  }

  void endHand(Hand& myHand, Hand& opponentHand) {
    for (Card& c : myHand.cards){
      seenCards++;
      if (c.value > 1 && c.value <= 6)
      	count++;
      if (c.value == 1 || c.value == 10)
      	count--;
    }
    for (Card& c : opponentHand.cards){
      seenCards++;
      if (c.value > 1 && c.value <= 6)
      	count++;
      if (c.value == 1 || c.value == 10)
      	count--;
    }    
  }

  bool takeInsurance(){
    if (getRealCount() >= 4)
      return true;
    return false;
  }

  double chooseWager(){
    if (getRealCount() >= 3){
      totalWager += 1;
      return 1;
    }
    else{
      totalWager += 1;
      return 1;
    }
  }
  
  GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, bool canDouble, bool canSurrender){

    HandState myState = HandState(myHand.value(), myHand.hasAce());
    int dealerUpCard = opponentHand.cards[0].value;

    GameAction bestAction;
    if (getRealCount() >= 3)
      bestAction = highCountMatrix.getData(myState, dealerUpCard);
    else
      bestAction = actionMatrix.getData(myState, dealerUpCard);

    //if (myHand.size() == 2)
    if (bestAction == GameAction::HIT ||
	bestAction == GameAction::STAND ||
	(bestAction == GameAction::SPLIT && canSplit) ||
	(bestAction == GameAction::DOUBLE && canDouble) ||
	(bestAction == GameAction::SURRENDER && canSurrender))
      return bestAction;

    int myValue = myHand.value();
    if (myHand.hasAce()){
      if ((myValue + 10 > 17 && myValue + 10 <= 21) || myValue >= 17)
	  return GameAction::STAND;
      return GameAction::HIT;
    }
    else{
      if (myValue < 17)
	return GameAction::HIT;
      else
	return GameAction::STAND;
    }
  }

};

// used for testing: if the chosen action can be done and there are only 2 cards in hand, do it, otherwise act as dealer
struct ActionPlayer : Player{

  GameAction myAction;

  ActionPlayer(GameAction myAction) : myAction(myAction) {}

  GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, bool canDouble, bool canSurrender){
    if (myHand.size() == 2 && 
	(myAction == GameAction::HIT ||
	 myAction == GameAction::STAND ||
	 (myAction == GameAction::SPLIT && canSplit) ||
	 (myAction == GameAction::DOUBLE && canDouble) ||
	 (myAction == GameAction::SURRENDER && canSurrender)))
      return myAction;

    int myValue = myHand.value();
    if (myHand.hasAce()){
      if ((myValue + 10 > 17 && myValue + 10 <= 21) || myValue >= 17)
	  return GameAction::STAND;
      return GameAction::HIT;
    }
    else{
      if (myValue < 17)
	return GameAction::HIT;
      else
	return GameAction::STAND;
    }

  }

};

// iterative testing stage: if the chosen action can be done, do it, otherwise do as the given matrix prescribes if possible,
// otherwise do as dealer
struct ActionMatrixPlayer : Player{

  GameAction myAction;
  HandMatrix<GameAction> actionMatrix;

  ActionMatrixPlayer(GameAction myAction, HandMatrix<GameAction>& actionMatrix) : myAction(myAction), actionMatrix(actionMatrix) {}

  GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, bool canDouble, bool canSurrender){
    if (myHand.size() == 2 &&
	(myAction == GameAction::HIT ||
	 myAction == GameAction::STAND ||
	 (myAction == GameAction::SPLIT && canSplit) ||
	 (myAction == GameAction::DOUBLE && canDouble) ||
	 (myAction == GameAction::SURRENDER && canSurrender)))
      return myAction;

    // HandState myState = HandState(myHand.value(), myHand.hasAce());
    // int dealerUpCard = opponentHand.cards[0].value;

    // GameAction bestAction;
    // bestAction = actionMatrix.getData(myState, dealerUpCard);

    // if (bestAction == GameAction::HIT ||
    // 	bestAction == GameAction::STAND)
    //   return bestAction;
    
    int myValue = myHand.value();
    if (myHand.hasAce()){
      if ((myValue + 10 > 17 && myValue + 10 <= 21) || myValue >= 17)
	  return GameAction::STAND;
      return GameAction::HIT;
    }
    else{
      if (myValue < 17)
	return GameAction::HIT;
      else
	return GameAction::STAND;
    }

  }
};


#endif

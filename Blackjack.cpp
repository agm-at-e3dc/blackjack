#include <iostream>
#include <stdio.h>
#include <fstream>

#include <cstdlib>
#include <time.h>

#include <random>
#include <vector>
#include <algorithm>

using namespace std;

static double BJRATIO = 1.5;
double dealerBusts = 0;
int numSplits = 0;
int numSurrenders = 0;

vector<double> totalWagers;

// results based on upcard of dealer
vector<double> twoResults;
vector<double> threeResults;
vector<double> fourResults;
vector<double> fiveResults;
vector<double> sixResults;
vector<double> sevenResults;
vector<double> eightResults;
vector<double> nineResults;
vector<double> tenResults;
vector<double> aceResults;

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

struct Card{

  char name;
  int value, optValue, count;
  
  Card() : name('n'), value(0), optValue(0) {}
  Card(char name, int value) : name(name), value(value), optValue(value), count(0) {}
  Card(char name, int value, int optValue) : name(name), value(value), optValue(optValue), count(0) {}
  Card(char name, int value, int optValue, int count) : name(name), value(value), optValue(optValue), count(count) {}
  
};

ostream& operator<<(ostream& out, Card& c){
  out << c.name;
  return out;
}

Card baseCards[] = {Card('2', 2, 2, +1),
		    Card('3', 3, 3, +1),
		    Card('4', 4, 4, +2),
		    Card('5', 5, 5, +2),
		    Card('6', 6, 6, +1),
		    Card('7', 7, 7, +1),
		    Card('8', 8, 8,  0),
		    Card('9', 9, 9,  0),
		    Card('T', 10, 10, -2),
		    Card('J', 10, 10, -2),
		    Card('Q', 10, 10, -2),
		    Card('K', 10, 10, -2),
		    Card('A', 1, 11, -0)};

struct Deck{

  Card* cards;

  int numCards;

  int curCardNum;

  int curCount;

  mt19937 gen;

  ~Deck(){
    //cout << "why is this causing a memory error?" << endl;
    delete[] cards;
  }

  // numDecks is number of normal decks
  Deck(int numDecks){
    numCards = 52 * numDecks;
    cards = new Card[numCards];
    for (int i=0; i<numDecks; i++){
      for (int j=0; j<13; j++){
	for (int k=0; k<4; k++){
	  cards[52*i + 4*j + k] = baseCards[j];
	}
      }
    }
	  
    random_device rd;
    gen = mt19937(rd());

    curCardNum = 0;
    curCount = 0;
  }

  // randomly permute the cards
  void shuffle(int start = 0){
    curCount = 0;
    curCardNum = start;
    for (int i=start; i<numCards; i++){
      int swapNum = i + (int)((generate_canonical<double, 32>(gen)) * (numCards-i));
      if (swapNum >= numCards){
	cout << "WTFUUU" << endl;
	swapNum = numCards - 1;
      }
      //int swapNum = i + (int) (((double) rand()/RAND_MAX) * (numCards-i));
      Card temp = cards[swapNum];
      cards[swapNum] = cards[i];
      cards[i] = temp;
    }
  }

  Card& nextCard(){
    curCount += cards[curCardNum].count;
    return cards[curCardNum++];
  }

  bool isEmpty(){
    return curCardNum >= numCards;
  }
  
  // search the deck starting at card start for a card w/value val and swap card at start for it
  void findAndSwap(int start, int val){
    for (int i=start; i<numCards; i++){
      if (cards[i].value == val){
	Card temp = cards[i];
	cards[i] = cards[start];
	cards[start] = temp;
	curCount += temp.count;
      }
    }
  }

  // searches through the decks and places the chosen cards in order in the first 3 spots, then shuffles the rest
  void setup(int pVal1, int pVal2, int dUpVal){
    findAndSwap(0, pVal1);
    findAndSwap(1, pVal2);
    findAndSwap(2, dUpVal);
    shuffle(3);
    curCardNum = 0;
  }

  void setup(vector<int> vals){
    for (uint i=0; i<vals.size(); i++)
      findAndSwap(i, vals[i]);
    shuffle(vals.size());
    curCardNum = 0;
  }
  
};

ostream& operator<<(ostream& out, Deck& d){
  for (int i=0; i<d.numCards; i++)
    out << d.cards[i] << endl;
  return out;
}

struct Hand{

  vector<Card> cards;

  Hand(){
    cards = vector<Card>();
  }

  void addCard(Card& c){
    cards.push_back(c);
  }

  int numCards(){
    return (int)cards.size();
  }

  vector<int> value_sub(int curNum = 0, int curVal = 0){
    for (int i=curNum; i<numCards(); i++){
      Card c = cards[i];
      if (c.value != c.optValue)
	return vCat(value_sub(i+1, curVal + c.value), value_sub(i+1, curVal + c.optValue));
      else
	curVal += c.value;
    }
    return vector<int> {curVal};
  }

  vector<int> value(){
    vector<int> values = value_sub();
    sort(values.begin(), values.end());
    vector<int> result;
    int count = 0;
    result.push_back(values[0]);
    for (uint i=1; i<values.size(); i++){
      if (values[i] != result[count]){
	result.push_back(values[i]);
	count++;
      }
    }
    return result;
  }

  int simpleValue(){
    int val = 0;
    for (Card& c : cards)
      val += c.value;
    return val;
  }

  bool containsAce(){
    for (Card& c : cards)
      if (c.name == 'A')
	return true;
    return false;
  }

  bool isBlackjack(){
    if (cards[0].name == 'A' && cards[1].value == 10)
      return true;
    if (cards[1].name == 'A' && cards[0].value == 10)
      return true;
    return false;
  }

  // returns the highest hand value <= 21
  // if all values are over 21, returns the first value
  int bestValue(){
    vector<int> values = value();
    int bestVal = values[0];
    for (uint i=0; i<values.size(); i++)
      if (values[i] <= 21 && values[i] > bestVal)
	bestVal = values[i];
    return bestVal;
  }

};

ostream& operator<<(ostream& out, Hand& h){
  for (int i=0; i<h.numCards(); i++)
    out << h.cards[i] << " ";
  return out;
}

enum struct GameAction{
  HIT, STAND, DOUBLE, SPLIT, SURRENDER
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
  default :
    out << "FAIL!";
    break;
  }
  return out;
}

// player is the general base class for both dealers and all types of strategies
struct Player{

  virtual double chooseWager(int count) { return 1;}
  
  virtual GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, int curCount) = 0;
};

struct Dealer : Player {

  Dealer() : Player() {}
  
  double chooseWager(int count){
    return 1;
  }

  // dealer will hit soft 17, stand on hard 17+
  GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, int curCount){
    vector<int> myValues = myHand.value();
    if (myHand.containsAce()){
      for (int& val : myValues)
	if (val > 17 && val <= 21)
	  return GameAction::STAND;
      return GameAction::HIT;
    }
    else{
      if (myValues[0] < 17)
	return GameAction::HIT;
      else
	return GameAction::STAND;
    }
  }
  
};

struct ActionPlayer : Player {

  GameAction myAction;

  ActionPlayer(GameAction myAction) : Player(), myAction(myAction) {}

  GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, int curCount){

    if (myHand.numCards() == 2 && canSplit)
      return myAction;

    // Card dealerUpCard = opponentHand.cards[0];
    vector<int> myValues = myHand.value();

    if (myHand.containsAce()){
      for (int& val : myValues)
	if (val > 17 && val <= 21)
	  return GameAction::STAND;
      return GameAction::HIT;
    }
    else{
      if (myValues[0] < 17)
	return GameAction::HIT;
      else
	return GameAction::STAND;
    }
  }
  
};

struct BasicStrategy : Player {

  BasicStrategy() : Player() {}
  
  double chooseWager(int count){
    return 1;
  }

  // dealer will hit soft 17, stand on hard 17+
  GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, int curCount){

    //Card dealerUpCard = opponentHand.cards[0];
    vector<int> myValues = myHand.value();
    
    if (myHand.containsAce()){
      for (int& val : myValues)
	if (val > 17 && val <= 21)
	  return GameAction::STAND;
      return GameAction::HIT;
    }
    else{
      if (myValues[0] < 17)
	return GameAction::HIT;
      else
	return GameAction::STAND;
    }
  }
  
};

struct BestActionPlayer : Player {
  
  GameAction* bestActions;
  GameAction* bestActions_highCount;

  BestActionPlayer(GameAction* bestActions, GameAction* bestAction_highCount) :
    bestActions(bestActions), bestActions_highCount(bestActions_highCount) {}

  double chooseWager(int count){
    if (count > 5){
      totalWagers.push_back(10);      
      return 10;
    }
    else{
      totalWagers.push_back(1);
      return 1;
    }
  }

  GameAction makeDecision(Hand& myHand, Hand& opponentHand, bool canSplit, int curCount){

    if (myHand.numCards() == 2){
      GameAction bestAction;
      if (curCount < 5)
	bestAction =
	  bestActions[(myHand.cards[0].value-1) * 100 + (myHand.cards[1].value-1) * 10 + opponentHand.cards[0].value - 1];
      else
	bestAction =
	  bestActions_highCount[(myHand.cards[0].value-1) * 100 + (myHand.cards[1].value-1) * 10 + opponentHand.cards[0].value - 1];
	
      if (bestAction == GameAction::SPLIT){
	if (canSplit)
	  return bestAction;
      }
      else
	return bestAction;
    }

    vector<int> myValues = myHand.value();
    
    if (myHand.containsAce()){
      for (int& val : myValues)
	if (val > 17 && val <= 21)
	  return GameAction::STAND;
      return GameAction::HIT;
    }
    else{
      if (myValues[0] < 17)
	return GameAction::HIT;
      else
	return GameAction::STAND;
    }
    
    return GameAction::HIT;
  }

};

struct Game{

  Deck& deck;
  Player* dealer;
  vector<Player*> players;
  
  int numPlayers;

  Game(Deck& deck, Player* dealer, vector<Player*> players) : deck(deck), dealer(dealer), players(players) {
    numPlayers = (int) players.size();
  }

  Hand dealHand(){
    Hand newHand;
    newHand.addCard(deck.nextCard());
    newHand.addCard(deck.nextCard());
    return newHand;
  }

  // have each player choose their wager, then play out the hand + return results
  vector<double> playHand(int count, bool record = true){
    vector<double> wagers;
    for (Player* p : players)
      wagers.push_back(p->chooseWager(count));

    vector<Hand> playerHands;
    for (int i=0; i<numPlayers; i++)
      playerHands.push_back(dealHand());
    Hand dealerHand = dealHand();

    vector<bool> playerBlackjacks;
    vector<bool> playerSurrenders = vector<bool>(false, numPlayers);
    for (int i=0; i<numPlayers; i++){
      bool bj = playerHands[i].isBlackjack();
      playerBlackjacks.push_back(bj);

      bool canSplit = true;
      int curSplits = 0;

      if (!bj){
	vector<int> values = playerHands[i].value();
	GameAction curAction = GameAction::HIT;
	while (curAction == GameAction::HIT && values[0] <= 21){
	  curAction = players[i]->makeDecision(playerHands[i], dealerHand, canSplit, deck.curCount);

	  if (curAction == GameAction::HIT)
	    playerHands[i].addCard(deck.nextCard());
	  if (curAction == GameAction::DOUBLE){
	    wagers[i]*=2;
	    playerHands[i].addCard(deck.nextCard());
	  }
	  // lets just ghetto approx this for the moment
	  if (curAction == GameAction::SPLIT){
	    wagers[i]*=2;
	    playerHands[i].cards[1] = deck.nextCard();
	    canSplit = false;
	    curAction = GameAction::HIT;
	    curSplits++;
	  }
	  if (curAction == GameAction::SURRENDER){
	    playerSurrenders[i] = true;
	  }
	  values = playerHands[i].value();
	}
      }
    }

    // cout << "Dealer hand : " << dealerHand << endl;
    // cout << "Player hand : " << playerHands[0] << endl;

    vector<double> payoffs;
    bool dealerBJ = dealerHand.isBlackjack();
    if (dealerBJ){
      for (int i=0; i<numPlayers; i++){
	if (playerBlackjacks[i])
	  payoffs.push_back(0);
	else
	  payoffs.push_back(-1*wagers[i]);
      }
    }
    else{

      bool dealerContinues = false;
      for (int i=0; i<numPlayers; i++){
	int bestPlayerVal = playerHands[i].bestValue();
	if (bestPlayerVal <= 21)
	  dealerContinues = true;
      }

      if (dealerContinues){
	vector<int> values = dealerHand.value();
	GameAction curAction = GameAction::HIT;
	while (curAction != GameAction::STAND && values[0] <= 21){
	  curAction = dealer->makeDecision(dealerHand, playerHands[0], true, deck.curCount);
	  if (curAction == GameAction::HIT)
	    dealerHand.addCard(deck.nextCard());
	  values = dealerHand.value();
	}
      }

      // cout << "Dealer hand : " << dealerHand << endl;
      // cout << "Player hand : " << playerHands[0] << endl;

      int bestDealerVal = dealerHand.bestValue();

      if (bestDealerVal > 21)
	dealerBusts++;
	
      for (int i=0; i<numPlayers; i++){
	if (playerBlackjacks[i])
	  payoffs.push_back(BJRATIO * wagers[i]);
	else if (playerSurrenders[i])
	  payoffs.push_back(-0.5*wagers[i]);
	else{
	  int bestPlayerVal = playerHands[i].bestValue();
	  // cout << "bestDealerVal : " << bestDealerVal << endl;
	  // cout << "bestPlayerVal : " << bestPlayerVal << endl;	  
	  if (bestPlayerVal > 21)
	    payoffs.push_back(-1 * wagers[i]);
	  else if (bestDealerVal > 21)
	    payoffs.push_back(wagers[i]);
	  else if (bestPlayerVal < bestDealerVal)
	    payoffs.push_back(-1 * wagers[i]);
	  else if (bestPlayerVal == bestDealerVal)
	    payoffs.push_back(0);
	  else
	    payoffs.push_back(1 * wagers[i]);
	}
      }
    }

    if (record){
      switch (dealerHand.cards[0].value){
      case 2: twoResults.push_back(payoffs[0]); break;
      case 3: threeResults.push_back(payoffs[0]); break;
      case 4: fourResults.push_back(payoffs[0]); break;
      case 5: fiveResults.push_back(payoffs[0]); break;
      case 6: sixResults.push_back(payoffs[0]); break;
      case 7: sevenResults.push_back(payoffs[0]); break;
      case 8: eightResults.push_back(payoffs[0]); break;
      case 9: nineResults.push_back(payoffs[0]); break;
      case 10: tenResults.push_back(payoffs[0]); break;
      case 1: aceResults.push_back(payoffs[0]); break;
      default: break;
      }
    }

    return payoffs;
  }
  
};

GameAction findBestAction(int numDecks, int numHands, int pVal1, int pVal2, int dUpVal, bool highCount){

  Deck deck = Deck(numDecks);
  Dealer dealer = Dealer();

  ActionPlayer player = ActionPlayer(GameAction::HIT);
  vector<Player*> players = {&player};

  Game g = Game(deck, &dealer, players);

  GameAction bestAction = GameAction::HIT;
  double bestResult = -999999999;

  for (int a=0; a<5; a++){

    if (a==3 && (pVal1 != pVal2))
      continue;
    
    player = ActionPlayer((GameAction) a);
    double result = 0;
    for (int i=0; i<numHands; i++){
      //deck.setup(pVal1, pVal2, dUpVal);

      // if (highCount){
      // 	deck.setup(vector<int> {2, 3, 4, 5, pVal1, pVal2, dUpVal});
      // 	deck.curCardNum = 0;
      // }
      // else{
	deck.setup(vector<int> {pVal1, pVal2, dUpVal});
      // }
      result += g.playHand(deck.curCount, false)[0];
    }
    if (result > bestResult){
      bestResult = result;
      bestAction = player.myAction;
    }
    
    //cout << "Percent return from " << player.myAction << ": " << 100*result/numHands << endl;
  }
  
  //cout << "For " << pVal1 << " " << pVal2 << " " << dUpVal << " the best action is " << bestAction << endl;

  if (bestAction == GameAction::SPLIT)
    numSplits++;

  return bestAction;
}

int main(int argc, char** argv){


  RandomDeck rd = RandomDeck();

  for (int i=0; i<10; i++)
     cout << rd.dealCard() << endl;

  // struct timespec start, finish;
  // double elapsed;
  
  // clock_gettime(CLOCK_MONOTONIC, &start);

  // int numDecks = 1;

  // GameAction bestActions[1000];

  // for (int i=1; i<11; i++){
  //   for (int j=1; j<11; j++){
  //     for (int k=1; k<11; k++){
  // 	//bestActions[i-1][j-1][k-1] = findBestAction(1, 100, i, j, k);
  // 	bestActions[100*(i-1) + 10*(j-1) + k - 1] = findBestAction(numDecks, 1000, i, j, k, false);
  //     }
  //   }
  // }

  // cout << endl << "num optimal splits: " << numSplits << endl;
  // cout << "num optimal surrenders: " << numSurrenders << endl;

  // numSplits = 0;
  // numSurrenders = 0;

  // Deck d = Deck(numDecks);
  // d.shuffle();

  // Dealer dealer = Dealer();
  // //Dealer player = Dealer();
  // //BasicStrategy player = BasicStrategy();
  // //ActionPlayer player = ActionPlayer(GameAction::HIT);
  // BestActionPlayer player = BestActionPlayer(bestActions, bestActions);
  // vector<Player*> players = {&player};

  // Game g = Game(d, &dealer, players);

  // double result = 0;
  // int numHands = 1000000;
  // for (int i=0; i<numHands; i++){
  //   if (i%5 == 0)
  //     d.shuffle();
  //   // d.setup(vector<int> {});
  //   // d.curCardNum = 0;
  //   //result += g.playHand(0)[0];
  //   result += g.playHand(d.curCount)[0];
  // }
  // cout << "Units won/lost: " << result << endl;
  // cout << "Units bet: " << vSum(totalWagers) << endl;
  // cout << "Percent won/lost: " << 100*result/vSum(totalWagers) << endl;
  // cout << "Percent won/lost versus dealer two upcard: " << 100*vMean(twoResults) << endl;
  // cout << "Percent won/lost versus dealer three upcard: " << 100*vMean(threeResults) << endl;
  // cout << "Percent won/lost versus dealer four upcard: " << 100*vMean(fourResults) << endl;
  // cout << "Percent won/lost versus dealer five upcard: " << 100*vMean(fiveResults) << endl;
  // cout << "Percent won/lost versus dealer six upcard: " << 100*vMean(sixResults) << endl;
  // cout << "Percent won/lost versus dealer seven upcard: " << 100*vMean(sevenResults) << endl;
  // cout << "Percent won/lost versus dealer eight upcard: " << 100*vMean(eightResults) << endl;
  // cout << "Percent won/lost versus dealer nine upcard: " << 100*vMean(nineResults) << endl;
  // cout << "Percent won/lost versus dealer ten upcard: " << 100*vMean(tenResults) << endl;
  // cout << "Percent won/lost versus dealer ace upcard: " << 100*vMean(aceResults) << endl;

  // clock_gettime(CLOCK_MONOTONIC, &finish);

  // elapsed = (finish.tv_sec - start.tv_sec);
  // elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  // printf("Actual elapsed time is %f seconds\n", elapsed);

  return 0;
}

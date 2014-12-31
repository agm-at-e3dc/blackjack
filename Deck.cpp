#ifndef DECKCPP
#define DECKCPP

#include <random>

#include "Card.cpp"

using namespace std;

struct Deck{

  Card* cards;
  int numCards;
  int topCard;

  mt19937 gen;
  
  ~Deck(){
    delete[] cards;
  }

  Deck(){}

  Deck(int numDecks){
    numCards = 52 * numDecks;
    cards = new Card[numCards];
    for (int i=0; i<numDecks; i++){
      for (int j=0; j<13; j++){
	for (int k=0; k<4; k++){
	  cards[52*i + 4*j + k] = REGULAR_CARDS[j];
	}
      }
    }
	  
    random_device rd;
    gen = mt19937(rd());

    topCard = 0;
  }

  // randomly permute the cards, leaving the first start alone
  void shuffle(int start = 0){
    for (int i=start; i<numCards; i++){
      int swapNum = i + (int)((generate_canonical<double, 32>(gen)) * (numCards-i));
      if (swapNum >= numCards){
	cout << "Zero-measure event in shuffle! Setting swapNum to numCards - 1." << endl;
	swapNum = numCards - 1;
      }
      Card temp = cards[swapNum];
      cards[swapNum] = cards[i];
      cards[i] = temp;
    }
    topCard = 0;
  }

  Card& dealCard(){
    return cards[topCard++];
  }

  // search the deck starting at card start for a card w/value val and swap card at start for it
  void findAndSwap(int start, int val){
    for (int i=start; i<numCards; i++){
      if (cards[i].value == val){
	Card temp = cards[i];
	cards[i] = cards[start];
	cards[start] = temp;
      }
    }
  }

  // if count > 0, effectively puts count high cards into discard pile
  // if count < 0, effectively puts count low cards into discard pile
  void setCount(int count){
    if (count > 0){
      int curVal = 2;
      for (int i=0; i<count; i++){
	findAndSwap(i, curVal);
	curVal++;
	if (curVal > 6)
	  curVal = 2;
      }
    }
    else if (count < 0){
      for (int i=0; i<(-count); i++)
	findAndSwap(i, 10);
    }
    if (count>=0)
      topCard = count;
    else
      topCard = -count;
  }
  
};

struct RandomDeck{

  mt19937 gen;

  RandomDeck(){
    random_device rd;
    gen = mt19937(rd());
  }

  Card dealCard(){
    int cardNum = (int)((generate_canonical<double, 32>(gen)) * (13));
    return REGULAR_CARDS[cardNum];
  }

};


#endif

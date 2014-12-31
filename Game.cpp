#ifndef GAMECPP
#define GAMECPP

#include "Deck.cpp"
#include "Player.cpp"

static double BJRATIO = 1.5;

struct CountResult{

  int count;
  double result;
  double wager;

  CountResult() : count(0), result(0) {}
  CountResult(int count, double result, double wager) : count(count), result(result), wager(wager) {}
};

vector<CountResult> aggregateResults(vector<CountResult> countResults){
  vector<CountResult> aggResults;
  for (int i=-30; i<31; i++)
    aggResults.push_back(CountResult(i, 0., 0.));
  for (CountResult& cr : countResults){
    if (cr.count >=-30 && cr.count <=30){
      aggResults[cr.count + 30].result += cr.result;
      aggResults[cr.count + 30].wager  += cr.wager;
    }
  }
  return aggResults;
}

ostream& operator<<(ostream& out, CountResult& cr){
  out << cr.count << " " << cr.result << " " << cr.wager;
  return out;
}

void printCountResult(int count, vector<CountResult> countResults){
  
  double result = 0;
  double wager = 0;
  int numResults = 0;
  for (auto& cr : countResults)
    if (cr.count == count){
      result += cr.result;
      wager += cr.wager;
      numResults++;
    }
  cout << "Count " << count << " : " << (result/wager * 100) << "% | " << wager << " | " << result << endl;
}

struct Game{
  
  Deck* deck;
  Player* dealer;
  Player* player;

  Game(Deck* deck, Player* dealer, Player* player) : deck(deck), dealer(dealer), player(player) {}

  Hand dealHand(){
    Hand newHand;
    newHand.addCard(deck->dealCard());
    newHand.addCard(deck->dealCard());
    return newHand;
  }

  double playHands(int numHands, double penetration, HandMatrix<double>& hm, vector<CountResult>& countResults,
		   bool printResults, int setCount){
    vector<CountResult> insuranceResults;
    double result = 0;
    int numShuffles = 0;
    deck->shuffle();
    for (long int i=0; i<numHands; i++){
      if ( ((double) deck->topCard / deck->numCards) > penetration){
	deck->shuffle();
	deck->setCount(setCount);
	player->shuffling();
	numShuffles++;
      }
      if (i % 10000000 == 0){
	cout << i/1000000 << " million " << endl;
	//printf("%dm\n", i/1000000);
	countResults = aggregateResults(countResults);
	insuranceResults = aggregateResults(insuranceResults);
      }
      
      HandState hs;
      int dealerUpCard = 0;
      Hand playerHand, dealerHand;
      int playerCount  = player->getRealCount();
      double insuranceResult;
      double wager;
      double handResult = playHand(hs, dealerUpCard, playerHand, dealerHand, insuranceResult, wager);
      player->endHand(playerHand, dealerHand);
      result += handResult;
      countResults.push_back(CountResult(playerCount, handResult, wager));
      if (insuranceResult != 0)
	insuranceResults.push_back(CountResult(playerCount, insuranceResult, wager));
      hm.update(hs, dealerUpCard, handResult);
    }
    if (printResults){
      cout << "Shuffled " << numShuffles << " times." << endl;
      cout << "true total result: " << hm.percentReturn()*numHands/player->totalWager << "%" << endl;
      cout << "units won: " << hm.totalData() << endl;
      cout << "Wager total: " << player->totalWager << endl;
      for (int i=-10; i<11; i++)
	printCountResult(i, insuranceResults);
      // for (Card& c : COMPACT_CARDS)
      // 	cout << "result versus dealer upcard " << c.name << ": " << hm.percentReturn(c.value) << "% | "
      // 	     << hm.totalCount(c.value) << endl;
      // for (int i=2; i<22; i++)
      // 	cout << "player =  " << i << "H return: " << hm.percentReturn(HandState(i, false)) << "% | "
      // 	     << hm.totalCount(HandState(i, false)) << endl;
      // for (int i=2; i<22; i++)
      // 	cout << "player =  " << i << "S return: " << hm.percentReturn(HandState(i, true)) << "% | "
      // 	     << hm.totalCount(HandState(i, true)) << endl;
      
    }
    return result;
  }

  double playHand(HandState& hs, int& dealerUpCard, Hand& playerHand, Hand& dealerHand, double& insuranceResult, double& wager){
    wager = player->chooseWager();
    
    playerHand = dealHand();
    dealerHand = dealHand();

    hs = HandState(playerHand.value(), playerHand.hasAce());
    dealerUpCard = dealerHand.cards[0].value;

    bool tookInsurance = false;
    double insurancePayoff = 0;    
    // if dealer upcard is A, check if player wants to take insurance
    if (dealerHand.cards[0].name == 'A'){
      tookInsurance = player->takeInsurance();
      if (tookInsurance)
    	insurancePayoff = -0.5 * wager;
    }

    // check for BJ's and pay out accordingly
    bool playerBJ = playerHand.isBlackjack();
    bool dealerBJ = dealerHand.isBlackjack();
    if (dealerBJ && tookInsurance)
      insurancePayoff = wager;

    insuranceResult = insurancePayoff;
    
    if (playerBJ && !dealerBJ)
      return BJRATIO * wager + insurancePayoff;
    if (dealerBJ && !playerBJ)
      return -1 * wager + insurancePayoff;
    if (dealerBJ && playerBJ)
      return 0 + insurancePayoff;

    // if neither person has BJ, player plays 
    bool canSplit = true;
    if (playerHand.cards[0].value != playerHand.cards[1].value)
      canSplit = false;
    bool canDouble = true;
    bool canSurrender = true;
    bool keepPlaying = true;
    while (keepPlaying){
      GameAction curAction = player->makeDecision(playerHand, dealerHand, canSplit, canDouble, canSurrender);
      switch (curAction){
      case GameAction::HIT:
    	playerHand.addCard(deck->dealCard());
    	canSplit = false;
    	canDouble = false;
    	canSurrender = false;
    	break;
      case GameAction::STAND:
    	keepPlaying = false;
    	break;
      case GameAction::DOUBLE:
    	wager *= 2;
    	playerHand.addCard(deck->dealCard());
    	keepPlaying = false;
    	break;
      case GameAction::SPLIT: // this is still approximate only
    	wager *= 2;
    	playerHand.cards[1] = deck->dealCard();
    	canSplit = false;
	canSurrender = false;
    	break;
      case GameAction::SURRENDER:
    	return -.5 * wager + insurancePayoff;
      default:
	cout << "Programming fail in playHand." << endl;
	exit(0);
      }
      if (playerHand.value() > 21)
	keepPlaying = false;
    }

    // if player busted, he loses and dealer does not play
    if (playerHand.value() > 21)
      return -1 * wager + insurancePayoff;

    // otherwise dealer plays
    keepPlaying = true;
    while (keepPlaying){
      GameAction curAction = dealer->makeDecision(dealerHand, playerHand, false, false, false);
      switch (curAction){
      case GameAction::HIT:
    	dealerHand.addCard(deck->dealCard());
    	break;
      case GameAction::STAND:
    	keepPlaying = false;
    	break;
      default:
    	cout << "Dealer made an illegal action. Exiting." << endl;
    	exit(0);
      }
    }
    
    // if dealer busted, player wins. 
    if (dealerHand.value() > 21)
      return wager + insurancePayoff;

    // otherwise, we compare the hand values
    int bestPlayerVal = playerHand.bestValue();
    int bestDealerVal = dealerHand.bestValue();
    if (bestPlayerVal < bestDealerVal)
      return -1 * wager + insurancePayoff;
    else if (bestPlayerVal == bestDealerVal)
      return 0 + insurancePayoff;
    else
      return wager + insurancePayoff;
  }

};

#endif

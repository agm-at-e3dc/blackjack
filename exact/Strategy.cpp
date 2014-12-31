#ifndef STRATEGYCPP
#define STRATEGYCPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

#include "vectorFunctions.cpp"
#include "Lookup.cpp"
#include "GameAction.cpp"
#include "Prob.cpp"
#include "HandState.cpp"
#include "StrategyElement.cpp"

vector<string> split(string str, const char delim) {
    vector<string> v;
    string tmp;

    for(string::const_iterator i = str.begin(); i <= str.end(); ++i) {
        if(*i != delim && i != str.end()) {
            tmp += *i; 
        } else {
            v.push_back(tmp);
            tmp = ""; 
        }   
    }   

    return v;
}

// represents an actionable state, that is a certain player's HandState and an observed dealer's upcard
struct StatePair{

  HandState hs;
  int dealerUpcard;

  StatePair() : hs(HandState()), dealerUpcard(0) {}
  
  StatePair(HandState hs, int dealerUpcard) : hs(hs), dealerUpcard(dealerUpcard) {}

  bool operator==(const StatePair& sp) const{
    return (hs == sp.hs && dealerUpcard == sp.dealerUpcard);
  }
  
};

ostream& operator<<(ostream& out, const StatePair& sp){
  //out << sp.hs << "-" << sp.dealerUpcard;
  out << sp.hs << endl;
  out << sp.dealerUpcard;
  return out;
}

istream& operator>>(istream& in, StatePair& sp){
  in >> sp.hs;
  in >> sp.dealerUpcard;
  return in;
}

/*
  A strategy is a matrix where rows correspond to a player handState and the columns correspond to the dealer upcard.
  Each element gives the strategy's action when in that player handState facing that dealer upcard, and the corresponding
  expected value (with respect to the corresponding Prob that represents the probabilities of the next card.)
*/
struct Strategy{

  // minimal representation of the final state of both dealer and player's hand values.
  // Less than 16 can be grouped together as dealer never ends with <= 16.
  // similarly values >= 22 can be grouped, as they are all bust.
  vector<int> endVals; // 16(less than 17), 17, 18, 19, 20, 21, 22(bust)

  Prob<int> prob; // probs here are chance next card is whatever (index 0 = 'A', 1='2', etc)
  Lookup<HandState, Prob<int> > dealerTable; // probs here are chance dealer ends up in state hard: 17, 18, 19, 20, 21, 22(bust)
  Lookup<int, Prob<int> > dealerFinalStates; // same as ^^, except from initial upcard not a full state
  Lookup<StatePair, StrategyElement> playerTable; // maps a given situation to a best action (and other info)
  
  Strategy(Prob<int>& prob) : prob(prob) {

    // if (tryToLoad(prob.pdf))
    //   return;
    // else{
      endVals = {16, 17, 18, 19, 20, 21, 22};
      bootstrapDealer();
      bootstrapPlayer();

    //   serialize();
    // }
  }

  vector<double> getPdfFromString(string strat){

    vector<string> numStrs = split(strat, '-');
    vector<double> strPdf;
    for (string numStr : numStrs){
      stringstream s;
      s << numStr;
      double d;
      s >> d;
      strPdf.push_back(d);
    }

    return strPdf;
  }
  
  double pdfDist(vector<double> pdf1, vector<double> pdf2){
    double dist = 0;
    for (int i=0; i<(int) pdf1.size(); i++)
      dist += fabs(pdf1[i] - pdf2[i]);
    return dist;
  }

  bool tryToLoad(vector<double> newPdf, double PDF_TOL = .01){
    fstream g("knownStrategies.txt", fstream::in);
    string strat = "start";
    while (strat.size() > 2){
      strat = "";
      g >> strat;
      vector<double> knownPdf = getPdfFromString(strat);
      double dist = pdfDist(newPdf, knownPdf);
      // cout << "pdf dist : " << dist << endl;
      if (dist < PDF_TOL){
	//cout << "found a match, loading it" << endl;
	stringstream s;
	s << "known/";
	s << strat;
	string filename;
	s >> filename;
	deserialize(filename);
	return true;
      }
    }
    return false;
  }

  void deserialize(string filename){

    fstream f(filename, fstream::in);
    f >> prob;
    f >> dealerTable;
    f >> dealerFinalStates;
    f >> playerTable;
  }
  
  void serialize(){

    stringstream s;
    for (int i=0; i<(int) prob.pdf.size()-1; i++)
      s << prob.pdf[i] << "-";
    s << prob.pdf[prob.pdf.size()-1];
    string strat;
    s >> strat;

    fstream g("knownStrategies.txt", fstream::out | fstream::app);
    g << strat << endl;
    g.close();
    
    stringstream ss;
    ss << "known/";
    ss << strat;
    string filename;
    ss >> filename;

    fstream f(filename, fstream::out);

    // real serialization begins here
    
    //f << "prob.pdf" << endl;
    f << prob << endl;
    //f << "dealerTable" << endl;
    f << dealerTable << endl;
    //f << "dealerFinalStates" << endl;
    f << dealerFinalStates << endl;
    //f << "playerTable" << endl;    
    f << playerTable;
    
    f.close();

  }
  
  // use the tools we have to create the dealer table and dealerFinalStates 
  void bootstrapDealer(){
    computeDealerTable();
    for (int i=1; i<11; i++)
      dealerFinalStates.add(i, computeDealerFinalState(i));
  }

  // computes the probabilities associated with each action at each player handState, dealerUpcard pair state by
  // assuming optimal downstream play. Uses the dealerTable's final state pdfs to evaluate best actions,
  // backfilling the player state from state 21H.
  void bootstrapPlayer(){

    for (int i=1; i<11; i++) // busts
      playerTable.add(StatePair(HandState(22, false, false, false), i), StrategyElement({-1, -999, -999, -999}));

    for (int i=21; i>10; i--){ // 21H to 11H
      for (int j=1; j<11; j++){
	StatePair sPair = StatePair(HandState(i, false, false, false), j);
	playerTable.add(sPair, computePlayerAction(sPair));
      }
    }

    for (int i=10; i>4; i--){ // 10H,10S to 5H, 5S
      for (int j=1; j<11; j++){
	StatePair sPair = StatePair(HandState(i, false, false, false), j);
	playerTable.add(sPair, computePlayerAction(sPair));
	sPair = StatePair(HandState(i, true, false, false), j);
	playerTable.add(sPair, computePlayerAction(sPair));
      }
    }

    for (int i=4; i>2; i--){ // 4S and 3S
      for (int j=1; j<11; j++){
    	StatePair sPair = StatePair(HandState(i, true, false, false), j);
    	playerTable.add(sPair, computePlayerAction(sPair));
      }
    }

    for (int i=10; i>1; i--){ // TT through 22
      for (int j=1; j<11; j++){
    	StatePair sPair = StatePair(HandState(2*i, false, true, false), j);
    	playerTable.add(sPair, computePlayerAction(sPair));
      }
    }

    for (int j=1; j<11; j++){ // AA
      StatePair sPair = StatePair(HandState(2, true, true, false), j); 
      playerTable.add(sPair, computePlayerAction(sPair));
    }
    
  }
  
  // finds the outcome of each possible player action at a given handState, dealerUpcard pair state
  // based on the assumption of optimal play for the remainder of the hand,
  // then chooses the best action. Requires that all downstream player HandState, dealerUpcard pairs states already be computed.

  // basically we want to map our actions onto a final state probability vector for the player. Then, combined with
  // the already known dealer final state probability vector, we can easily compute the expected value.
  
  StrategyElement computePlayerAction(StatePair sp){
    Prob<int> dfs = dealerFinalStates.get(sp.dealerUpcard);
    vector<double> results;
    Prob<int> pfs = Prob<int>(endVals, {0., 0., 0., 0., 0., 0., 0.}); // player's final pdf. Will be different for each action.    
    
    // stand
    int newVal = sp.hs.value;
    if (sp.hs.soft && newVal + 10 <= 21)
      newVal += 10;
    if      (newVal <= 16) pfs.pdf = {1., 0., 0., 0., 0., 0., 0.};
    else if (newVal == 17) pfs.pdf = {0., 1., 0., 0., 0., 0., 0.};
    else if (newVal == 18) pfs.pdf = {0., 0., 1., 0., 0., 0., 0.};
    else if (newVal == 19) pfs.pdf = {0., 0., 0., 1., 0., 0., 0.};
    else if (newVal == 20) pfs.pdf = {0., 0., 0., 0., 1., 0., 0.};
    else if (newVal == 21) pfs.pdf = {0., 0., 0., 0., 0., 1., 0.};
    else                   pfs.pdf = {0., 0., 0., 0., 0., 0., 1.};
    results.push_back(computeExpectation(pfs.pdf, dfs.pdf));
    
    // hit
    double ev = 0;
    for (int i=0; i<10; i++){
      if ((sp.hs.soft || i == 0) && sp.hs.value < 22){ // player has an ace already or gets an ace
    	  if (sp.hs.value + i + 1 <= 10)
    	    ev += prob.pdf[i] * playerTable.get(StatePair(HandState(sp.hs.value+i+1, true, false, false), sp.dealerUpcard)).bestValue;
    	  else if (sp.hs.value + i + 1 == 11)
    	    ev += prob.pdf[i] * playerTable.get(StatePair(HandState(21, false, false, false), sp.dealerUpcard)).bestValue;
    	  else
    	    ev += prob.pdf[i] * playerTable.get(StatePair(HandState(sp.hs.value+i+1,false, false, false), sp.dealerUpcard)).bestValue;
      }
      else{
    	  if (sp.hs.value+i+1 < 22)
    	    ev += prob.pdf[i] * playerTable.get(StatePair(HandState(sp.hs.value+1+i, false, false, false),sp.dealerUpcard)).bestValue;
    	  else
    	    ev += prob.pdf[i] * playerTable.get(StatePair(HandState(22, false, false, false), sp.dealerUpcard)).bestValue; // =-1 !
      }
    }
    if (ev > 1 + .0000000001){ // fix this shit
      // cout << "wtfuuu" << endl;
      // cout << ev << endl;
      // cout << sp << endl;
      
    }
    results.push_back(ev);

    // double down
    pfs.pdf = {0., 0., 0., 0., 0., 0., 0.};
    for (int i=0; i<10; i++){
      int newVal = sp.hs.value + i + 1;

      if (sp.hs.soft || i == 0)
    	if (newVal + 10 <= 21)
    	  newVal = newVal + 10;

      if (newVal <= 16)      pfs += prob.pdf[i] * Prob<int>(endVals, {1., 0., 0., 0., 0., 0., 0.});
      else if (newVal == 17) pfs += prob.pdf[i] * Prob<int>(endVals, {0., 1., 0., 0., 0., 0., 0.});
      else if (newVal == 18) pfs += prob.pdf[i] * Prob<int>(endVals, {0., 0., 1., 0., 0., 0., 0.});
      else if (newVal == 19) pfs += prob.pdf[i] * Prob<int>(endVals, {0., 0., 0., 1., 0., 0., 0.});
      else if (newVal == 20) pfs += prob.pdf[i] * Prob<int>(endVals, {0., 0., 0., 0., 1., 0., 0.});
      else if (newVal == 21) pfs += prob.pdf[i] * Prob<int>(endVals, {0., 0., 0., 0., 0., 1., 0.});
      else                   pfs += prob.pdf[i] * Prob<int>(endVals, {0., 0., 0., 0., 0., 0., 1.});
    }
    results.push_back(2 * computeExpectation(pfs.pdf, dfs.pdf));
    
    // split
    ev = 0;
    if (sp.hs.pair){
      int splitVal = sp.hs.value / 2;
      if (splitVal == 1){ //splitting aces
    	for (int i=0; i<10; i++){
    	  if (i == 9) // A + T
    	    ev += prob.pdf[i] * playerTable.get(StatePair(HandState(21, false, false, false), sp.dealerUpcard)).bestValue;
    	  else if (i == 0) // A + A again
    	    ; // deal with this by adjusting ev
    	  else
    	    ev += prob.pdf[i] * playerTable.get(StatePair(HandState(splitVal+i+1, true, false, false), sp.dealerUpcard)).bestValue;
    	}
      }
      else{ // splitting 2-2 ... T-T
    	for (int i=0; i<10; i++){
    	  if (i == 0){
    	    if (splitVal + 1 == 11) // T + A
    	      ev += prob.pdf[i] * playerTable.get(StatePair(HandState(21, false, false, false), sp.dealerUpcard)).bestValue;
    	    else
    	      ev += prob.pdf[i] * playerTable.get(StatePair(HandState(splitVal+1, true, false, false), sp.dealerUpcard)).bestValue;
    	  }
    	  else if (splitVal == i + 1) // X + X again
    	      ; // deal with this by adjusting ev
    	  else
    	    ev += prob.pdf[i] * playerTable.get(StatePair(HandState(splitVal+i+1, false, false, false), sp.dealerUpcard)).bestValue;
    	}
      }
      results.push_back(2 * ev / (1. - prob.pdf[sp.hs.value/2])); // accounts for resplits nicely
    }
    else
      results.push_back(-999);

    return StrategyElement(results);
  }

  // computes the expectation value of a player's final state pdf versus the dealer's.
  // note that blackjack is dealt with earlier, only possible results here are +1 unit, draw, or -1 unit
  double computeExpectation(vector<double> playerPdf, vector<double> dealerPdf){
    double result = 0;

    // if player busts he loses regardless of dealer
    result -= playerPdf[6];
    
    // non-busted player wins if dealer busts or his value is higher than dealer's
    // he draws if the values are the same
    // and he loses if his value is less than dealer's
    for (int i=0; i<6; i++){ // start at 16, proceed to 21

      result += playerPdf[i] * dealerPdf[6]; // player wins if dealer busts
      
      for (int j=0; j<i; j++) // player has higher score than non-busted dealer
	result += playerPdf[i] * dealerPdf[j];

      // ignore draws, result += 0

      for (int j=i+1; j<6; j++) // player loses if his score is less than dealer's
	result -= playerPdf[i] * dealerPdf[j];
    }
      
    return result;
  }
  
  // calculate the pdf of the dealer final state. For now we assume dealer stands on soft 17.
  // Dealer has only 6 possible final states: 17, 18, 19, 20, 21, and bust, which we represent as 22.
  // e.g. w/ dealer upcard 4, we have possible states 5s, 6H-14H.
  // hence to compute the final state, we need the final states of 5s and 6H-14H, and then
  // the final state of duc 4 is just the probability weighted avg of ^^ those states from the dealerTable.
  Prob<int> computeDealerFinalState(int upcard){
    Prob<int> result = Prob<int>(endVals, {0., 0., 0., 0., 0., 0., 0.});

    // if his upcard is an A , then the second card cant be a 10 (since he didn't bj)
    // hence, mult probs by 1/(1-prob[9])
    if (upcard == 1){
      double condFactor = 1./(1. - prob.pdf[9]);
      for (int i=0; i<9; i++){
	if (upcard + i + 1 <= 6)
	  result += condFactor * prob.pdf[i] * dealerTable.get(HandState(upcard+i+1, true, false, false));
	else
	  result += condFactor * prob.pdf[i] * dealerTable.get(HandState(upcard+i+1+10, false, false, false));
	// note the third case, (upcard + i) >= 12, is not possible!
      }
    }

    // if his upcard is 10, then his second card can't be an A (since he didn't bj)
    // hence mult probs by 1/(1-prob[0])
    else if (upcard == 10){
      double condFactor = 1./(1. - prob.pdf[0]);
      for (int i=1; i<10; i++){
	result += condFactor * prob.pdf[i] * dealerTable.get(HandState(upcard+1+i, false, false, false));
      }
    }

    // if his upcard is 2-9, his second card can be anything
    else{
      // first do the case where he gets an A
      if ((upcard + 1) <= 6)
	result += prob.pdf[0] * dealerTable.get(HandState(upcard+1, true, false, false));
      else 
	result += prob.pdf[0] * dealerTable.get(HandState(upcard+1+10, false, false, false));
      // note the third case, (upcard + 1) >= 12, is not possible!
      
      // then 2-10
      for (int i=1; i<10; i++)
	result += prob.pdf[i] * dealerTable.get(HandState(upcard+1+i, false, false, false));
    }
    
    return result;
  }

  // gives the dealer's final state pdf for each upcard 1-10, ie. A,2, ...., 10 
  // so, if we calculate the dealer's final state pdf for each needed handstate (2s-10s + 4H-16H + the trivial ones)
  // we can instantly calculate the final state of each duc by a simple sum
  // Now start at dealer 16H and work your way backwards. Remember above 10 there are no soft.
  // e.g. 16 hard-> (1/13, 1/13, 1/13, 1/13, 1/13, 8/13)
  //      15 hard -> 1/13 * 16H + (1/13, 1/13, 1/13, 1/13, 1/13, 7/13);
  void computeDealerTable(){
  
    // remember the trivial ones: 18 H -> (0, 1, 0, 0, 0, 0), etc.
    //Lookup<HandState,Prob<int> > dealerTable;
    dealerTable.add(HandState(17, false, false, false), Prob<int>(endVals, {0., 1., 0., 0., 0., 0., 0.}));
    dealerTable.add(HandState(18, false, false, false), Prob<int>(endVals, {0., 0., 1., 0., 0., 0., 0.}));
    dealerTable.add(HandState(19, false, false, false), Prob<int>(endVals, {0., 0., 0., 1., 0., 0., 0.}));
    dealerTable.add(HandState(20, false, false, false), Prob<int>(endVals, {0., 0., 0., 0., 1., 0., 0.}));
    dealerTable.add(HandState(21, false, false, false), Prob<int>(endVals, {0., 0., 0., 0., 0., 1., 0.}));
    dealerTable.add(HandState(22, false, false, false), Prob<int>(endVals, {0., 0., 0., 0., 0., 0., 1.}));

    // and blackjack!
    // dealerTable.add(HandState(21, false, false, true ), Prob<int>(endVals, {0., 0., 0., 0., 0., 1., 0.}));
    
    HandState hs;
    for (int i=16; i>10; i--){
      hs = HandState(i, false, false, false);
      dealerTable.add(hs, computeDealerState(hs));
    }
    for (int i=10; i>3; i--){
      hs = HandState(i, false, false, false);
      dealerTable.add(hs, computeDealerState(hs)); 
      hs = HandState(i, true, false, false);
      dealerTable.add(hs, computeDealerState(hs));
    }
    hs = HandState(3, true, false, false); // A + 2
    dealerTable.add(hs, computeDealerState(hs));    
    hs = HandState(2, true, false, false); // A + A
    dealerTable.add(hs, computeDealerState(hs));     
  }

  Prob<int> computeDealerState(HandState hs){
    // hand has ace. if the card we draw to brings total value to 6 or less, we remain in soft state.
    // If the value is between 7 and 11, we transition to the value + 10 hard state.
    // If it is 12 or over, we go to hard state at that value.
    // note that you cannot bust from a soft state.
    if (hs.soft){
      if (hs.value > 10){
	cout << "soft hands can't be above 10, fuuuuuuuu" << endl;
	exit(0);
      }
      else if (hs.value > 6)
	return dealerTable.get(HandState(hs.value + 10, false, false, false));

      // so we're at 2S-6S
      Prob<int> result = Prob<int>(endVals, {0., 0., 0., 0., 0., 0., 0.}); 
      for (int i=0; i<10; i++){
      	int newVal = hs.value + (i + 1);
	if (newVal <= 6)
	  result += prob.pdf[i] * dealerTable.get(HandState(newVal, true, false, false));
	else if (newVal <= 11)
	  result += prob.pdf[i] * dealerTable.get(HandState(newVal + 10, false, false, false)); 
	else
	  result += prob.pdf[i] * dealerTable.get(HandState(newVal, false, false, false));
      }
      return result;
    }
    // hand doesn't have ace. If the new card isn't an ace, transition to the new total value
    // if it is less than 22, and to 22 (bust) otherwise. If the new card is an ace,
    // then if new total value is <=6 transition to soft state, if between 7 and 11
    // transition to hard state at the value + 10, and if 12 or over transition to hard state at that value.
    else{ 
      Prob<int> result = Prob<int>(endVals, {0., 0., 0., 0., 0., 0., 0.}); 
      for (int i=0; i<10; i++){
	int newVal = hs.value + (i + 1);
	if (i == 0){
	  if (newVal <= 6)
	    result += prob.pdf[i] * dealerTable.get(HandState(newVal, true, false, false));
	  else if (newVal <= 11)
	    result += prob.pdf[i] * dealerTable.get(HandState(newVal + 10, false, false, false)); 
	  else
	    result += prob.pdf[i] * dealerTable.get(HandState(newVal, false, false, false));
	}
	else {
	if (newVal > 22) // busts map to 22
	  newVal = 22;
	result += prob.pdf[i] * dealerTable.get(HandState(newVal, false, false, false));
	}
      }
      return result;
    }
  }
   
};  

#endif


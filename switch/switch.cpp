#include <ctime>

#include "Strategy.cpp"
#include <sstream>

vector<double> updatePdf(vector<double> pdf, int cardVal, int numCards){
  vector<double> newPdf;
  for (int i = 0; i < (int) pdf.size(); i++){
    if (i + 1 == cardVal){
      double newVal = pdf[i] - 1./numCards;
      if (newVal < 0)
	newVal = 0;
      newPdf.push_back(newVal);
    }
    else
      newPdf.push_back(pdf[i]);
  }
  double tot = 0;
  for (int i = 0; i < (int) pdf.size(); i++)
    tot += newPdf[i];
  for (int i = 0; i < (int) pdf.size(); i++)
    newPdf[i] /= tot;
  return newPdf;
}

int main(int argc, char** argv){
  try{
    srand(time(NULL));
    
    vector<double> compPdf;
    int numCards;
    double count;
    if (argc > 10){
      for (int i=1; i<11; i++){
	stringstream ss;
	ss << argv[i];
	double L;
	ss >> L;
	compPdf.push_back(L);
      }
      stringstream ss;
      ss << argv[11];
      ss >> numCards;
      stringstream sss;
      sss << argv[12];
      sss >> count;
      
    }
    else if (argc > 1){
      compPdf = vector<double>{1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 4./13.};
      stringstream ss;
      ss << argv[1];
      ss >> numCards;
      stringstream sss;
      sss << argv[2];
      sss >> count;
    }
    else{
      compPdf = vector<double>{1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 4./13.};
      numCards = 52;
      count = 0;
    }

    vector<int> cards = vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Prob<int> p_basic = Prob<int>(cards, {1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 4./13.});      
    Strategy* basic_ptr = new Strategy(p_basic);

    double ev = 0;
    double totProb = 0;
    // int deviations = 0;

    for (int dc1=1; dc1<11; dc1++){
      vector<double> firstPdf = updatePdf(compPdf, dc1, numCards);

      // Prob<int> prob1 = Prob<int>(cards, firstPdf);
      // delete basic_ptr;      
      // basic_ptr = new Strategy(prob1);
      
      for (int dc2=1; dc2<11; dc2++){
	vector<double> secondPdf = updatePdf(firstPdf, dc2, numCards - 1);
	
	// Prob<int> prob1 = Prob<int>(cards, secondPdf);
	// delete basic_ptr;      
	// basic_ptr = new Strategy(prob1);

	for (int uc1=1; uc1<11; uc1++){
	  vector<double> thirdPdf = updatePdf(secondPdf, uc1, numCards - 2);

	  // Prob<int> prob1 = Prob<int>(cards, thirdPdf);
	  // delete basic_ptr;      
	  // basic_ptr = new Strategy(prob1);

	  for (int uc2=1; uc2<11; uc2++){
	    vector<double> fourthPdf = updatePdf(thirdPdf, uc2, numCards - 3);

	    // Prob<int> prob2 = Prob<int>(cards, fourthPdf);
	    // delete basic_ptr;      
	    // basic_ptr = new Strategy(prob2);
	    
	    for (int dealerCard=1; dealerCard<11; dealerCard++){
	    
	      double handProb = compPdf[dc1-1] * firstPdf[dc2-1] * secondPdf[uc1-1] * thirdPdf[uc2-1] * fourthPdf[dealerCard-1];
	      totProb += handProb;
	
	      StatePair orig1, orig2, sw1, sw2;

	      if (dc1 == 1 && uc1 == 1)
		orig1 = StatePair(HandState(2, true, true), dealerCard);
	      else if (dc1 == 1 || uc1 == 1)
		orig1 = StatePair(HandState(dc1 + uc1, true, false), dealerCard);
	      else if (dc1 == uc1)
		orig1 = StatePair(HandState(dc1 + uc1, false, true), dealerCard);
	      else
		orig1 = StatePair(HandState(dc1 + uc1, false, false), dealerCard);

	      if (dc2 == 1 && uc2 == 1)
		orig2 = StatePair(HandState(2, true, true), dealerCard);
	      else if (dc2 == 1 || uc2 == 1)
		orig2 = StatePair(HandState(dc2 + uc2, true, false), dealerCard);
	      else if (dc2 == uc2)
		orig2 = StatePair(HandState(dc2 + uc2, false, true), dealerCard);
	      else
		orig2 = StatePair(HandState(dc2 + uc2, false, false), dealerCard);

	      if (dc1 == 1 && uc2 == 1)
		sw1 = StatePair(HandState(2, true, true), dealerCard);
	      else if (dc1 == 1 || uc2 == 1){
		if ((dc1 ==1 && uc2 == 10) || (dc1 == 10 && uc2 == 1))
		  sw1 = StatePair(HandState(21, false, false), dealerCard);
		else
		  sw1 = StatePair(HandState(dc1 + uc2, true, false), dealerCard);
	      }
	      else if (dc1 == uc2)
		sw1 = StatePair(HandState(dc1 + uc2, false, true), dealerCard);
	      else
		sw1 = StatePair(HandState(dc1 + uc2, false, false), dealerCard);

	      if (dc2 == 1 && uc1 == 1)
		sw2 = StatePair(HandState(2, true, true), dealerCard);
	      else if (dc2 == 1 || uc1 == 1){
		if ((dc2 ==1 && uc1 == 10) || (dc2 == 10 && uc1 == 1))
		  sw2 = StatePair(HandState(21, false, false), dealerCard);
		else
		  sw2 = StatePair(HandState(dc2 + uc1, true, false), dealerCard);
	      }
	      else if (dc2 == uc1)
		sw2 = StatePair(HandState(dc2 + uc1, false, true), dealerCard);
	      else
		sw2 = StatePair(HandState(dc2 + uc1, false, false), dealerCard);

	      // cout << dc1 << " " << dc2 << " " << uc1 << " " << uc2 << " " << dealerCard << endl;
	      
	      // if one hand naturals, we only play out the other, as bj pays 1-1
	      if ((dc1 == 1 && uc1 == 10) || (dc1 == 10 && uc1 == 1)){
		if ((dc2 == 1 && uc2 == 10) || (dc2 == 10 && uc2 == 1))
		  continue;
		StrategyElement se_basic = basic_ptr->playerTable.get(orig2);
		switch(se_basic.bestAction){
		case GameAction::STAND:
		  ev += handProb * se_basic.values[0];
		  break;
		case GameAction::HIT:
		  ev += handProb * se_basic.values[1];
		  break;
		case GameAction::DOUBLE:
		  ev += handProb * se_basic.values[2];
		  break;
		case GameAction::SPLIT:
		  ev += handProb * se_basic.values[3];
		  break;
		default:
		  exit(0);
		}
		continue;
	      }
	      else if ((dc2 == 1 && uc2 == 10) || (dc2 == 10 && uc2 == 1)){
		StrategyElement se_basic = basic_ptr->playerTable.get(orig1);
		switch(se_basic.bestAction){
		case GameAction::STAND:
		  ev += handProb * se_basic.values[0];
		  break;
		case GameAction::HIT:
		  ev += handProb * se_basic.values[1];
		  break;
		case GameAction::DOUBLE:
		  ev += handProb * se_basic.values[2];
		  break;
		case GameAction::SPLIT:
		  ev += handProb * se_basic.values[3];
		  break;
		default:
		  exit(0);
		}
		continue;
	      }

	      // now we consider switching. To do this we look at the original hand evs and the switched hand evs,
	      // then choose the higher of the two.

	      double origEV1, origEV2, swEV1, swEV2;

	      StrategyElement origSE1 = basic_ptr ->playerTable.get(orig1);
	      switch(origSE1.bestAction){
	      case GameAction::STAND:
		origEV1 = origSE1.values[0];
		break;
	      case GameAction::HIT:
		origEV1 = origSE1.values[1];
		break;
	      case GameAction::DOUBLE:
		origEV1 = origSE1.values[2];
		break;
	      case GameAction::SPLIT:
		origEV1 = origSE1.values[3];
		break;
	      default:
		exit(0);
	      }

	      StrategyElement origSE2 = basic_ptr ->playerTable.get(orig2);
	      switch(origSE2.bestAction){
	      case GameAction::STAND:
		origEV2 = origSE2.values[0];
		break;
	      case GameAction::HIT:
		origEV2 = origSE2.values[1];
		break;
	      case GameAction::DOUBLE:
		origEV2 = origSE2.values[2];
		break;
	      case GameAction::SPLIT:
		origEV2 = origSE2.values[3];
		break;
	      default:
		exit(0);
	      }

	      StrategyElement swSE1 = basic_ptr ->playerTable.get(sw1);
	      switch(swSE1.bestAction){
	      case GameAction::STAND:
	      	swEV1 = swSE1.values[0];
	      	break;
	      case GameAction::HIT:
	      	swEV1 = swSE1.values[1];
	      	break;
	      case GameAction::DOUBLE:
	      	swEV1 = swSE1.values[2];
	      	break;
	      case GameAction::SPLIT:
	      	swEV1 = swSE1.values[3];
	      	break;
	      default:
	      	exit(0);
	      }
	      
	      StrategyElement swSE2 = basic_ptr ->playerTable.get(sw2);
	      switch(swSE2.bestAction){
	      case GameAction::STAND:
	      	swEV2 = swSE2.values[0];
	      	break;
	      case GameAction::HIT:
	      	swEV2 = swSE2.values[1];
	      	break;
	      case GameAction::DOUBLE:
	      	swEV2 = swSE2.values[2];
	      	break;
	      case GameAction::SPLIT:
	      	swEV2 = swSE2.values[3];
	      	break;
	      default:
	      	exit(0);
	      }
	      
	      if (swEV1 + swEV2 > origEV1 + origEV2)
		ev += handProb * (swEV1 + swEV2);
	      else
		ev += handProb * (origEV1 + origEV2);
	      
	    }
	  }
	}
      }
    }
    cout << ev * 100 << endl;
    cout << totProb << endl;    
    delete basic_ptr;
  }
  catch (char const* exc){
    cout << exc << endl;
  }

}

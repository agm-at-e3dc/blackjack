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

    // actual EV based on action we take -- note: assumes we play perfect composition-dependent afterwards, so actually OVERESTIMATES
    // peformance of non-composition dependent strategies

    Strategy* basic_ptr = NULL; // basic strategy 
    Strategy* low_ptr = NULL; // low count strategy
    Strategy* high_ptr = NULL; // high count strategy
    Strategy* comp_ptr = NULL; // exact comp dependent strategy
    
    // basic strategy
    Prob<int> p_basic = Prob<int>(cards, {1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 4./13.});      
    basic_ptr = new Strategy(p_basic);

    // low count strategy
    Prob<int> p_low = Prob<int>(cards, {0.047619047619,0.0952380952381,0.0952380952381,0.0952380952381,0.0952380952381,
	  0.0952380952381, 0.0952380952381,0.0952380952381,0.0952380952381,0.190476190476,});
    low_ptr = new Strategy(p_low);
    
    // high count strategy
    Prob<int> p_high = Prob<int>(cards, {0.0909090909091,0.0909090909091,0.0454545454545,0.0454545454545,0.0454545454545,
	  0.0454545454545,0.0909090909091,0.0909090909091,0.0909090909091,0.363636363636,});	
    high_ptr = new Strategy(p_high);

    // level-0 comp dependent
    Prob<int> p_comp = Prob<int>(cards, compPdf);      
    comp_ptr = new Strategy(p_comp);

    //Strategy* mixed_ptr = NULL; // special case 

    for (int strat = 0; strat < 5; strat++){
    
      //for (Strategy* current_ptr : {basic_ptr, low_ptr, high_ptr, comp_ptr, mixed_ptr}){

      double ev = 0;
      double totProb = 0;
      int deviations = 0;
      
      for (int i=1; i<11; i++){
      
	vector<double> firstPdf = updatePdf(compPdf, i, numCards);

	// Prob<int> p_comp = Prob<int>(cards, firstPdf);      
	// comp_ptr = new Strategy(p_comp);
	
	for (int j=1; j<11; j++){
		
	  vector<double> secondPdf = updatePdf(firstPdf, j, numCards - 1);

	  if (strat == 3){
	    Prob<int> p_comp = Prob<int>(cards, secondPdf);      
	    comp_ptr = new Strategy(p_comp);
	  }
	  
	  for (int k=1; k<11; k++){
	    
	    vector<double> thirdPdf = updatePdf(secondPdf, k, numCards - 2);

	    // if (strat == 3){
	      // p_comp = Prob<int>(cards, thirdPdf);      
	      // comp_ptr = new Strategy(p_comp);
	    // }
	    
	    double handProb = compPdf[i-1] * firstPdf[j-1] * secondPdf[k-1];
	  
	    totProb += handProb;

	    if (i == 1){ // check insurance
	      //if (current_ptr != NULL && current_ptr->prob.pdf[9] > .33) // we always take insurance in high count strategy
	      if (strat == 2)
	      	ev += handProb * (3 * thirdPdf[9] - 1);
	      else if ((3 * thirdPdf[9] - 1) > 0)
		ev += handProb * (3 * thirdPdf[9] - 1);
	    }

	    if ((j == 1 && k == 10) || (j == 10 && k == 1))
	      ev += handProb * .5; // the asymmetry gain from bj;
	    else{
	      StatePair sp;
	      if (j == 1 && k == 1)
		sp = StatePair(HandState(2, true, true, false), i);
	      else if (j == 1 || k == 1)
		sp = StatePair(HandState(j + k, true, false, false), i);
	      else if (j == k)
		sp = StatePair(HandState(j + k, false, true, false), i);
	      else
		sp = StatePair(HandState(j + k, false, false, false), i);

	      StrategyElement se_basic = basic_ptr->playerTable.get(sp);
	      StrategyElement se_comp = comp_ptr->playerTable.get(sp);
	      StrategyElement se_current;
	      if (strat==0)
		se_current = se_basic;
	      else if (strat==1)
		se_current = low_ptr->playerTable.get(sp);
	      else if (strat==2)
		se_current = high_ptr->playerTable.get(sp);
	      else if (strat==3)
		se_current = comp_ptr->playerTable.get(sp);
	      else{ // mixed strat
		if (count < -3)
		  se_current = low_ptr->playerTable.get(sp);
		else
		if (count < 3)
		  se_current = basic_ptr->playerTable.get(sp);
		else
		  se_current = high_ptr->playerTable.get(sp);
	      }

	      if (se_current.bestAction != se_basic.bestAction)
		deviations++;

	      if (strat!=3){
		switch (se_current.bestAction){
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
	      }
	      else{
		switch (se_current.bestAction){
		case GameAction::STAND:
		  ev += handProb * se_comp.values[0];
		  break;
		case GameAction::HIT:
		  ev += handProb * se_comp.values[1];
		  break;
		case GameAction::DOUBLE:
		  ev += handProb * se_comp.values[2];
		  break;
		case GameAction::SPLIT:
		  ev += handProb * se_comp.values[3];
		  break;
		default:
		  exit(0);
		}
	      }
	    }
	  }
	}
      }
      cout << ev * 100 << endl;
      cout << deviations/1000. << endl;
    }
    delete basic_ptr;
    delete low_ptr;
    delete high_ptr;
    delete comp_ptr;
  }
  catch (char const* exc){
    cout << exc << endl;
  }
}

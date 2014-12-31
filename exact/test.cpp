#include <ctime>

#include "Strategy.cpp"
#include <sstream>

int main(int argc, char** argv){
  try{
    srand(time(NULL));
    
    vector<double> pdf;
    if (argc > 1){
      for (int i=1; i<11; i++){
	stringstream ss;
	ss << argv[i];
	double L;
	ss >> L;
	pdf.push_back(L);
	// cout << L << endl;
      }
    }
    else
      pdf = vector<double>{1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 4./13.};

    vector<int> cards = vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Prob<int> p = Prob<int>(cards, pdf);
    Strategy S(p);

    double ev = 0;
    double totProb = 0;
    
    // exact optimal outcome in infinite deck limit
    for (int i=1; i<11; i++){
      double dealerProb = S.prob.pdf[i-1];

      if (i == 1) // check insurance
	if ((3 * S.prob.pdf[9] - 1) > 0)
	  ev += dealerProb * (3 * S.prob.pdf[9] - 1);
      
      for (int j=1; j<11; j++){
    	for (int k=1; k<11; k++){
	  double playerProb = S.prob.pdf[j-1] * S.prob.pdf[k-1];

	  totProb += dealerProb * playerProb;
	  
	  if ((j == 1 && k == 10) || (j == 10 && k == 1))
	    ev += dealerProb * playerProb * .5; // the asymmetry gain from bj;
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
	    StrategyElement se = S.playerTable.get(sp);
	    ev += dealerProb * playerProb * se.bestValue;
	  }
	  
    	}
      }

    }

    cout << ev * 100 << endl;
    
  }
  catch (char const* exc){
     cout << exc << endl;
  }
}



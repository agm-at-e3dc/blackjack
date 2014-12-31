#include <ctime>

#include "Strategy.cpp"
#include <sstream>

int main(int argc, char** argv){
  try{
    srand(time(NULL));
    
    vector<double> pdf;
    int numCards;
    if (argc > 1){
      for (int i=1; i<11; i++){
	stringstream ss;
	ss << argv[i];
	double L;
	ss >> L;
	pdf.push_back(L);
      }
      stringstream ss;
      ss << argv[11];
      ss >> numCards;
    }
    else{
      pdf = vector<double>{1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 1./13., 4./13.};
      numCards = 52;
    }
    
    vector<int> cards = vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    double ev = 0;
    double totProb = 0;

    // Prob<int> p = Prob<int>(cards, pdf);
    // Strategy S(p);

    for (int i=1; i<11; i++){
      double dealerProb = pdf[i-1];

      vector<double> newPdf;
      for (int q = 0; q < 10; q++){
	if (i == (q+1)){
	  double newVal = pdf[q] - 1./numCards;
	  if (newVal < 0)
	    newVal = 0;
	  newPdf.push_back(newVal);
	}
	else
	  newPdf.push_back(pdf[q]);
      }
      double newTotProb = 0;
      for (int q = 0; q < 10; q++)
	newTotProb += newPdf[q];
      for (int q = 0; q < 10; q++)
	newPdf[q] /= newTotProb;

      Prob<int> p = Prob<int>(cards, newPdf);
      Strategy S(p);
      
      for (int j=1; j<11; j++){

	vector<double> newerPdf;
	for (int q = 0; q < 10; q++){
	  if (j == (q+1)){
	    double newerVal = newPdf[q] - 1./(numCards-1);
	    if (newerVal < 0)
	      newerVal = 0;
	    newerPdf.push_back(newerVal);
	  }
	  else
	    newerPdf.push_back(newPdf[q]);
	}

	double newerTotProb = 0;
	for (int q = 0; q < 10; q++)
	  newerTotProb += newerPdf[q];
	for (int q = 0; q < 10; q++)
	  newerPdf[q] /= newerTotProb;
	
	// Prob<int> p = Prob<int>(cards, newerPdf);
	// Strategy S(p);

	for (int k=1; k<11; k++){
	  double playerProb = newPdf[j-1] * newerPdf[k-1];

	  totProb += dealerProb * playerProb;
	  
	  vector<double> newestPdf;
	  for (int q = 0; q < 10; q++){
	    if (k == (q+1)){
	      double newestVal = newerPdf[q] - 1./(numCards-2);
	      if (newestVal < 0)
		newestVal = 0;
	      newestPdf.push_back(newestVal);
	    }
	    else
	      newestPdf.push_back(newerPdf[q]);
	  }
	  double newestTotProb = 0;
	  for (int q = 0; q < 10; q++)
	    newestTotProb += newestPdf[q];
	  for (int q = 0; q < 10; q++)
	    newestPdf[q] /= newestTotProb;

	  if (i == 1) // check insurance
	    if ((3 * newestPdf[9] - 1) > 0)
	      ev += dealerProb * playerProb * (3 * newestPdf[9] - 1);

	  // Prob<int> p = Prob<int>(cards, newestPdf);
	  // Strategy S(p);
	  
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
    
    // if ((3*pdf[9] - 1) > 0)  // insurance
    //   cout << (ev + pdf[0] * (3 * pdf[9] - 1)) * 100 << endl;
    // else
    //   cout << ev * 100 << endl;
    cout << ev * 100 << endl;
    
  }
  catch (char const* exc){
    cout << exc << endl;
  }
}



#include "Game.cpp"
#include <cstdlib>

#include <algorithm>

template <typename T>
int maxIndex(vector<T>& A){
  T curMax = A[0];
  int curIndex = 0;
  for (uint i=1; i<A.size(); i++){
    if (A[i] > curMax){
      curMax = A[i];
      curIndex = i;
    }
  }
  return curIndex;
}

HandMatrix<GameAction> findBestActions(int numDecks, double penetration, int numHands, int setCount){

  Deck deck = Deck(numDecks);
  Dealer dealer = Dealer();

  vector<GameAction> actions = {GameAction::HIT, GameAction::STAND, GameAction::DOUBLE,
				GameAction::SPLIT, GameAction::SURRENDER};

  HandMatrix<double> hitMatrix = HandMatrix<double>();
  HandMatrix<double> standMatrix = HandMatrix<double>();
  HandMatrix<double> doubleMatrix = HandMatrix<double>();
  HandMatrix<double> splitMatrix = HandMatrix<double>();
  HandMatrix<double> surrenderMatrix = HandMatrix<double>();
  vector<HandMatrix<double> > matrices = {hitMatrix, standMatrix, doubleMatrix, splitMatrix, surrenderMatrix};

  vector<CountResult> countResults;
  
  for (int i=0; i<5; i++){
    ActionPlayer player = ActionPlayer(actions[i]);
    Game game = Game(&deck, &dealer, &player);
    game.playHands(numHands, penetration, matrices[i], countResults, false, setCount);
  }

  HandMatrix<GameAction> actionMatrix = HandMatrix<GameAction>();
  for (int i=0; i<40; i++){
    for (int j=0; j<10; j++){
      vector<double> results;
      for (int k=0; k<5; k++)
	results.push_back(matrices[k].data[i][j]);
      int bestIndex = maxIndex(results);
      GameAction bestAction;
      if (matrices[bestIndex].count[i][j] > 0)
	bestAction = actions[bestIndex];
      else
	bestAction = GameAction::NA;
      actionMatrix.data[i][j] = bestAction;
    }
  }

  return actionMatrix;
}

void printActions(HandMatrix<GameAction> hm){
  vector<int> results = vector<int>(6, 0);
  vector<string> names = {"NA", "HIT", "STAND", "DOUBLE", "SPLIT", "SURRENDER"};
  for (int i=0; i<40; i++)
    for (int j=0; j<10; j++)
      results[(int)hm.data[i][j]]++;
  for (int i=0; i<6; i++)
    cout << names[i] << " : " << results[i] << endl;
}

int numDifferentActions(HandMatrix<GameAction> hm1, HandMatrix<GameAction> hm2){
  int numDifferent = 0;
  for (int i=0; i<40; i++)
    for (int j=0; j<10; j++)
      if (hm1.data[i][j] != hm2.data[i][j])
	numDifferent++;
  return numDifferent;
}

int main(int argc, char** argv){

  int numDecks = 8;
  double penetration = .875;
  long int numHands = 10000000;

  HandMatrix<GameAction> actionMatrix = findBestActions(numDecks, penetration, numHands, 0);
  printActions(actionMatrix);

  HandMatrix<GameAction> highCountMatrix = findBestActions(numDecks, penetration, numHands, 0);
  printActions(highCountMatrix);
  
  cout << "num different actions : " << numDifferentActions(actionMatrix, highCountMatrix) << endl;

  Deck deck = Deck(numDecks);
  Dealer dealer = Dealer();
  //Dealer player = Dealer();
  //ActionPlayer player = ActionPlayer(GameAction::HIT);
  //MatrixPlayer player = MatrixPlayer(actionMatrix, highCountMatrix, numDecks);
  MatrixPlayer player = MatrixPlayer(actionMatrix, actionMatrix, numDecks);
 
  HandMatrix<double> hm = HandMatrix<double>();
  
  Game game = Game(&deck, &dealer, &player);
  
  struct timespec start, finish;
  double elapsed;
  
  clock_gettime(CLOCK_MONOTONIC, &start);

  vector<CountResult> countResults;

  game.playHands(10*numHands, penetration, hm, countResults, true, 0*numDecks);

  for (int i=-30; i<31; i++)
    printCountResult(i, countResults);

  clock_gettime(CLOCK_MONOTONIC, &finish);

  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  printf("Actual elapsed time is %f seconds\n", elapsed);
  
  return 0;
}

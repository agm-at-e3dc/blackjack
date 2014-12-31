#ifndef HANDCPP
#define HANDCPP

struct Hand{
  
  vector<Card> cards;

  Hand() : cards(vector<Card>()) {}
  Hand(vector<Card> cards) : cards(cards) {}

  int size(){
    return (int) cards.size();
  }

  bool hasAce(){
    for (Card& c : cards)
      if (c.name == 'A')
	return true;
    return false;
  }
  
  int value(){
    int tot = 0;
    for (Card& c : cards)
      tot += c.value;
    return tot;
  }

  bool isBlackjack(){
    if (cards[0].name == 'A' && cards[1].value == 10)
      return true;
    if (cards[1].name == 'A' && cards[0].value == 10)
      return true;
    return false;
  }

  void addCard(Card& c){
    cards.push_back(c);
  }

  // returns the best way of valuing the hand, i.e. the largest value not over 21.
  // if all values are over 21, returns the smallest value.
  int bestValue(){
    int baseValue = value();
    if (hasAce()){
      if (baseValue > 11)
	return baseValue;
      else
	return baseValue + 10;
    }
    else
      return baseValue;
  }
  
};

ostream& operator<<(ostream& out, Hand& h){
  for (int i=0; i<h.size(); i++)
    out << h.cards[i] << " ";
  return out;
}

struct HandState{

  int value;
  bool soft;

  HandState() : value(0), soft(false) {}
  HandState(int value, bool soft) : value(value), soft(soft) {}

  int toInt(){
    if (soft)
      return value - 2 + 20;
    else
      return value - 2;
  }

};

template <typename T>
struct HandMatrix{

  T data[40][10];
  int count[40][10];
  
  HandMatrix(){
    for (int i=0; i<40; i++){
      for (int j=0; j<10; j++){
	data[i][j] = T();
	count[i][j] = 0;
      }
    }
  }

  void update(HandState playerState, int dealerUpVal, T change){
    data[playerState.toInt()][dealerUpVal-1] += change;
    count[playerState.toInt()][dealerUpVal-1]++;
  }

  void set(HandState playerState, int dealerUpVal, T val){
    data[playerState.toInt()][dealerUpVal-1] = val;
    count[playerState.toInt()][dealerUpVal-1] = 0;
  }

  T getData(HandState playerState, int dealerUpVal){
    return data[playerState.toInt()][dealerUpVal-1];
  }

  int getCount(HandState playerState, int dealerUpVal){
    return count[playerState.toInt()][dealerUpVal-1];
  }

  T totalData(){
    T tot = T();
    for (int i=0; i<40; i++)
      for (int j=0; j<10; j++)
	tot += data[i][j];
    return tot; 
  }

  T totalData(HandState hs){
    T tot = T();
    for (int j=0; j<10; j++)
      tot += data[hs.toInt()][j];
    return tot;
  }

  T totalData(int dealerUpCard){
    T tot = T();
    for (int i=0; i<40; i++)
      tot += data[i][dealerUpCard-1];
    return tot;
  }

  int totalCount(){
    int tot = 0;
    for (int i=0; i<40; i++)
      for (int j=0; j<10; j++)
	tot += count[i][j];
    return tot; 
  }

  int totalCount(HandState hs){
    int tot = 0;
    for (int j=0; j<10; j++)
      tot += count[hs.toInt()][j];
    return tot;
  }

  int totalCount(int dealerUpCard){
    int tot = 0;
    for (int i=0; i<40; i++)
      tot += count[i][dealerUpCard-1];
    return tot;
  }

  T percentReturn(){
    return 100 * totalData() / totalCount();
  }

  T percentReturn(HandState hs){
    return 100 * totalData(hs) / totalCount(hs);
  }

  T percentReturn(int dealerUpCard){
    return 100 * totalData(dealerUpCard) / totalCount(dealerUpCard);
  }

};

ostream& operator<<(ostream& out, HandState& hs){
  out << hs.value;
  if (hs.soft)
    out << 'S';
  else
    out << 'H';
  return out;
}


#endif

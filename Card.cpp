#ifndef CARDCPP
#define CARDCPP

using namespace std;

#include <iostream>

struct Card{

  char name;
  int value;
  
  Card() : name('n'), value(0) {}
  Card(char name, int value) : name(name), value(value) {}

};

ostream& operator<<(ostream& out, Card& c){
  out << c.name;
  return out;
}

Card REGULAR_CARDS[] = {Card('A', 1),
			Card('2', 2),
			Card('3', 3),
			Card('4', 4),
			Card('5', 5),
			Card('6', 6),
			Card('7', 7),
			Card('8', 8),
			Card('9', 9),
			Card('T', 10),
			Card('J', 10),
			Card('Q', 10),
			Card('K', 10)};

Card COMPACT_CARDS[] = {Card('2', 2),
			Card('3', 3),
			Card('4', 4),
			Card('5', 5),
			Card('6', 6),
			Card('7', 7),
			Card('8', 8),
			Card('9', 9),
			Card('T', 10),
			Card('A', 1)};


#endif

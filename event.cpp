#include <iostream>
#include "event.hpp"

using namespace std;

Event::Event (bool type, int time, Event* next, Event* prev) 
  : type_(type), time_(time), next_(next), prev_(prev) {
}

Event::~Event () {
  // destructor will get called multiple times if program makes copies
  // it will also be destroyed when out of scope using this syntax: 
  // Event e(1, 1, NULL, NULL);
}

void Event::print () {
  cout << "Event(";
  cout << type_ << ", ";
  cout << time_ << ", ";
  cout << next_ << ", ";
  cout << prev_;
  cout << ")";
}


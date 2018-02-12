#include <iostream>
#include "event.hpp"

Event::Event (bool type, int time, Event* next, Event* prev) 
  : type_(type), time_(time), next_(next), prev_(prev) {
  std::cout << "Event::Event()" << std::endl;
}

Event::~Event () {
  std::cout << "Event::~Event()" << std::endl;
}


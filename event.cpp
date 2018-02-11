#include <iostream>
#include "event.hpp"

Event::Event (int timestamp) : timestamp_(timestamp) {
  std::cout << "Event::Event()" << std::endl;
}

Event::~Event () {
  std::cout << "Event::~Event()" << std::endl;
}

int Event::get_timestamp () {
  return timestamp_;
}

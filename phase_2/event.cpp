#include <iostream>
#include <string>
#include "event.hpp"

using namespace std;

Event::Event (char type, double time) : type_(type), time_(time) {}
Event::~Event () {}
string Event::details () { 
  string s = "Event(";
  s += to_string(type_);
  s += ", ";
  s += to_string(time_);
  s += ")";
  return s; 
}


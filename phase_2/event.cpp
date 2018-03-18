#include <iostream>
#include <string>
#include "event.hpp"

using namespace std;

Event::Event (char type, double time, int hn) {
  type_ = type;
  time_ = time;
  host_num_ = hn;
}

Event::~Event () {}
string Event::details () { 
  string s = "Event(";
  s += to_string(type_);
  s += ", ";
  s += to_string(time_);
  s += ", ";
  s += to_string(host_num_);
  s += ")";
  return s; 
}


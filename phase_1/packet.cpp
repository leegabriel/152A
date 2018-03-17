#include <iostream>
#include <string>
#include "packet.hpp"

using namespace std;

Packet::Packet (double st) : service_time_(st) {}
Packet::~Packet () {}
string Packet::details () { 
  string s = "Packet(";
  s += to_string(service_time_);
  s += ")"; 
  return s;
}


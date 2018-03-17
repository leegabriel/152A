#include <iostream>
#include <string>
#include "packet.hpp"

using namespace std;

Packet::Packet (double st, int ps) : service_time_(st), packet_size_(ps) {}
Packet::~Packet () {}
string Packet::details () { 
  string s = "Packet(";
  s += to_string(service_time_);
  s += ", ";
  s += to_string(packet_size_);
  s += ")"; 
  return s;
}


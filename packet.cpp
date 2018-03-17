#include <iostream>
#include <string>
#include "packet.hpp"

using namespace std;

Packet::Packet (double st) : service_time_(st) {
	packet_size_ = (rand() % (1518 - 64)) + 64;
}
Packet::~Packet () {}
string Packet::details () { 
  string s = "Packet(";
  s += to_string(service_time_) + "t";
  s += ", " + to_string(packet_size_) + "s";
  s += ")"; 
  return s;
}


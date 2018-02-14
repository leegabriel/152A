#include <iostream>
#include "packet.hpp"

using namespace std;

Packet::Packet (int st) : service_time_(st) {}

Packet::~Packet () {}

void Packet::print () {
  cout << "Packet(" << service_time_ << ")";
}


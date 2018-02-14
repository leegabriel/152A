#include <iostream>
#include "packet.hpp"

using namespace std;

Packet::Packet (int time) : time_(time) {}

Packet::~Packet () {}

void Packet::print () {
  cout << "Packet(" << time_ << ")";
}


#include <iostream>
#include <string>
#include "packet.hpp"

using namespace std;

Packet::Packet (double st, double at, int ps, int dhn) {
  service_time_ = st;
  arrival_time_ = at; // absolute arrival time
  packet_size_ = ps;
  dest_host_num_ = dhn;
}

Packet::~Packet () {}
string Packet::details () { 
  string s = "Packet(";
  s += to_string(service_time_);
  s += ", ";
  s += to_string(arrival_time_);
  s += ", ";
  s += to_string(packet_size_);
  s += ", ";
  s += to_string(dest_host_num_);
  s += ")"; 
  return s;
}


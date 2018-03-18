#include <iostream>
#include <string>
#include <queue>
#include "host.hpp"
#include "packet.hpp"

using namespace std;

Host::Host (int n) {
  num_ = n;
  last_token_time_ = 0;
}
Host::~Host () {}
string Host::details () { 
  string s = "Host(";
  s += to_string(num_);
  // s += ", ";
  // s += to_string(last_token_time_);
  s += ", ";
  s += to_string(buffer_.size()) + " packets";
  s += ")"; 
  return s;
}

void Host::push_packet(Packet packet) {
  buffer_.push(packet);
}

queue<Packet> Host::get_buffer() {
  return buffer_;
}

void Host::empty_buffer() {
  while (!buffer_.empty()) {
    buffer_.pop();
  }
}
#ifndef HOST_HPP
#define HOST_HPP

#include <iostream>
#include <string>
#include <queue>
#include <cstdlib>
#include "packet.hpp"

using namespace std;

class Host {
  private:
    int num_;
    int has_token_; // 1 = has token, 0 = no token
    double last_token_time_;
    queue<Packet> buffer_; 
  public:
    Host (int n);
    ~Host ();
    int get_num () { return num_; }
    int has_token() { return has_token_; }
    int get_size () { return buffer_.size(); }
    double get_last_token_time() { return last_token_time_; }
    queue<Packet> get_buffer();
    void set_last_token_time (double time) { last_token_time_ = time; }
    void push_packet(Packet packet);
    void empty_buffer();
    string details ();
};

#endif

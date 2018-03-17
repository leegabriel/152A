#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Packet {
  private:
    double service_time_; // intrinsic service time
	int packet_size_;
  public:
    Packet (double st);
    ~Packet ();
    double get_service_time () { return service_time_; }
	int get_packet_size() { return packet_size_; }
    string details ();
};

#endif

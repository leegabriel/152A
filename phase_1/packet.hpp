#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include <string>

using namespace std;

class Packet {
  private:
    double service_time_; // intrinsic service time
  public:
    Packet (double st);
    ~Packet ();
    double get_service_time () { return service_time_; }
    string details ();
};

#endif

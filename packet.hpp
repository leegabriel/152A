#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>

class Packet {
  private:
    int service_time_; // the packet's intrinsic service time

  public:
    Packet (int st);
    ~Packet ();

    int get_service_time () { return service_time_; }

    void print ();
};

#endif

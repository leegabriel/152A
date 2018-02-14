#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>

class Packet {
  private:
    int time_; // when packet arrives to xmitter or server finishes xmitting

  public:
    Packet (int time);
    ~Packet ();

    void print ();
};

#endif

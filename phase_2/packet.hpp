#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Packet {
  private:
    double service_time_; // intrinsic service time (queuing delay)
    double arrival_time_; // intrinsic service time (queuing delay)
    int packet_size_; // uniformly distributed in [64, 1518] bytes
    int dest_host_num_; // randomly chosen
  public:
    Packet (double st, double at, int ps, int dhn);
    ~Packet ();
    double get_service_time () { return service_time_; }
    double get_arrival_time () { return arrival_time_; }
    int get_packet_size () { return packet_size_; }
    int get_dest_host_num () { return dest_host_num_; }
    string details ();
};

#endif

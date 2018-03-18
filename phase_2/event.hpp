#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>
#include <string>

using namespace std;

class Event {
  private: 
    char type_; // arrival = 'A', token = 'T'
    double time_; // absolute system time when event occurs
    int host_num_;
  public:
    Event(char type, double time, int hn);
    Event();
    ~Event();
    char get_type() { return type_; }
    double get_time () const { return time_; } // needs to be const for operator
    int get_host_num () { return host_num_; }
    void set_type (char type) { type_ = type; }
    void set_time (double time) { time_ = time; }
    void set_host_num (int hn) { host_num_ = hn; }
    inline bool operator > (const Event& rhs) const { return time_ > rhs.get_time(); }
    string details();
};

#endif

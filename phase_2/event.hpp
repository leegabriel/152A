#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>
#include <string>

using namespace std;

class Event {
  private: // will be initialized in this order
    char type_; // arrival = 'A', token = 'T'
    double time_; // absolute system time when event occurs
  public:
    Event(char type, double time);
    ~Event();
    char get_type() { return type_; }
    double get_time() const { return time_; } // needs to be const for operator
    void set_type(char type) { type_ = type; }
    void set_time(double time) { time_ = time; }
    inline bool operator > (const Event& rhs) const { return time_ > rhs.get_time(); }
    string details();
};

#endif

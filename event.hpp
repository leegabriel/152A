#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>

class Event {
  private:
    // will be initialized in this order
    bool type_; // arrival = 1, departure = 0
    int time_; // when packet arrives to xmitter or server finishes xmitting
    Event* next_;
    Event* prev_;

  public:
    Event (bool type, int time, Event* next, Event* prev);
    ~Event ();

    // getters

    bool get_type () { return type_; }
    int get_time () { return time_; }
    Event* get_next () { return next_; }
    Event* get_prev () { return prev_; }

    // setters

    void set_type (bool type) { type_ = type; }
    void set_time (int time) { time_ = time; }
    void set_next (Event* next) { next_ = next; }
    void set_prev (Event* prev) { prev_ = prev; }
};

#endif
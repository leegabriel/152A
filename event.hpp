#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>

class Event {
  private:
    // will be initialized in this order
    bool type_; // arrival = 1, departure = 0
    int time_; // absolute time when packet arrives/departs from system
    Event* next_;
    Event* prev_;

  public:
    Event (bool type, int time, Event* next, Event* prev);
    ~Event ();

    // getters

    bool get_type () { return type_; }
    int get_time () const { return time_; } // needs to be const for operators
    Event* get_next () { return next_; }
    Event* get_prev () { return prev_; }

    // setters

    void set_type (bool type) { type_ = type; }
    void set_time (int time) { time_ = time; }
    void set_next (Event* next) { next_ = next; }
    void set_prev (Event* prev) { prev_ = prev; }

    // operators

    inline bool operator < (const Event& rhs) const { return time_ < rhs.get_time(); }
    inline bool operator > (const Event& rhs) const { return time_ > rhs.get_time(); }
    inline bool operator <= (const Event& rhs) const { return time_ <= rhs.get_time(); }
    inline bool operator >= (const Event& rhs) const { return time_ >= rhs.get_time(); }
    inline bool operator == (const Event& rhs) const { return time_ == rhs.get_time(); }

    void print ();
};

#endif

#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>

class Event {
  private:
    int timestamp_;

  public:
    Event (int timestamp);
    ~Event ();

    int get_timestamp ();
};

#endif
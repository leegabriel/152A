#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>
#include <string>

using namespace std;

class Event {
private: // will be initialized in this order
	bool type_; // arrival = 1, departure = 0
	double time_; // absolute time when packet arrives/departs from system
public:
	Event(bool type, double time);
	~Event();
	bool get_type() { return type_; }
	double get_time() const { return time_; } // needs to be const for operator
	void set_type(bool type) { type_ = type; }
	void set_time(double time) { time_ = time; }
	inline bool operator > (const Event& rhs) const { return time_ > rhs.get_time(); }
	string details();
};

#endif

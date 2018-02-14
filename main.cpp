#include <iostream>
#include <queue>
#include <functional>
#include <stdlib.h>
#include <math.h>
#include <random>
#include "event.hpp"
#include "packet.hpp"

using namespace std;

const int MAX_BUFFER_SIZE = 10;
const float LAMBDA = 0.2;
const float MU = 1.0;

int g_time;
int g_length; // total number of packets in system (buffer + server)

priority_queue<Event, vector<Event>, greater<Event>> gel;
queue<Packet> buffer;

void print_buffer () {
  queue<Packet> tmp(buffer);
  cout << "Buffer contents: {";
  Packet p = tmp.front();
  p.print();
  tmp.pop();
  while (!tmp.empty()) {
    Packet p = tmp.front();
    cout << ", ";
    p.print();
    tmp.pop();
  }
  cout << "}" << endl;
}

int negative_exponential (float rate) {
  // TODO: implement properly
  mt19937 rng;
  rng.seed(random_device()());
  uniform_int_distribution<mt19937::result_type> dist100(0,100); 
  return dist100(rng);
}

void init () {
  cout << "Initializing" << endl;
  g_time = 0;
  g_length = 0; // set to >= 1 to make server busy initially
  int t = g_time + negative_exponential(LAMBDA);
  Event first(1, t, NULL, NULL); // first arrival event
  cout << "First event: ";
  first.print(); 
  cout << endl;
  gel.push(first);
  cout << "Done initializing" << "\n\n";
}

void process_arrival_event (Event a) {
  // advance simulation time
  int save = g_time;
  g_time = a.get_time();
  cout << "Advancing time from ";
  cout << save << " to " << g_time << endl;

  // create new packet, generate its intrinsic service time
  Packet p(negative_exponential(MU)); 

  // create next arrival event, generate its absolute arrival time
  Event next_arrival(1, g_time + negative_exponential(LAMBDA), NULL, NULL);
  cout << "Generated next arrival event: ";
  next_arrival.print(); 
  cout << endl;
  gel.push(next_arrival);

  // server is free
  if (g_length == 0) {
    int service_time = p.get_service_time();
    Event depart_event(0, g_time + service_time, NULL, NULL);
    cout << "Generated departure event: ";
    depart_event.print(); 
    cout << endl;
    gel.push(depart_event);
  }
  // server is busy
  else if (g_length > 0) {
    cout << "Server busy. ";
    if (g_length - 1 < MAX_BUFFER_SIZE) {
      // buffer not full, queue the packet
      cout << "Buffer not full, queuing packet.";
      buffer.push(p);
      g_length++;
    }
    else {
      cout << "Buffer is full, dropping packet.";
    }
  }
  cout << "\n\n"; // done processing
}

void process_departure_event (Event d) {
  int save = g_time;
  g_time = d.get_time();
  cout << "Advancing time from ";
  cout << save << " to " << g_time << endl;
  // if buffer not empty
  if (g_length > 0) {
    Packet p = buffer.front(); 
    buffer.pop(); // dequeue packet
    g_length--;
    // next depart time is current time + time to transmit this dequeued packet
    Event depart_event(0, g_time + p.get_service_time(), NULL, NULL);
    cout << "Generated departure event: ";
    depart_event.print(); 
    cout << endl;
    gel.push(depart_event);
  }
}

void update_statistics () {
  // TODO: implement properly
  cout << "Update statistics" << endl;
}

void output_statistics () {
  // TODO: implement properly
  cout << "Output statistics" << endl;
}

void test () {
  Event e1(1, 23, NULL, NULL);
  Event e2(0, 24, NULL, NULL);
  Event e3(1, 25, NULL, NULL);

  gel.push(e1);
  gel.push(e2);
  gel.push(e3);

  cout << "Min heap, popped one by one: " << endl;

  while (!gel.empty()) {
    Event e = gel.top();
    e.print();
    cout << endl;
    gel.pop();
  }
}

int main (int argc, char* argv[]) {
  init();
  cout << "Going through global event list" << "\n\n";
  int i = 0;
  while (i < 50) {
    Event e = gel.top();
    gel.pop();
    cout << "Processing ";
    e.print();
    cout << ". ";
    // arrival = 1, departure = 0
    if (e.get_type()) {
      process_arrival_event(e);
    }
    else {
      process_departure_event(e);
    }
    i++;
    cout << buffer.size() << endl;
  }
  // test();
  cout << "\n\nFinished going through global event list" << "\n\n";
  output_statistics();
  return 0;
}

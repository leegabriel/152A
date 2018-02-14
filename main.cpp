#include <iostream>
#include <queue>
#include <functional>
#include <stdlib.h>
#include <math.h>
#include <random>
#include "event.hpp"

#define MAX_BUFFER_SIZE 20
#define LAMBDA 0.2
#define MU 1.0

using namespace std;

int g_time;
int g_length;

priority_queue<Event, vector<Event>, greater<Event>> gel;
queue<Event> buffer();

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
  g_length = 0;
  int t = g_time + negative_exponential(LAMBDA);
  Event first(1, t , NULL, NULL); // first arrival event
  cout << "First event: ";
  first.print(); 
  cout << endl;
  gel.push(first);
  cout << "Done initializing" << endl;
}

void process_arrival_event (Event a) {
  int save = g_time;
  g_time = a.get_time();
  cout << "Processing arrival event. Advancing time from ";
  cout << save << " to " << g_time << endl;
  Event next_arrival(1, g_time + negative_exponential(LAMBDA), NULL, NULL);
  cout << "Generated next event: ";
  next_arrival.print(); 
  cout << endl;
  gel.push(next_arrival);

  // // server is free
  // if (g_length == 0) {
  //   int service_time = a_new->get_time();
  //   Event* depart_event = (0, g_time + service_time, NULL, NULL);
  //   gel.push(depart_event);
  // }
  // // server is busy
  // else if (g_length > 0) {
  //   if (g_length - 1 < MAX_BUFFER_SIZE) {

  //   }
  //   else {
  //     // buffer is full
  //     cout << "Packet drop" << endl;
  //   }
  //   g_length++;
  //   update_statistics();
  // }
}

void process_departure_event (Event d) {
  cout << "Processing departure event. Time: " << g_time << endl;
  g_time = d.get_time();
  // if (g_length > 0) {

  // }
  // update_statistics();
  // g_length--; // packet departure
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
    cout << "{ ";
    cout << e.get_type() << " ";
    cout << e.get_time() << " ";
    cout << e.get_next() << " ";
    cout << e.get_prev() << " ";
    cout << "}" << endl;
    gel.pop();
  }
}

int main (int argc, char* argv[]) {
  init();
  cout << "Going through global event list" << endl;
  int i = 0;
  while (i < 10) {
    Event e = gel.top();
    gel.pop();
    // arrival = 1, departure = 0
    if (e.get_type()) {
      process_arrival_event(e);
    }
    else {
      process_departure_event(e);
    }
    i++;
  }
  // test();
  cout << "Finished going through global event list" << endl;
  output_statistics();
  return 0;
}

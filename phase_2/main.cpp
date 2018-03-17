#include <iostream>
#include <fstream>
#include <queue>
#include <functional>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <limits.h>
#include "event.hpp"
#include "packet.hpp"

using namespace std;

//
// constants
//

const int NUM_EVENTS = 100000; // number of events to process for simulation
const int MAX_BUFF_SIZE = INT_MAX; // maximum buffer size
const double LAMBDA = 0.9; // 0.01, 0.05, 0.1, 0.2, 0.3, 0.5, 0.6, 0.7, 0.8, 0.9

const int NUM_HOSTS = 25; // number of hosts in Token Ring LAN
const double LINK_PROP_DELAY = 0.01; // 10 microseconds = 0.01 milliseconds
const int TRANSMISSION_RATE = 100; // 100 Mbps, rates are always in bits

//
// simulation variables
//

double g_time; // absolute/total system time
priority_queue<Event, vector<Event>, greater<Event>> gel; // global event list

//
// statistical variables
//

int g_bytes_transmitted_sum; // number of bytes successfully transmitted
double g_queuing_delay_sum; // each host 
double g_transmission_delay_sum; // each host 
double g_propagation_delay_sum; // among hosts

//
// functions
//

double negative_exponential (double rate) {
  mt19937 rng;
  rng.seed(random_device()());
  uniform_real_distribution<double> distribution(0.0, 1.0);
  double u = distribution(rng);
  return (((-1 / rate) * log(1 - u)));
}

int generate_packet_size () {
  return (rand() % (1518 - 64)) + 64;
}

int choose_next_host (int src) {
  int next = rand() % NUM_HOSTS;
  if (next == src) { return choose_next_host (src); } 
  return next;
}

void advance_system_time (Event e) {
  // TODO: implement
}

Event generate_event (char type) {
  // TODO: implement
}

void init (ofstream& fs) {
  cout << "Initializing" << endl;

  g_bytes_transmitted_sum = 0, g_queuing_delay_sum = 0.0, 
    g_transmission_delay_sum = 0.0, g_propagation_delay_sum = 0.0;

  cout << "Done initializing" << "\n\n";
  fs << "N/A" << endl;
}

void process_arrival_event (Event a) {
  // TODO: implement
}

void process_token_event (Event t) {
  // TODO: implement
}

void update_statistics () {
  // TODO: implement
}

void output_statistics (ofstream& fs) {
  cout << "----------" << endl;
  cout << "Statistics" << endl;
  cout << "----------" << endl;
  cout << "Total simulation time: " << g_time << endl;
  cout << "Bytes transmitted: " << g_bytes_transmitted_sum << endl;
  cout << "Throughput: " << ((double) g_bytes_transmitted_sum / g_time) << endl;
  cout << "Total queuing delay: " << g_queuing_delay_sum << endl;
  cout << "Total transmission delay: " << g_transmission_delay_sum << endl;
  cout << "Total propagation delay: " << g_propagation_delay_sum << endl;
  cout << "Average packet delay: " <<  
    ((g_queuing_delay_sum + g_transmission_delay_sum
      + g_transmission_delay_sum) / g_time) << endl;
  fs << "N/A" << endl;
}

//
// main function
//

int main (int argc, char* argv[]) {
  ofstream fs("output.csv");
  if (!fs) {
    cerr << "Cannot open file" << endl;
    return 1;
  }
  init(fs);
  for (int i = 0; i < NUM_EVENTS; i++) {
    // TODO: implement
  }
  output_statistics(fs);
  fs.close();
  return 0;
}

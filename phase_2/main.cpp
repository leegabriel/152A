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

const int NUM_EVENTS = 100000; // number of events to process 
const int NO_CUSTOM = -1; 
const int MAX_BUFFER_SIZE = INT_MAX; // maximum buffer size
const double LAMBDA = 0.9; // arrival rate in pkts/sec
const double MU = 1.0; // departure rate in pkts/sec
int num_hosts = 25;

// simulation variables

double g_time; // absolute system time
int g_length; // total number of packets in system (buffer + server)
priority_queue<Event, vector<Event>, greater<Event>> gel; // global event list
queue<Packet> buffer; // queue for link processor

// statistical variables

double g_queue_length_sum; // sum of queue lengths 
double g_free_time_sum;
int g_pkts_dropped; 
double g_time_difference;

double negative_exponential (double rate) {
  mt19937 rng;
  rng.seed(random_device()());
  uniform_real_distribution<double> distribution(0.0, 1.0);
  double u = distribution(rng);
  return (((-1 / rate) * log(1 - u)));
}

//Randomly chooses the next host
int chooseNextHost(int src) {
  int next = rand() % num_hosts;
  if (next == src) {
	  return chooseNextHost(src);
  }
  else {
	  return next;
  }
}

void advance_system_time (Event e) {
  double old = g_time;
  g_time = e.get_time(); 
  g_time_difference = g_time - old;
  cout << "Advancing time from ";
  cout << old << " to " << g_time << endl;
}

Event generate_event (bool type, double custom) {
  double ist; // intrinsic service time
  if (custom != NO_CUSTOM) { ist = custom; } // user-defined ist
  else { ist = type ? negative_exponential(LAMBDA) : negative_exponential(MU); }
  Event e(type, g_time + ist);
  cout << "New event: " << e.details() << endl;
  return e;
}

void init (ofstream& fs) {
  //TODO: User prompts
  cout << "Set the number of hosts: ";
  cout << "Set Lambda: ";

  cout << "Initializing" << endl;
  //TODO: Change Init sequence
  g_time = 0.0, g_length = 0, g_queue_length_sum = 0.0,
    g_free_time_sum = 0.0, g_pkts_dropped = 0, g_time_difference = 0;
  Event first = generate_event(1, NO_CUSTOM); // generate first arrival event
  gel.push(first); // push first arrival event to start simulation
  cout << "Done initializing" << "\n\n";
  fs << "lambda" << ",";
<<<<<<< HEAD:main.cpp
  fs << "Number of packets dropped" << endl;
=======
  // fs << "mu" << ",";
  // fs << "max buffer size" << ",";
  // fs << "Sum of queue lengths" << ",";
  // fs << "Total server busy time" << ",";
  // fs << "Total simulation time" << ",";
  fs << "Utilization" << ",";
  fs << "Mean queue length" << endl;
  // fs << "Number of packets dropped" << endl;
>>>>>>> a7d38822d3f16e0c8980b4939f85dcec8e7938a2:phase_1/main.cpp
}

void process_arrival_event (Event a) {
  advance_system_time(a);  //update global timer
  Packet packet(negative_exponential(MU)); // create new packet

  Event next_arrival = generate_event(1, NO_CUSTOM); 
  gel.push(next_arrival);

  if (g_length == 0) { // server free
    cout << "Server free, will now process this packet" << endl;
    g_free_time_sum += g_time_difference; // record server free time
    Event depart_event = generate_event(0, packet.get_service_time());
    gel.push(depart_event);
    g_length = 1; // 1 packet in system, being processed in server
  }
  else if (g_length > 0) { // server busy
    cout << "Server busy. ";
    if (g_length - 1 < MAX_BUFFER_SIZE) { // buffer not full
      cout << "Buffer NOT full, queuing packet" << endl;
      buffer.push(packet); 
      g_length++; // 1 more packet in system (in buffer)
    }
    else { // buffer full
      cout << "Buffer full, dropping packet" << endl;
      g_pkts_dropped++;
    }
  }
}

//TODO: Debate on necessity
void process_departure_event (Event d) {
  advance_system_time(d);
  g_length--; // packet is out of system now
  cout << "Server done processing packet" << endl;
  if (g_length > 0) { // buffer not empty
    cout << "Buffer NOT empty, processing next packet" << endl;
    Packet packet = buffer.front(); 
    buffer.pop(); // dequeue packet
    Event depart_event = generate_event(0, packet.get_service_time());
    gel.push(depart_event);
  } 
  else { // buffer empty
    cout << "Buffer empty, taking no action" << endl; 
  }
}

void update_statistics () {
	cout << "Total server free time: " << g_free_time_sum << endl;
  cout << "Updating statistics" << "\n\n";
  g_queue_length_sum += buffer.size() * g_time_difference;
}

void output_statistics (ofstream& fs) {
  //TODO: Change Output Statistics
  cout << "----------" << endl;
  cout << "Statistics" << endl;
  cout << "----------" << endl;
  cout << "Sum of queue lengths: " << g_queue_length_sum << endl; 
  cout << "Total server busy time: " << (g_time - g_free_time_sum) << endl;
  cout << "Total simulation time: " << g_time << endl;
  cout << "Utilization: " << 
    ((double) ((g_time - g_free_time_sum)) / (double) g_time) << endl;
  cout << "Mean queue length: " << 
    ((double) g_queue_length_sum / (double) g_time) << endl;
  cout << "Number of packets dropped: " << g_pkts_dropped << endl;
  fs << LAMBDA << ",";
  // fs << MU << ",";
  // fs << MAX_BUFFER_SIZE << ",";
  // fs << g_queue_length_sum << ","; 
  // fs << g_time - g_free_time_sum << ","; 
  // fs << g_time << ","; 
  fs << ((double) (g_time - g_free_time_sum) / (double) g_time) << ","; 
  fs << ((double) g_queue_length_sum / (double) g_time) << endl; 
  // fs << g_pkts_dropped << endl; 
}

int main (int argc, char* argv[]) {
  ofstream fs("output.csv");
  if (!fs) {
    cerr << "Cannot open file" << endl;
    return 1;
  }
  init(fs);
  //TODO: Change to hosting loop 
  for (int i = 0; i < NUM_EVENTS; i++) {
    Event e = gel.top();
    cout << "Processing " << e.details() << endl;
    gel.pop();
	  if (e.get_type() == 1) { process_arrival_event(e); }
    else { process_departure_event(e); }
    cout << "buffer.size(): " << buffer.size() << endl;
    cout << "g_length: " << g_length << endl;
    update_statistics();
  }
  output_statistics(fs);
  fs.close();
  return 0;
}

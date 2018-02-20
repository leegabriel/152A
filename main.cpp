#include <iostream>
#include <fstream>
#include <queue>
#include <functional>
#include <stdlib.h>
#include <random>
#include <math.h>
#include "event.hpp"
#include "packet.hpp"

using namespace std;

const int NUM_EVENTS = 1000; // number of events to process 
const int NO_CUSTOM = -1; 
const int MAX_BUFFER_SIZE = 10; // maximum buffer size
const double LAMBDA = 0.2; // arrival rate in pkts/sec
const double MU = 1.0; // departure rate in pkts/sec

// simulation variables

double g_time; // absolute system time
int g_length; // total number of packets in system (buffer + server)
priority_queue<Event, vector<Event>, greater<Event>> gel; // global event list
queue<Packet> buffer; // queue for link processor

// statistical variables

double g_queue_length_sum; // sum of queue lengths 
bool g_server_free;
double g_last_free_time;
double g_free_time_sum;
int g_pkts_dropped; 
double g_time_difference;

double negative_exponential (double rate) {
  mt19937 rng;
  rng.seed(random_device()());
  uniform_real_distribution<double> distribution(0.0,1.0);
  double u = distribution(rng);
  return (((-1 / rate) * log(1 - u)));
}

void advance_system_time (Event e) {
  double old = g_time;
  g_time = e.get_time(); 
  g_time_difference = g_time - old;
  cout << "Advancing time from ";
  cout << old << " to " << g_time << endl;
  //cout << "Time Difference " << g_time_difference << endl;
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
  cout << "Initializing" << endl;
  g_time = 0.0, g_length = 0, g_queue_length_sum = 0.0, g_last_free_time = 0.0,
    g_free_time_sum = 0.0, g_pkts_dropped = 0, g_time_difference = 0;
  g_server_free = false; // system not started, starts on first arrival event
  Event first = generate_event(1, NO_CUSTOM); // generate first arrival event
  gel.push(first); // push first arrival event to start simulation
  cout << "Done initializing" << "\n\n";
  fs << "g_time" << ","; 
  fs << "g_length" << ","; 
  fs << "g_server_free" << ","; 
  fs << "g_last_free_time" << ","; 
  fs << "g_free_time_sum" << ","; 
  fs << "g_pkts_dropped" << ","; 
  fs << "g_time_difference" << endl;
}

void process_arrival_event (Event a) {
  advance_system_time(a);  //update global timer
  Packet packet(negative_exponential(MU)); // create new packet

  Event next_arrival = generate_event(1, NO_CUSTOM); 
  gel.push(next_arrival);

  if (g_length == 0) { // server free
    cout << "Server free, will now process this packet" << endl;
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
  g_server_free = false; // server is processing
}

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
    g_server_free = false; // server is processing
  } 
  else { // buffer empty
    cout << "Buffer empty, taking no action" << endl; 
    g_server_free = true; // server is idle
  }
}

void update_statistics (ofstream& fs) {
	cout << "Free Time " << g_free_time_sum << endl;
  cout << "Updating statistics" << "\n\n";
  g_queue_length_sum += buffer.size() * g_time_difference;
  if (g_server_free) {
    //double save = g_last_free_time;
    //g_last_free_time = g_time;
    g_free_time_sum += g_time_difference;
  }
  fs << g_time << ","; 
  fs << g_length << ","; 
  fs << g_server_free << ","; 
  fs << g_last_free_time << ","; 
  fs << g_free_time_sum << ","; 
  fs << g_pkts_dropped << ","; 
  fs << g_time_difference << endl;
}

void output_statistics () {
  cout << "----------" << endl;
  cout << "Statistics" << endl;
  cout << "----------" << endl;
  cout << "Sum of queue lengths: " << g_queue_length_sum << endl; 
  cout << "Total server busy time: " << g_time - g_free_time_sum << endl;
  cout << "Total simulation time: " << g_time << endl;
  cout << "Utilization: " << 
    ((double) (g_time - g_free_time_sum) / (double) g_time) << endl;
  cout << "Mean queue length: " << 
    ((double) g_queue_length_sum / (double) g_time) << endl;
  cout << "Number of packets dropped: " << g_pkts_dropped << endl;
}

int main (int argc, char* argv[]) {
  ofstream fs("output.csv");
  if (!fs) {
    cerr << "Cannot open output file" << endl;
    return 1;
  }
  init(fs);
  for (int i = 0; i < NUM_EVENTS; i++) {
    Event e = gel.top();
    cout << "Processing " << e.details() << endl;
    gel.pop();
	  if (e.get_type() == 1) { process_arrival_event(e); }
    else { process_departure_event(e); }
    cout << "buffer.size(): " << buffer.size() << endl;
    cout << "g_length: " << g_length << endl;
    update_statistics(fs);
  }
  output_statistics();
  fs.close();
  return 0;
}

#include <iostream>
#include <fstream>
#include <queue>
#include <functional>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <limits.h>
#include "event.hpp"
#include "host.hpp"
#include "packet.hpp"

using namespace std;

//
// constants
//

const int NUM_EVENTS = 100000; // number of events to process for simulation
const int MAX_BUFF_SIZE = INT_MAX; // maximum buffer size
const double LAMBDA = 175; // 0.01, 0.05, 0.1, 0.2, 0.3, 0.5, 0.6, 0.7, 0.8, 0.9

const int NUM_HOSTS = 10; // number of hosts in Token Ring LAN
const double LINK_PROP_DELAY = 0.01; // 10 microseconds = 0.01 milliseconds
const int TRANSMIT_RATE = 100; // 100 Mbps, rates are always in bits

//
// simulation variables
//

double g_time; // absolute/total system time
priority_queue<Event, vector<Event>, greater<Event>> gel; // global event list
Host* g_hosts[NUM_HOSTS]; // ring of hosts

//
// statistical variables
//

int g_total_bytes_transmitted; // number of bytes successfully transmitted
double g_total_queuing_delay; // each host 
double g_total_transmit_delay; // each host 
double g_total_prop_delay; // among hosts
int g_total_pkts_transmitted;

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

int generate_packet_size () { return (rand() % (1518 - 64)) + 64; }

// for choosing a random destination host for the packet
int choose_next_host (int src) {
  int next = rand() % NUM_HOSTS;
  if (next == src) { return choose_next_host (src); } 
  return next;
}

void advance_system_time (Event e) {
  double old = g_time;
  g_time = e.get_time(); 
  cout << "Advancing time from ";
  cout << old << " to " << g_time << endl;
}

Event generate_event (char type, int host_num) {
  Event e;
  if (type == 'A') {
    e = Event('A', g_time + negative_exponential(LAMBDA), host_num);
  }
  else if (type == 'T') {
    e = Event('T', g_time + LINK_PROP_DELAY, ((host_num + 1) % NUM_HOSTS) );
  }
  cout << "New event: " << e.details() << endl;
  return e;
}

void init (ofstream& fs) {
  cout << "Initializing" << endl;

  g_total_bytes_transmitted = 0, g_total_queuing_delay = 0.0, 
    g_total_transmit_delay = 0.0, g_total_prop_delay = 0.0,
    g_total_pkts_transmitted = 0;

  // initialize array of Host objects
  for (int i = 0; i < NUM_HOSTS; i++) {
    g_hosts[i] = new Host(i);
  }

  // first arrival event for each host
  for (int i = 0; i < NUM_HOSTS; i++) {
    Event a = generate_event('A', i); 
    gel.push(a); 
  }

  // first token event
  int start_host_num = rand() % NUM_HOSTS - 1;
  Event t = generate_event('T', start_host_num);
  gel.push(t);

  cout << "Done initializing" << "\n\n";
  fs << "lambda,num_hosts,throughput,avg_packet_delay" << endl;
}

void process_arrival_event (Event a) {
  advance_system_time(a); 
  int host_num = a.get_host_num();
  // add packet with random destination host to buffer
  // packet service time is queuing delay
  // packet size is random generated, between 64 and 1518 bytes
  // next host is "randomly" selected, it is any host except itself.
  Packet packet(negative_exponential(LAMBDA), generate_packet_size(), 
    choose_next_host(host_num));

  // add packet to host's buffer
  g_hosts[host_num]->push_packet(packet);

  // generate next arrival event
  Event next_arrival_event = generate_event('A', host_num); 
  gel.push(next_arrival_event); // push next arrival for this host
  cout << "\n";
}

void process_token_event (Event t) {
  advance_system_time(t); 
  int host_num = t.get_host_num();
  Event next_token_event = generate_event('T', host_num); 
  gel.push(next_token_event); // push next token event for this host
  
  int buffer_size = g_hosts[host_num]->get_size();
  if (buffer_size == 0) {
    // pass token to next host
    cout << "No packets in buffer, continuing" << endl;
  }
  else {
    cout << "Packets in buffer, emptying buffer and recording stats" << endl;
    queue<Packet> b = g_hosts[host_num]->get_buffer();
    g_total_pkts_transmitted += b.size();

    // load bytes of each packet into frame while emptying buffer
    while (!b.empty()) {
      Packet p = b.front();
      int packet_size = p.get_packet_size(); // bytes
      g_total_bytes_transmitted += packet_size;

      // xmission delay = ((packet size * 8)/ 100Mbps) * (1000 ms/s) * NUM_HOSTS
      // prop delay = 0.01 * NUM_HOSTS
      // queuing delay = packet.service time

      g_total_transmit_delay += 
        ((packet_size * 8) / (TRANSMIT_RATE * 1000000.0)) * 1000.0 
          * NUM_HOSTS; // ms
      g_total_prop_delay += 0.01 * NUM_HOSTS; // ms
      g_total_queuing_delay += (p.get_service_time()); // ms
      b.pop();
    }
    g_hosts[host_num]->empty_buffer();
  }
  cout << "\n";
}

void output_statistics (ofstream& fs) {
  cout << "----------" << endl;
  cout << "Statistics" << endl;
  cout << "----------" << endl;
  cout << "Arrival rate: " << LAMBDA << endl;
  cout << "Number of hosts: " << NUM_HOSTS << endl;
  cout << "Total simulation time: " << g_time << endl;
  cout << "Bytes transmitted: " << g_total_bytes_transmitted << endl;
  cout << "Packets transmitted: " << g_total_pkts_transmitted << endl;
  cout << "Throughput: " << ((double) g_total_bytes_transmitted / g_time) << endl;
  cout << "Total queuing delay: " << g_total_queuing_delay << endl;
  cout << "Total transmission delay: " << g_total_transmit_delay << endl;
  cout << "Total propagation delay: " << g_total_prop_delay << endl;
  cout << "Average packet delay: " <<  
    ((g_total_queuing_delay + g_total_transmit_delay
      + g_total_transmit_delay) / g_time) << endl;
  fs << LAMBDA << ",";
  fs << NUM_HOSTS << ",";
  fs << ((double) g_total_bytes_transmitted / g_time) << ",";
  fs << ((g_total_queuing_delay + g_total_transmit_delay
      + g_total_transmit_delay) / g_time) << endl;
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
    Event e = gel.top();
    cout << "Processing " << e.details() << endl;
    gel.pop();
    if (e.get_type() == 'A') { 
      process_arrival_event(e); 
    }
    else if (e.get_type() == 'T') { 
      process_token_event(e); 
    }
    else {
      cout << "Invalid event." << endl;
      return 1;
    }
    // update_statistics();
  }
  output_statistics(fs);
  fs.close();

  // deallocate
  for (int i = 0; i < NUM_HOSTS; i++) {
    delete g_hosts[i];
  }

  return 0;
}













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

const int NUM_EVENTS = 5; // number of events to process for simulation
const int MAX_BUFF_SIZE = INT_MAX; // maximum buffer size
const double LAMBDA = 0.9; // 0.01, 0.05, 0.1, 0.2, 0.3, 0.5, 0.6, 0.7, 0.8, 0.9

const int NUM_HOSTS = 25; // number of hosts in Token Ring LAN
const double LINK_PROP_DELAY = 0.01; // 10 microseconds = 0.01 milliseconds
const int TRANSMISSION_RATE = 100; // 100 Mbps, rates are always in bits

// transmission delay = ((packet size * 8)/ 100mbps) * NUM_Hosts
// prop  delay = 0.01 * Num_hosts
// queueing delay = packet.service time

//
// simulation variables
//

double g_time; // absolute/total system time
priority_queue<Event, vector<Event>, greater<Event>> gel; // global event list
Host g_hosts[NUM_HOSTS]; // ring of hosts

// g_frame[0] = token bit, g_frame[1] = dest host num, g_frame[2] = data (bytes)
int g_frame[3]; 

//
// statistical variables
//

int g_bytes_transmitted_sum; // number of bytes successfully transmitted
double g_queuing_delay_sum; // each host 
double g_transmission_delay_sum; // each host 
double g_propagation_delay_sum; // among hosts
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
    e = Event('T', g_time + LINK_PROP_DELAY, (host_num % NUM_HOSTS) + 1);
  }
  cout << "New event: " << e.details() << endl;
}

void init (ofstream& fs) {
  cout << "Initializing" << endl;

  g_bytes_transmitted_sum = 0, g_queuing_delay_sum = 0.0, 
    g_transmission_delay_sum = 0.0, g_propagation_delay_sum = 0.0,
    g_total_pkts_transmitted = 0;

  // initialize array of Host objects
  for (int i = 0; i < NUM_HOSTS; i++) {
    g_hosts[i] = Host(i);
  }

  // first arrival event for each host
  for (int i = 0; i < NUM_HOSTS; i++) {
    Event a = generate_event('A', i); 
    gel.push(a); 
  }

  // first token event
  int start_host_num = rand() % NUM_HOSTS;
  Event t = generate_event('T', start_host_num);
  gel.push(t);

  cout << "Done initializing" << "\n\n";
  fs << "N/A" << endl;
}

void process_arrival_event (Event a) {
  advance_system_time(a); 
  int host_num = a.get_host_num();
  // add packet with random destination host to buffer
  // queuing delay is a time from the negative exponential distribution ??? NO??
  // DOESNT QUEUING DELAY DEPEND ON SIZE?
  // packet size is random generated, it is between 64 and 1518 BYTES
  // next host is "randomly" selected, it is any host except itself.
  Packet packet (negative_exponential(LAMBDA), generate_packet_size(), 
    choose_next_host(host_num));

  // add packet to host's buffer
  g_hosts[host_num].push_packet(packet);

  // generate next arrival event
  Event next_arrival = generate_event('A', host_num); 
  gel.push(next_arrival); // push next arrival for this host
}

void process_token_event (Event t) {
  advance_system_time(t); 
  int host_num = t.get_host_num();
  Host host = g_hosts[host_num]; // host with this particular host number
  Event next_token_event = generate_event('T', host_num); 
  gel.push(next_token_event); // push next token event for this host
  
  // TODO: calculate delay

  int buffer_size = host.get_size();
  if (buffer_size == 0) {
    // pass token to next host
    cout << "Pass token to next host" << endl;
  }
  else {

    // transmission delay = ((packet size * 8)/ 100mbps) * NUM_Hosts
    // prop  delay = 0.01 * Num_hosts
    // queueing delay = packet.service time
    queue<Packet> b = host.get_buffer();
    g_total_pkts_transmitted += b.size();

    // load bytes of each packet into frame while emptying buffer
    while (!b.empty()) {
      Packet p = b.front();
      int packet_size = p.get_packet_size(); // bytes
      g_frame[2] += packet_size;
      g_bytes_transmitted_sum += packet_size;

      g_transmission_delay_sum += ((p.get_packet_size() * 8) / 100000000) * NUM_HOSTS;
      g_propagation_delay_sum += 0.01 * NUM_HOSTS;
      g_queuing_delay_sum += p.get_service_time();
      b.pop();
    }
    g_hosts[host_num].empty_buffer();
  }
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
  cout << "Packets transmitted: " << g_total_pkts_transmitted << endl;
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
    update_statistics();
  }
  output_statistics(fs);
  fs.close();
  return 0;
}













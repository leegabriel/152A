#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include "event.hpp"

using namespace std;

int main (int argc, char* argv[]) {

  Event* e1 = new Event(1, 23, NULL, NULL);
  Event* e2 = new Event(0, 24, NULL, NULL);
  Event* e3 = new Event(1, 25, NULL, NULL);

  priority_queue<Event*, vector<Event*>, greater<Event*> > gel;
  gel.push(e1);
  gel.push(e2);
  gel.push(e3);

  cout << "Min heap, popped one by one: " << endl;
  while (!gel.empty()) {
    Event* e = gel.top();
    cout << e->get_type() << " ";
    cout << e->get_time() << " ";
    cout << e->get_next() << " ";
    cout << e->get_prev() << endl;
    gel.pop();
  }

  delete e1;
  delete e2;
  delete e3;
  return 0;
}

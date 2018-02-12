#include <iostream>
#include "event.hpp"

using namespace std;

int main () {
  Event* e = new Event(1, 23, NULL, NULL);
  cout << e->get_type() << endl;
  cout << e->get_time() << endl;
  cout << e->get_next() << endl;
  cout << e->get_prev() << endl;
  delete e;
  return 0;
}
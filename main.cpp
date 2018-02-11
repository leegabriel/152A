#include <iostream>
#include "event.hpp"

using namespace std;

int main () {
  Event* e = new Event(23);
  cout << e->get_timestamp() << endl;
  delete e;
  return 0;
}
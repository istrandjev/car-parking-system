#include "delay.h"

#include <iostream>

#include <ctime>

inline double get_time(){    
  return static_cast<double>(clock())/CLOCKS_PER_SEC;
};

void delay(double seconds) {
  double start_time = get_time();
  while (get_time() - start_time < seconds);
}

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <unistd.h>

#include "timer.h"

int
main(int argc, char* argv[]) {
  long d = 0;
  static const useconds_t delta = useconds_t(1e4);

  late::timer_monotonic_coarse t;
  for (int i = 1 ; i < argc ; ++i) {
    char * ep;
    long v = strtol(argv[i], &ep, 0);
    if (v < 0 || *ep)
      throw std::runtime_error(argv[i]);

    for (d += v ; !t.alarm(d) ; usleep(delta)) { }
    std::cout << int(t.elapsed()) << std::endl;
  }

  std::cerr << late::timer_process().value() << std::endl;
  return EXIT_SUCCESS;
}

//

#ifndef TIMER_H
#define TIMER_H

#include "try.h"
#include <ctime>

template <clockid_t TYPE>
class timer {
public:
  timer() { reset(); };

  void reset() { TRY(clock_gettime, TYPE, &ts); }

  struct timespec delta() const {
    struct timespec e;
    TRY(clock_gettime, TYPE, &e);

    if (e.tv_nsec < ts.tv_nsec) {
      e.tv_sec  -= 1;
      e.tv_nsec += nsec_in_sec;
    }

    e.tv_sec  -= ts.tv_sec;
    e.tv_nsec -= ts.tv_nsec;

    return e;
  }

  bool alarm(struct timespec t) const {
    struct timespec d = delta();
    return t.tv_sec < d.tv_sec ||
           (t.tv_sec == d.tv_sec && t.tv_nsec < d.tv_nsec);
  }

  bool alarm(time_t sec, long nsec = 0) const {
    struct timespec t = { .tv_sec = sec, .tv_nsec = nsec };
    return timer::alarm(t);
  }

  double value() const { return to_double(ts); }
  double elapsed() const { return to_double(delta()); }

private:
  enum { nsec_in_sec = int(1e9) };

  static double to_double(const struct timespec & t) {
    return t.tv_sec + float(t.tv_nsec)/float(nsec_in_sec);
  }

  struct timespec ts;
};

typedef timer<CLOCK_REALTIME> timer_realtime;
typedef timer<CLOCK_REALTIME_COARSE> timer_realtime_coarse;
typedef timer<CLOCK_MONOTONIC> timer_monotonic;
typedef timer<CLOCK_MONOTONIC_COARSE> timer_monotonic_coarse;
typedef timer<CLOCK_MONOTONIC_RAW> timer_monotonic_raw;
typedef timer<CLOCK_BOOTTIME> timer_boottime;
typedef timer<CLOCK_PROCESS_CPUTIME_ID> timer_process_cputime_id;
typedef timer<CLOCK_THREAD_CPUTIME_ID> timer_thread_cputime_id;

typedef timer_process_cputime_id timer_process;
typedef timer_thread_cputime_id timer_thread;

#endif//TIMER_H

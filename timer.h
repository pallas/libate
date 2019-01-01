#ifndef LATE__TIMER_H
#define LATE__TIMER_H

#include <lace/try.h>
#include <ctime>
#include <cmath>

namespace late {

template <clockid_t TYPE>
class timer {
public:
  timer() { reset(); };

  void reset() { TRY(clock_gettime, TYPE, &ts); }

  struct timespec resolution() const {
    struct timespec e;
    TRY(clock_getres, TYPE, &e);
    return e;
  }

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

  bool alarm(double d) const {
    double i, f = modf(d, &i);
    return alarm(i, f * nsec_in_sec);
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

#if defined(CLOCK_REALTIME_COARSE)
typedef timer<CLOCK_REALTIME_COARSE> timer_realtime_coarse;
#elif defined(CLOCK_REALTIME_FAST)
typedef timer<CLOCK_REALTIME_FASE> timer_realtime_coarse
#elif defined(CLOCK_REALTIME_PRECISE)
typedef timer<CLOCK_REALTIME_PRECISE> timer_realtime_coarse
#else
typedef timer_realtime timer_realtime_coarse;
#endif

typedef timer<CLOCK_MONOTONIC> timer_monotonic;

#if defined(CLOCK_MONOTONIC_COARSE)
typedef timer<CLOCK_MONOTONIC_COARSE> timer_monotonic_coarse;
#elif defined(CLOCK_MONOTONIC_FAST)
typedef timer<CLOCK_MONOTONIC_FAST> timer_monotonic_coarse;
#elif defined(CLOCK_MONOTONIC_PRECISE)
typedef timer<CLOCK_MONOTONIC_PRECISE> timer_monotonic_coarse;
#else
typedef timer_monotonic timer_monotonic_coarse;
#endif

#if defined(CLOCK_MONOTONIC_RAW)
typedef timer<CLOCK_MONOTONIC_RAW> timer_monotonic_raw;
#elif defined(CLOCK_MONOTONIC_RAW_APPROX)
typedef timer<_CLOCK_MONOTONIC_RAW> timer_monotonic_raw;
#else
typedef time_monotonic timer_monotonic_coarse;
#endif

#if defined(CLOCK_BOOTTIME)
typedef timer<CLOCK_BOOTTIME> timer_boottime;
#elif defined(CLOCK_UPTIME_RAW)
typedef timer<CLOCK_UPTIME_RAW> timer_boottime;
#elif defined(CLOCK_UPTIME_RAW_APPROX)
typedef timer<CLOCK_UPTIME_RAW_APPROX> timer_boottime;
#elif defined(CLOCK_UPTIME_FAST)
typedef timer<CLOCK_UPTIME_FAST> timer_boottime;
#elif defined(CLOCK_UPTIME_PRECISE)
typedef timer<CLOCK_UPTIME_PRECISE> timer_boottime;
#endif

typedef timer<CLOCK_PROCESS_CPUTIME_ID> timer_process_cputime_id;
typedef timer<CLOCK_THREAD_CPUTIME_ID> timer_thread_cputime_id;

typedef timer_process_cputime_id timer_process;
typedef timer_thread_cputime_id timer_thread;

} // namespace late

#endif//LATE__TIMER_H

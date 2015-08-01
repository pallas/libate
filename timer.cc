#include "timer.h"
template<> class timer<CLOCK_REALTIME>;
template<> class timer<CLOCK_REALTIME_COARSE>;
template<> class timer<CLOCK_MONOTONIC>;
template<> class timer<CLOCK_MONOTONIC_COARSE>;
template<> class timer<CLOCK_MONOTONIC_RAW>;
template<> class timer<CLOCK_BOOTTIME>;
template<> class timer<CLOCK_PROCESS_CPUTIME_ID>;
template<> class timer<CLOCK_THREAD_CPUTIME_ID>;

//

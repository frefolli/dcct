#ifndef DCCT_TIMER_HH
#define DCCT_TIMER_HH
#include <chrono>
#include <string>
#include <cmath>
namespace dcct {
  struct Timer {
    std::chrono::time_point<std::chrono::utc_clock> internal_clock;

    void reset();
    void round(std::string info);
    double_t round();
  };
}
#endif//DCCT_TIMER_HH
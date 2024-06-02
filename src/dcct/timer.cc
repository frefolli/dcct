#include <dcct/timer.hh>
#include <dcct/logging.hh>

void dcct::Timer::reset() {
  internal_clock = std::chrono::utc_clock::now();
}

void dcct::Timer::round(std::string info) {
  decltype(internal_clock) now = std::chrono::utc_clock::now();
  double_t timediff = std::chrono::duration<double_t>(now - internal_clock).count();
  dcct::LogInfo("Timer | " + info + " | Elapsed " + std::to_string(timediff) + "s");
  internal_clock = now;
}

double_t dcct::Timer::round() {
  decltype(internal_clock) now = std::chrono::utc_clock::now();
  double_t timediff = std::chrono::duration<double_t>(now - internal_clock).count();
  internal_clock = now;
  return timediff;
}
#include <dcct/logging.hh>
#include <iostream>

void dcct::RaiseFatalError(std::string message, uint32_t exit_code) {
  std::cerr << dcct::FATAL_ERROR << message << std::endl;
  exit(exit_code);
}

void dcct::LogInfo(std::string message) {
  std::cerr << dcct::INFO << message << std::endl;
}

void dcct::LogWarning(std::string message) {
  std::cerr << dcct::WARNING << message << std::endl;
}
#ifndef DCCT_TESTS
#define DCCT_TESTS
#include <cmath>
#include <cstdint>
#include <dcct/logging.hh>

namespace dcct {
  const double_t TESTING_ATOL = 10e-5;

  #define AssertLessThan(Lhs, Rhs) \
  if (Lhs >= Rhs) \
    dcct::RaiseFatalError("AssertLessThan(" #Lhs ", " #Rhs ") failed -> " + std::to_string(Lhs) + " >= " + std::to_string(Rhs));

  #define AssertEqual(Lhs, Rhs) \
  if (Lhs != Rhs) \
    dcct::RaiseFatalError("AssertEqual(" #Lhs ", " #Rhs ") failed -> " + std::to_string(Lhs) + " != " + std::to_string(Rhs));
}
#endif//DCCT_TESTS
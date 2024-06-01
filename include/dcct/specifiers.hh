#ifndef DCCT_SPECIFIERS_HH
#define DCCT_SPECIFIERS_HH
/** @file specifiers.hh specifier support structures */
#include <string>
#include <cstdint>
#include <cmath>
namespace dcct {
  /** Matrix Variants defined with X clause */
  #define MATRIX_VARIANTS \
    X(NONE), \
    X(SRC), \
    X(RND)
  
  #define X(_) _
  /**
   * Represents a request of matrix
  */
  struct MatrixSpecifier {
    enum Type {
      MATRIX_VARIANTS
    } type;
    std::string ID;
    uint32_t N;
    uint32_t M;
  };
  #undef X

  /**
   * Returns the string name of the Matrix type
   * @param type input matrix type
  */
  std::string ToString(MatrixSpecifier::Type type);

  /**
   * Parses a Matrix specifier from a string pattern
   * @param pattern input pattern
  */
  MatrixSpecifier ParseMatrixSpecifier(const std::string& pattern);
  
  /** Actuator Variants defined with X clause */
  #define ACTUATOR_VARIANTS \
    X(NONE), \
    X(SLOW), \
    X(FAST), \
    X(FFTW)
  
  #define X(_) _
  /**
   * Represents a request of actuator
  */
  struct ActuatorSpecifier {
    enum Type { ACTUATOR_VARIANTS } type;
    uint32_t blockSize;
    uint32_t quality;
  };
  #undef X
  
  /**
   * Returns the string name of the Actuator type
   * @param type input actuator type
  */
  std::string ToString(ActuatorSpecifier::Type type);

  /**
   * Parses a Actuator specifier from a string pattern
   * @param pattern input pattern
  */
  ActuatorSpecifier ParseActuatorSpecifier(const std::string& pattern);
}
#endif//DCCT_SPECIFIERS_HH
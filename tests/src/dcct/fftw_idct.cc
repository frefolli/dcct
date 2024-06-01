#include <dcct/tests.hh>
#include <dcct.hh>
#include <cassert>
#include <iostream>

int main() {
  Eigen::MatrixXd input;
  assert(dcct::FromMatrixSpecifier(input, {.type = dcct::MatrixSpecifier::Type::SRC, .ID = "dct", .N = 0, .M = 0}));
  Eigen::MatrixXd expected;
  assert(dcct::FromMatrixSpecifier(expected, {.type = dcct::MatrixSpecifier::Type::SRC, .ID = "input", .N = 0, .M = 0}));

  dcct::FFTWActuator actuator;
  Eigen::MatrixXd got = actuator.idct(input);

  std::cout << got << std::endl;

  dcct::Comparison comparison = dcct::CompareMatrices(expected, got);
  assert(comparison.same_dimension);
  AssertLessThan(comparison.norm_of_diff, dcct::TESTING_ATOL);
  AssertLessThan(comparison.mean_cwise_diff, dcct::TESTING_ATOL);
}
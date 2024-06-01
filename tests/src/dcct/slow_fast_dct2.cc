#include <dcct/tests.hh>
#include <dcct.hh>
#include <cassert>

int main() {
  Eigen::MatrixXd input;
  assert(dcct::FromMatrixSpecifier(input, {.type = dcct::MatrixSpecifier::Type::RND, .ID = "", .N = 50, .M = 50}));

  dcct::SlowActuator slow_actuator;
  dcct::FastActuator fast_actuator;
  Eigen::MatrixXd slow_dct2 = slow_actuator.dct2(input);
  Eigen::MatrixXd fast_dct2 = fast_actuator.dct2(input);

  dcct::Comparison comparison = dcct::CompareMatrices(slow_dct2, fast_dct2);
  assert(comparison.same_dimension);
  AssertLessThan(comparison.norm_of_diff, dcct::TESTING_ATOL);
  AssertLessThan(comparison.mean_cwise_diff, dcct::TESTING_ATOL);
}
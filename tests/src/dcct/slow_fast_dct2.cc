#include <dcct/tests.hh>
#include <dcct.hh>
#include <cassert>

int main() {
  Eigen::MatrixXd slow_input;
  assert(dcct::FromMatrixSpecifier(slow_input, {.type = dcct::MatrixSpecifier::Type::RND, .ID = "", .N = 50, .M = 50}));
  Eigen::MatrixXd fast_input = slow_input;

  dcct::SlowActuator slow_actuator;
  dcct::FastActuator fast_actuator;
  Eigen::MatrixXd slow_dct2 = slow_actuator.dct2(slow_input);
  Eigen::MatrixXd fast_dct2 = fast_actuator.dct2(fast_input);

  dcct::Comparison comparison = dcct::CompareMatrices(slow_dct2, fast_dct2);
  assert(comparison.same_dimension);
  AssertLessThan(comparison.norm_of_diff, dcct::TESTING_ATOL);
  AssertLessThan(comparison.mean_cwise_diff, dcct::TESTING_ATOL);
}
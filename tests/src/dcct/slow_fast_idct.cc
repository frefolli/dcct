#include <dcct/tests.hh>
#include <dcct.hh>
#include <cassert>

int main() {
  Eigen::MatrixXd slow_input;
  assert(dcct::FromMatrixSpecifier(slow_input, {.type = dcct::MatrixSpecifier::Type::RND, .ID = "", .N = 50, .M = 50}));
  Eigen::MatrixXd fast_input = slow_input;

  dcct::SlowActuator slow_actuator;
  dcct::FastActuator fast_actuator;
  Eigen::MatrixXd slow_idct = slow_actuator.idct(slow_input);
  Eigen::MatrixXd fast_idct = fast_actuator.idct(fast_input);

  dcct::Comparison comparison = dcct::CompareMatrices(slow_idct, fast_idct);
  assert(comparison.same_dimension);
  AssertLessThan(comparison.norm_of_diff, dcct::TESTING_ATOL);
  AssertLessThan(comparison.mean_cwise_diff, dcct::TESTING_ATOL);
}
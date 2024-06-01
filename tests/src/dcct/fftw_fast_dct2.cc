#include <dcct/tests.hh>
#include <dcct.hh>
#include <cassert>

int main() {
  Eigen::MatrixXd input;
  assert(dcct::FromMatrixSpecifier(input, {.type = dcct::MatrixSpecifier::Type::RND, .ID = "", .N = 10, .M = 10}));

  dcct::FFTWActuator fftw_actuator;
  dcct::FastActuator fast_actuator;
  Eigen::MatrixXd fftw_dct2 = fftw_actuator.dct2(input);
  Eigen::MatrixXd fast_dct2 = fast_actuator.dct2(input);

  dcct::Comparison comparison = dcct::CompareMatrices(fftw_dct2, fast_dct2);
  assert(comparison.same_dimension);
  AssertLessThan(comparison.norm_of_diff, dcct::TESTING_ATOL);
  AssertLessThan(comparison.mean_cwise_diff, dcct::TESTING_ATOL);
}
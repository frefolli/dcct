#include <dcct/tests.hh>
#include <dcct.hh>
#include <cassert>

int main() {
  Eigen::MatrixXd fftw_input;
  assert(dcct::FromMatrixSpecifier(fftw_input, {.type = dcct::MatrixSpecifier::Type::RND, .ID = "", .N = 10, .M = 10}));
  Eigen::MatrixXd fast_input = fftw_input;

  dcct::PocketFFTActuator fftw_actuator;
  dcct::FastActuator fast_actuator;
  Eigen::MatrixXd fftw_dct2 = fftw_actuator.dct2(fftw_input);
  Eigen::MatrixXd fast_dct2 = fast_actuator.dct2(fast_input);

  dcct::Comparison comparison = dcct::CompareMatrices(fftw_dct2, fast_dct2);
  assert(comparison.same_dimension);
  AssertLessThan(comparison.norm_of_diff, dcct::TESTING_ATOL);
  AssertLessThan(comparison.mean_cwise_diff, dcct::TESTING_ATOL);
}

#include <dcct/tests.hh>
#include <dcct.hh>
#include <cassert>

int main() {
  Eigen::MatrixXd fftw_input;
  assert(dcct::FromMatrixSpecifier(fftw_input, {.type = dcct::MatrixSpecifier::Type::RND, .ID = "", .N = 10, .M = 10}));
  Eigen::MatrixXd fast_input = fftw_input;

  dcct::PocketFFTActuator fftw_actuator;
  dcct::FastActuator fast_actuator;
  Eigen::MatrixXd fftw_idct = fftw_actuator.idct(fftw_input);
  Eigen::MatrixXd fast_idct = fast_actuator.idct(fast_input);

  dcct::Comparison comparison = dcct::CompareMatrices(fftw_idct, fast_idct);
  assert(comparison.same_dimension);
  AssertLessThan(comparison.norm_of_diff, dcct::TESTING_ATOL);
  AssertLessThan(comparison.mean_cwise_diff, dcct::TESTING_ATOL);
}

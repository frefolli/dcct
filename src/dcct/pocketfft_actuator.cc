#include <dcct/pocketfft_actuator.hh>
#include <pocketfft.h>
#define POCKETFFT_DCT2 2
#define POCKETFFT_DCT3 3
#define POCKETFFT_SINGLETHREAD 1

Eigen::MatrixXd dcct::PocketFFTActuator::dct(Eigen::MatrixXd& X) {
  uint32_t N = X.rows(), M = X.cols();
  Eigen::MatrixXd Y(N, M);
  double_t sqrt_of_two_over_M = std::sqrt(0.5 / (double_t) M);
  double_t one_over_sqrt_of_two = std::sqrt(0.5);

  const pocketfft::shape_t shape = {N, M};
  const pocketfft::stride_t stride_in = {sizeof(double_t), (long)(sizeof(double_t) * N)};
  const pocketfft::stride_t stride_out = {sizeof(double_t), (long)(sizeof(double_t) * N)};
  const pocketfft::shape_t axes = {1};
  pocketfft::dct<double_t>(shape, stride_in, stride_out, axes, POCKETFFT_DCT2, X.data(), Y.data(), 1, false, POCKETFFT_SINGLETHREAD);

  Y *= sqrt_of_two_over_M;
  Y.col(0) *= one_over_sqrt_of_two;
  return Y;
}

Eigen::MatrixXd dcct::PocketFFTActuator::dct2(Eigen::MatrixXd& X) {
  Eigen::MatrixXd Y = dct(X).transpose();
  X = dct(Y).transpose();
  return X;
}

Eigen::MatrixXd dcct::PocketFFTActuator::idct(Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);

  double_t sqrt_of_two_over_M = std::sqrt(0.5 / (double_t) M);
  double_t one_over_sqrt_of_two = std::sqrt(0.5);
  Y /= sqrt_of_two_over_M;
  Y.col(0) /= one_over_sqrt_of_two;

  const pocketfft::shape_t shape = {N, M};
  const pocketfft::stride_t stride_in = {sizeof(double_t), (long)(sizeof(double_t) * N)};
  const pocketfft::stride_t stride_out = {sizeof(double_t), (long)(sizeof(double_t) * N)};
  const pocketfft::shape_t axes = {1};
  pocketfft::dct<double_t>(shape, stride_in, stride_out, axes, POCKETFFT_DCT3, Y.data(), X.data(), 1, false, POCKETFFT_SINGLETHREAD);

  return X / (2 * M);
}

Eigen::MatrixXd dcct::PocketFFTActuator::idct2(Eigen::MatrixXd& Y) {
  Eigen::MatrixXd X = idct(Y).transpose();
  Y = idct(X).transpose();
  return Y;
}

#include <dcct/pocketfft_actuator.hh>
#include <pocketfft.h>
#define POCKETFFT_DCT2 2
#define POCKETFFT_SINGLETHREAD 1

Eigen::MatrixXd dcct::PocketFFTActuator::dct(Eigen::MatrixXd& X) {
  uint32_t N = X.rows(), M = X.cols();
  Eigen::MatrixXd Y(N, M);

  const pocketfft::shape_t shape = {N, M};
  const pocketfft::stride_t stride_in = {sizeof(double_t), (long)(sizeof(double_t) * M)};
  const pocketfft::stride_t stride_out = {sizeof(double_t), (long)(sizeof(double_t) * M)};
  const pocketfft::shape_t axes = {1};
  pocketfft::dct<double_t>(shape, stride_in, stride_out, axes, POCKETFFT_DCT2, X.data(), Y.data(), 1, true, POCKETFFT_SINGLETHREAD);

  return Y;
}

Eigen::MatrixXd dcct::PocketFFTActuator::dct2(Eigen::MatrixXd& X) {
  uint32_t N = X.rows(), M = X.cols();
  Eigen::MatrixXd Y(N, M);

  const pocketfft::shape_t shape = {N, M};
  const pocketfft::stride_t stride_in = {sizeof(double_t), (long)(sizeof(double_t) * M)};
  const pocketfft::stride_t stride_out = {sizeof(double_t), (long)(sizeof(double_t) * M)};
  const pocketfft::shape_t axes = {1};
  pocketfft::dct<double_t>(shape, stride_in, stride_out, axes, POCKETFFT_DCT2, X.data(), Y.data(), 1, true, POCKETFFT_SINGLETHREAD);

  return Y;
}

Eigen::MatrixXd dcct::PocketFFTActuator::idct(Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);
  return X;
}

Eigen::MatrixXd dcct::PocketFFTActuator::idct2(Eigen::MatrixXd& Y) {
  Eigen::MatrixXd X = idct(Y).transpose();
  Y = idct(X).transpose();
  return Y;
}

#include <dcct/fftw_actuator.hh>
#include <fftw3.h>

Eigen::MatrixXd dcct::FFTWActuator::dct(const Eigen::MatrixXd& X) {
  uint32_t N = X.rows(), M = X.cols();
  Eigen::MatrixXd Y(N, M);
  double_t sqrt_of_one_half_over_N = std::sqrt(0.5 / (double_t) M);
  double_t one_over_sqrt_of_two = std::sqrt(0.5);

  int dims[1] = {M}; 
  fftw_r2r_kind kinds[1] = {FFTW_REDFT10};
  fftw_plan plan = fftw_plan_many_r2r(1, dims, N, const_cast<double*>(X.data()), dims, N, 1, Y.data(), dims, N, 1, kinds, FFTW_PRESERVE_INPUT);
  fftw_execute_r2r(plan, const_cast<double*>(X.data()), Y.data());
  fftw_destroy_plan(plan);

  Y *= sqrt_of_one_half_over_N;
  Y.col(0) *= one_over_sqrt_of_two;
  return Y;
}

Eigen::MatrixXd dcct::FFTWActuator::dct2(const Eigen::MatrixXd& X) {
  return dct(dct(X).transpose()).transpose();
}

Eigen::MatrixXd dcct::FFTWActuator::idct(const Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);
  Eigen::MatrixXd Z(N, M);

  double_t sqrt_of_one_half_over_N = std::sqrt(0.5 / (double_t) M);
  double_t one_over_sqrt_of_two = std::sqrt(0.5);
  Z = Y / sqrt_of_one_half_over_N;
  Z.col(0) = Z.col(0) / one_over_sqrt_of_two;

  int dims[1] = {M}; 
  fftw_r2r_kind kinds[1] = {FFTW_REDFT01};
  fftw_plan plan = fftw_plan_many_r2r(1, dims, N, Z.data(), dims, N, 1, X.data(), dims, N, 1, kinds, FFTW_DESTROY_INPUT);
  fftw_execute_r2r(plan, Z.data(), X.data());
  fftw_destroy_plan(plan);

  return X / (2 * M);
}

Eigen::MatrixXd dcct::FFTWActuator::idct2(const Eigen::MatrixXd& Y) {
  return idct(idct(Y).transpose()).transpose();
}
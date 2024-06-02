#include <dcct/slow_actuator.hh>

Eigen::MatrixXd dcct::SlowActuator::dct(Eigen::MatrixXd& X) {
  uint32_t N = X.rows(), M = X.cols();
  Eigen::MatrixXd Y(N, M);

  double_t pi_over_M = std::numbers::pi / (double_t) M;
  double_t sqrt_of_two_over_M = std::sqrt(2.0 / (double_t) M);
  double_t one_over_sqrt_of_two = std::sqrt(0.5);

  for (uint32_t i = 0; i < N; ++i) {
    for (uint32_t k = 0; k < M; ++k) {
      double_t sum = 0;
      double_t _k_pi_over_M = pi_over_M * k;
      for (uint32_t j = 0; j < M; j++) {
        sum += std::cos(_k_pi_over_M * (j + 0.5)) * X.coeff(i, j);
      }
      Y.coeffRef(i, k) = sum * sqrt_of_two_over_M;
    }
    Y.coeffRef(i, 0) *= one_over_sqrt_of_two;
  }
  return Y;
}

Eigen::MatrixXd dcct::SlowActuator::dct2(Eigen::MatrixXd& X) {
  Eigen::MatrixXd Y = dct(X).transpose();
  X = dct(Y).transpose();
  return X;
}

Eigen::MatrixXd dcct::SlowActuator::idct(Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);
  
  double_t pi_over_M = std::numbers::pi / (double_t) M;
  double_t sqrt_of_two_over_M = std::sqrt(2.0 / (double_t) M);
  double_t one_over_sqrt_of_two = std::sqrt(0.5);

  for (uint32_t i = 0; i < N; ++i) {
    double_t y0 = Y.coeff(i, 0) * one_over_sqrt_of_two;
    for (uint32_t j = 0; j < M; ++j) {
      double_t j_plus_half_pi_over_M = (j + 0.5) * pi_over_M;
      double_t sum = y0;
      for (uint32_t k = 1; k < M; ++k) {
        sum += cos(k * j_plus_half_pi_over_M) * Y.coeff(i, k);
      }
      X.coeffRef(i, j) = sum * sqrt_of_two_over_M;
    }
  }
  return X;
}

Eigen::MatrixXd dcct::SlowActuator::idct2(Eigen::MatrixXd& Y) {
  Eigen::MatrixXd X = idct(Y).transpose();
  Y = idct(X).transpose();
  return Y;
}
#include <dcct/slow_actuator.hh>

Eigen::MatrixXd dcct::SlowActuator::dct(const Eigen::MatrixXd& X) {
  uint32_t N = X.rows(), M = X.cols();
  Eigen::MatrixXd Y(N, M);

  double_t _pi_over_M = std::numbers::pi / (double_t) M;
  double_t _sqrt_of_2_over_M = std::sqrt(2.0 / (double_t) M);
  double_t _1_over_sqrt_of_2 = std::sqrt(0.5);

  for (uint32_t j = 0; j < M; ++j) {
    for (uint32_t k = 0; k < N; ++k) {
      double_t sum = 0;
      double_t _k_pi_over_M = _pi_over_M * k;
      for (uint32_t i = 0; i < N; i++) {
        sum += std::cos(_k_pi_over_M * (i + 0.5)) * X.coeff(j, i);
      }
      Y.coeffRef(j, k) = sum * _sqrt_of_2_over_M;
    }
    Y.coeffRef(j, 0) *= _1_over_sqrt_of_2;
  }
  return Y;
}

Eigen::MatrixXd dcct::SlowActuator::dct2(const Eigen::MatrixXd& X) {
  return dct(dct(X).transpose()).transpose();
}

Eigen::MatrixXd dcct::SlowActuator::idct(const Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);
  return X;
}

Eigen::MatrixXd dcct::SlowActuator::idct2(const Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);
  return X;
}
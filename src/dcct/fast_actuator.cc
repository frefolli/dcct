#include <dcct/fast_actuator.hh>
#include <iostream>

Eigen::MatrixXd dcct::FastActuator::dct(const Eigen::MatrixXd& X) {
  uint32_t N = X.rows(), M = X.cols();
  Eigen::MatrixXd Y(N, M);

  double_t pi_over_M = std::numbers::pi / (double_t)M;
  double_t _sqrt_of_2_over_M = std::sqrt(2.0 / (double_t) M);
  double_t _1_over_sqrt_of_2 = std::sqrt(0.5);
  Eigen::VectorXd Z(M);
  for (uint32_t j = 0; j < M; ++j) {
    Z.coeffRef(j) = (j + 0.5) * pi_over_M;
  }

  Eigen::RowVectorXd W;
  for (uint32_t k = 0; k < M; ++k) {
    W = (Z * k).array().cos();
    Y.col(k) = (W * X.transpose()).colwise().sum() * _sqrt_of_2_over_M;
  }
  Y.col(0) *= _1_over_sqrt_of_2;

  return Y;
}

Eigen::MatrixXd dcct::FastActuator::dct2(const Eigen::MatrixXd& X) {
  return dct(dct(X).transpose()).transpose();
}

Eigen::MatrixXd dcct::FastActuator::idct(const Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);
  return X;
}

Eigen::MatrixXd dcct::FastActuator::idct2(const Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);
  return X;
}
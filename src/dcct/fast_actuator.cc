#include <dcct/fast_actuator.hh>
#include <iostream>

Eigen::MatrixXd dcct::FastActuator::dct(Eigen::MatrixXd& X) {
  uint32_t N = X.rows(), M = X.cols();
  Eigen::MatrixXd Y(N, M);

  double_t pi_over_M = std::numbers::pi / (double_t)M;
  double_t sqrt_of_two_over_M = std::sqrt(2.0 / (double_t) M);
  double_t one_over_sqrt_of_two = std::sqrt(0.5);
  Eigen::VectorXd Z(M);
  for (uint32_t j = 0; j < M; ++j) {
    Z.coeffRef(j) = (j + 0.5) * pi_over_M;
  }

  Eigen::RowVectorXd W;
  for (uint32_t k = 0; k < M; ++k) {
    W = (Z * k).array().cos();
    Y.col(k) = (W * X.transpose()).colwise().sum() * sqrt_of_two_over_M;
  }
  Y.col(0) *= one_over_sqrt_of_two;

  return Y;
}

Eigen::MatrixXd dcct::FastActuator::dct2(Eigen::MatrixXd& X) {
  Eigen::MatrixXd Y = dct(X).transpose();
  X = dct(Y).transpose();
  return X;
}

Eigen::MatrixXd dcct::FastActuator::idct(Eigen::MatrixXd& Y) {
  uint32_t N = Y.rows(), M = Y.cols();
  Eigen::MatrixXd X(N, M);
  
  double_t pi_over_M = std::numbers::pi / (double_t)M;
  double_t sqrt_of_two_over_M = std::sqrt(2.0 / (double_t) M);
  double_t one_over_sqrt_of_two = std::sqrt(0.5);
  Y *= sqrt_of_two_over_M;
  Y.col(0) *= one_over_sqrt_of_two;

  Eigen::VectorXd Z(M);
  for (uint32_t k = 0; k < M; ++k) {
    Z.coeffRef(k) = k * pi_over_M;
  }
  
  Eigen::RowVectorXd W;
  for (uint32_t j = 0; j < M; ++j) {
    W = (Z * (j + 0.5)).array().cos();
    X.col(j) = W * Y.transpose();
  }
  return X;
}

Eigen::MatrixXd dcct::FastActuator::idct2(Eigen::MatrixXd& Y) {
  Eigen::MatrixXd X = idct(Y).transpose();
  Y = idct(X).transpose();
  return Y;
}
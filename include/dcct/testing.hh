#ifndef DCCT_TESTING_HH
#define DCCT_TESTING_HH
/** @file testing.hh Testing utilities */
#include <cmath>
#include <eigen3/Eigen/Core>

namespace dcct {
  /**
   * Structure that contains report of similarity between matrices
  */
  struct Comparison {
    bool same_dimension;
    double_t min_cwise_diff;
    double_t max_cwise_diff;
    double_t mean_cwise_diff;
    double_t norm_of_diff;
  };

  /**
   * Compares two dense dynamic matrices
  */
  Comparison CompareMatrices(Eigen::MatrixXd& A, Eigen::MatrixXd& B);
}
#endif//DCCT_TESTING_HH
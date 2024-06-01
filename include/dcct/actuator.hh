#ifndef DCCT_ACTUATOR_HH
#define DCCT_ACTUATOR_HH
/** @file actuator trait */
#include <dcct/traits.hh>
#include <Eigen/Dense>
namespace dcct {
  /**
   * Trait for Actuator
  */
  Trait(Actuator,
    Fn(Eigen::MatrixXd, dct, (const Eigen::MatrixXd& X))
    Fn(Eigen::MatrixXd, dct2, (const Eigen::MatrixXd& X))
    Fn(Eigen::MatrixXd, idct, (const Eigen::MatrixXd& Y))
    Fn(Eigen::MatrixXd, idct2, (const Eigen::MatrixXd& Y))
  )
}
#endif//DCCT_ACTUATOR_HH

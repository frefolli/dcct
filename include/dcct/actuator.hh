#ifndef DCCT_ACTUATOR_HH
#define DCCT_ACTUATOR_HH
/** @file actuator trait */
#include <dcct/traits.hh>
#include <eigen3/Eigen/Core>
namespace dcct {
  /**
   * Trait for Actuator
  */
  Trait(Actuator,
    Fn(Eigen::MatrixXd, dct, (const Eigen::MatrixXd& M))
    Fn(Eigen::MatrixXd, dct2, (const Eigen::MatrixXd& M))
  )
}
#endif//DCCT_ACTUATOR_HH
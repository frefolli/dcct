#ifndef DCCT_SLOW_ACTUATOR_HH
#define DCCT_SLOW_ACTUATOR_HH
/** @file slow_actuator.hh Slow Actuator */
#include <dcct/actuator.hh>
namespace dcct {
  struct SlowActuator {
    Eigen::MatrixXd dct(const Eigen::MatrixXd& X);
    Eigen::MatrixXd dct2(const Eigen::MatrixXd& X);

    ImplTrait(SlowActuator, Actuator);
  };
}
#endif//DCCT_SLOW_ACTUATOR_HH

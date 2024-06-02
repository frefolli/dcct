#ifndef DCCT_SLOW_ACTUATOR_HH
#define DCCT_SLOW_ACTUATOR_HH
/** @file slow_actuator.hh Slow Actuator */
#include <dcct/actuator.hh>
namespace dcct {
  struct SlowActuator {
    Eigen::MatrixXd dct(Eigen::MatrixXd& X);
    Eigen::MatrixXd dct2(Eigen::MatrixXd& X);
    Eigen::MatrixXd idct(Eigen::MatrixXd& Y);
    Eigen::MatrixXd idct2(Eigen::MatrixXd& Y);

    ImplTrait(SlowActuator, Actuator);
  };
}
#endif//DCCT_SLOW_ACTUATOR_HH

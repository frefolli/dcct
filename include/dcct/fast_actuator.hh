#ifndef DCCT_FAST_ACTUATOR_HH
#define DCCT_FAST_ACTUATOR_HH
/** @file fast_actuator.hh Fast Actuator */
#include <dcct/actuator.hh>
namespace dcct {
  struct FastActuator {
    Eigen::MatrixXd dct(Eigen::MatrixXd& X);
    Eigen::MatrixXd dct2(Eigen::MatrixXd& X);
    Eigen::MatrixXd idct(Eigen::MatrixXd& Y);
    Eigen::MatrixXd idct2(Eigen::MatrixXd& Y);

    ImplTrait(FastActuator, Actuator);
  };
}
#endif//DCCT_FAST_ACTUATOR_HH

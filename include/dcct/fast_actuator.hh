#ifndef DCCT_FAST_ACTUATOR_HH
#define DCCT_FAST_ACTUATOR_HH
/** @file fast_actuator.hh Fast Actuator */
#include <dcct/actuator.hh>
namespace dcct {
  struct FastActuator {
    Eigen::MatrixXd dct(const Eigen::MatrixXd& M);
    Eigen::MatrixXd dct2(const Eigen::MatrixXd& M);

    ImplTrait(FastActuator, Actuator);
  };
}
#endif//DCCT_FAST_ACTUATOR_HH
#ifndef DCCT_FFTW_ACTUATOR_HH
#define DCCT_FFTW_ACTUATOR_HH
/** @file fftw_actuator.hh FFTW Actuator */
#include <dcct/actuator.hh>
namespace dcct {
  struct FFTWActuator {
    Eigen::MatrixXd dct(const Eigen::MatrixXd& X);
    Eigen::MatrixXd dct2(const Eigen::MatrixXd& X);

    ImplTrait(FFTWActuator, Actuator);
  };
}
#endif//DCCT_FFTW_ACTUATOR_HH

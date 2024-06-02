#ifndef DCCT_FFTW_ACTUATOR_HH
#define DCCT_FFTW_ACTUATOR_HH
/** @file fftw_actuator.hh FFTW Actuator */
#include <dcct/actuator.hh>
namespace dcct {
  struct FFTWActuator {
    Eigen::MatrixXd dct(Eigen::MatrixXd& X);
    Eigen::MatrixXd dct2(Eigen::MatrixXd& X);
    Eigen::MatrixXd idct(Eigen::MatrixXd& Y);
    Eigen::MatrixXd idct2(Eigen::MatrixXd& Y);

    ImplTrait(FFTWActuator, Actuator);
  };
}
#endif//DCCT_FFTW_ACTUATOR_HH

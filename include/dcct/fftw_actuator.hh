#ifndef DCCT_FFTW_ACTUATOR_HH
#define DCCT_FFTW_ACTUATOR_HH
/** @file fftw_actuator.hh FFTW Actuator */
#include <dcct/actuator.hh>
namespace dcct {
  struct FFTWActuator {
    Eigen::MatrixXd dct(const Eigen::MatrixXd& X);
    Eigen::MatrixXd dct2(const Eigen::MatrixXd& X);
    Eigen::MatrixXd idct(const Eigen::MatrixXd& Y);
    Eigen::MatrixXd idct2(const Eigen::MatrixXd& Y);

    ImplTrait(FFTWActuator, Actuator);
  };
}
#endif//DCCT_FFTW_ACTUATOR_HH

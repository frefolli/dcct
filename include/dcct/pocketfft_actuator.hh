#ifndef DCCT_POCKETFFT_ACTUATOR_HH
#define DCCT_POCKETFFT_ACTUATOR_HH
/** @file fftw_actuator.hh PocketFFT Actuator */
#include <dcct/actuator.hh>
namespace dcct {
  struct PocketFFTActuator {
    Eigen::MatrixXd dct(Eigen::MatrixXd& X);
    Eigen::MatrixXd dct2(Eigen::MatrixXd& X);
    Eigen::MatrixXd idct(Eigen::MatrixXd& Y);
    Eigen::MatrixXd idct2(Eigen::MatrixXd& Y);

    ImplTrait(PocketFFTActuator, Actuator);
  };
}
#endif//DCCT_POCKETFFT_ACTUATOR_HH

#include <cmath>
#include <dcct/compression.hh>
#include <dcct/fftw_actuator.hh>
#include <filesystem>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

inline void CopyTo(const stbi_uc* input_channel,
                   Eigen::MatrixXd& matrix,
                   const uint32_t& N,
                   const uint32_t& M,
                   const uint32_t& dx,
                   const uint32_t& dy) {
  for (uint32_t i = 0; i < N; ++i) {
    for (uint32_t j = 0; j < M; j++) {
      matrix.coeffRef(i, j) = *(input_channel + i * dy + j * dx);
    }
  }
}

inline void CopyFrom(stbi_uc* output_channel,
                     const Eigen::MatrixXd& matrix,
                     const uint32_t& N,
                     const uint32_t& M,
                     const uint32_t& dx,
                     const uint32_t& dy) {
  for (uint32_t i = 0; i < N; ++i) {
    for (uint32_t j = 0; j < M; j++) {
      *(output_channel + i * dy + j * dx) = matrix.coeff(i, j);
    }
  }
}

inline void RoundBlock(Eigen::MatrixXd& block, const uint32_t& blockSize) {
  double_t val = 0;
  for (uint32_t i = 0; i < blockSize; ++i) {
    for (uint32_t j = 0; j < blockSize; ++j) {
      val = std::round(block.coeff(i, j));
      if (val < 0)
        val = 0;
      else if (val > 255)
        val = 255;
      block.coeffRef(i, j) = val;
    }
  }
}

inline void CutBlock(Eigen::MatrixXd& block, const uint32_t& blockSize, const uint32_t& quality) {
  for (int32_t i = blockSize - 1; i >= 0; --i) {
    for (int32_t j = blockSize - 1; j >= 0; --j) {
      if (i + j >= quality) {
        block.coeffRef(i, j) = 0.0f;
      } else {
        break;
        // pass to next line: backward pixels not interested
      }
    }
  }
}

inline void CompressChannel(stbi_uc* output_channel,
                            const Eigen::MatrixXd& matrix,
                            const uint32_t& N,
                            const uint32_t& M,
                            const uint32_t& dx,
                            const uint32_t& dy)

bool dcct::CompressImage(const std::string& input_filepath,
                         const std::string& output_filepath,
                         const dcct::ActuatorSpecifier& specifier) {
  if (!std::filesystem::exists(input_filepath)) {
    return false;
  }

  int width, height, channels;
  stbi_uc* input_data = stbi_load(input_filepath.c_str(), &width, &height, &channels, 0);
  if (input_data == nullptr)
    return false;

  stbi_uc* output_data = (stbi_uc*) malloc (sizeof(stbi_uc) * width * height * channels);
  if (output_data == nullptr)
    return false;

  for (uint32_t i = 0; i < channels; ++i) {
    CompressChannel(input_data + i, output_data + i, channels, specifier);
  }
  /**
    * Magic goes here
    */

  stbi_image_free(input_data);
  stbi_image_free(output_data);

  return true;
}

#include <dcct/logging.hh>
#include <dcct/compression.hh>
#include <dcct/fftw_actuator.hh>
#include <dcct/slow_actuator.hh>
#include <dcct/fast_actuator.hh>
#include <cmath>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION 
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

/**
  * @param dx is the shift between cells in the same row
  * @param dy is the shift between cells in the same column
  */
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

/**
  * @param grid_height is the amount of blocks in vertical
  * @param grid_width is the amount of blocks in horizontal
  */
template<typename ActuatorImpl>
inline void CompressChannel(stbi_uc* input_channel,
                            stbi_uc* output_channel,
                            Eigen::MatrixXd& matrix,
                            const uint32_t& N,
                            const uint32_t& M,
                            ActuatorImpl& actuator,
                            const uint32_t& blockSize,
                            const uint32_t& quality,
                            const uint32_t& dx,
                            const uint32_t& dy) {
  CopyTo(input_channel, matrix, N, M, dx, dy);
  
  for (uint32_t i = 0; i < N; i += blockSize) {
    if (i + blockSize > N)
      break;
    for (uint32_t j = 0; j < M; j += blockSize) {
      if (j + blockSize > M)
        break;
      Eigen::MatrixXd block = matrix.block(i, j, blockSize, blockSize);
      block = actuator.dct2(block);
      CutBlock(block, blockSize, quality);
      block = actuator.idct2(block);
      RoundBlock(block, blockSize);
      matrix.block(i, j, blockSize, blockSize) = block;
    }
  }
  
  CopyFrom(output_channel, matrix, N, M, dx, dy);
}

template<typename ActuatorImpl>
inline void CompressChannels(stbi_uc* input_data,
                             stbi_uc* output_data,
                             const uint32_t& N,
                             const uint32_t& M,
                             const uint32_t& channels,
                             ActuatorImpl& actuator,
                             const uint32_t& blockSize,
                             const uint32_t& quality,
                             std::function<void (uint32_t)> set_progressbar_length = nullptr,
                             std::function<void ()> increment_progressbar = nullptr) {
  uint32_t dx = channels;
  uint32_t dy = dx * M;
  Eigen::MatrixXd matrix(N, M);
  if (set_progressbar_length != nullptr)
    set_progressbar_length(channels);
  for (uint32_t i = 0; i < channels; ++i) {
    CompressChannel(input_data + i, output_data + i,
                    matrix, N, M, actuator,
                    blockSize, quality, dx, dy);
    if (increment_progressbar != nullptr)
      increment_progressbar();
  }
}

bool dcct::CompressImage(const std::string& input_filepath,
                         const std::string& output_filepath,
                         const dcct::ActuatorSpecifier& specifier,
                         std::function<void (uint32_t)> set_progressbar_length,
                         std::function<void ()> increment_progressbar) {
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

  switch (specifier.type) {
    case dcct::ActuatorSpecifier::SLOW: {
      dcct::SlowActuator actuator;
      CompressChannels(input_data, output_data, height, width, channels, actuator, specifier.blockSize, specifier.quality, set_progressbar_length, increment_progressbar);
    }; break;
    case dcct::ActuatorSpecifier::FAST: {
      dcct::FastActuator actuator;
      CompressChannels(input_data, output_data, height, width, channels, actuator, specifier.blockSize, specifier.quality, set_progressbar_length, increment_progressbar);
    }; break;
    case dcct::ActuatorSpecifier::FFTW: {
      dcct::FFTWActuator actuator;
      CompressChannels(input_data, output_data, height, width, channels, actuator, specifier.blockSize, specifier.quality, set_progressbar_length, increment_progressbar);
    }; break;
    case dcct::ActuatorSpecifier::NONE: {
      dcct::RaiseFatalError("actuator not specified");
    }; break;
  }
  stbi_image_free(input_data);
  
  std::string ext = std::filesystem::path(output_filepath).extension();
  std::string png = ".png";
  std::string bmp = ".bmp";
  std::string jpg = ".jpg";
  if (ext == png) {
    stbi_write_png(output_filepath.c_str(), width, height, channels, output_data, channels * width);
  } else if (ext == bmp) {
    stbi_write_bmp(output_filepath.c_str(), width, height, channels, output_data);
  } else if (ext == jpg) {
    stbi_write_jpg(output_filepath.c_str(), width, height, channels, output_data, 100);
  } else {
    dcct::RaiseFatalError("wrong image extension '" + ext + "'");
  }
  
  stbi_image_free(output_data);

  return true;
}

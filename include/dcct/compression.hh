#ifndef DCCT_COMPRESSION_HH
#define DCCT_COMPRESSION_HH
/** @file compression.hh */
#include <dcct/specifiers.hh>
#include <functional>

namespace dcct {
  enum CompressionEvent {
    OK,
    CANNOT_OPEN,
    CANNOT_ALLOCATE,
    CANNOT_COMPRESS,
    CANNOT_WRITE
  };
  std::string ToString(CompressionEvent event);

  CompressionEvent CompressImage(const std::string& input_filepath,
                                 const std::string& output_filepath,
                                 const ActuatorSpecifier& specifier,
                                 std::function<void (uint32_t)> set_progressbar_length = nullptr,
                                 std::function<void ()> increment_progressbar = nullptr);
}
#endif//DCCT_COMPRESSION_HH

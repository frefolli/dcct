#ifndef DCCT_COMPRESSION_HH
#define DCCT_COMPRESSION_HH
/** @file compression.hh */
#include <dcct/specifiers.hh>
namespace dcct {
  bool CompressImage(const std::string& input_filepath,
                     const std::string& output_filepath,
                     const ActuatorSpecifier& specifier);
}
#endif//DCCT_COMPRESSION_HH

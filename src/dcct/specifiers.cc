#include <dcct/specifiers.hh>

inline void _ProcessBufferForMatrixSpecifier(dcct::MatrixSpecifier& result, bool& already_read_N, std::string& buffer) {
  switch (result.type) {
    case dcct::MatrixSpecifier::Type::NONE: {
      if (buffer == "src") {
        result.type = dcct::MatrixSpecifier::Type::SRC;
      } else if (buffer == "rnd") {
        result.type = dcct::MatrixSpecifier::Type::RND;
      }
    }; break;
    case dcct::MatrixSpecifier::Type::SRC: {
      result.ID = buffer;
    }; break;
    default: {
      if (already_read_N) {
        result.M = std::stoi(buffer);
      } else {
        result.N = std::stoi(buffer);
        already_read_N = true;
      }
    }; break;
  }
}

dcct::MatrixSpecifier dcct::ParseMatrixSpecifier(std::string pattern) {
  dcct::MatrixSpecifier result = {
    .type = dcct::MatrixSpecifier::Type::NONE,
    .ID = "",
    .N = 50,
    .M = 50
  };
  bool already_read_N = false;
  std::string buffer;
  for (char c : pattern) {
    if (c == ':') {
      if (buffer.size() > 0) {
        _ProcessBufferForMatrixSpecifier(result, already_read_N, buffer);
        buffer.clear();
      }
    } else {
      buffer += c;
    }
  }
  if (buffer.size() > 0) {
    _ProcessBufferForMatrixSpecifier(result, already_read_N, buffer);
  }
  return result;
}

std::string dcct::ToString(dcct::MatrixSpecifier::Type type) {
  constexpr const char* _variants[] = {
    #define X(_) #_
      MATRIX_VARIANTS
    #undef X
  };
  constexpr int _bound = (sizeof(_variants) / sizeof(const char *));

  // Int values can be coerced to component_t
  if (type < _bound)
    return _variants[type];
  return "<matrix-type>";
}

inline void _ProcessBufferForActuatorSpecifier(dcct::ActuatorSpecifier& result, bool& already_read_blockSize, std::string& buffer) {
  switch (result.type) {
    case dcct::ActuatorSpecifier::Type::NONE: {
      if (buffer == "slow") {
        result.type = dcct::ActuatorSpecifier::Type::SLOW;
      } else if (buffer == "fast") {
        result.type = dcct::ActuatorSpecifier::Type::FAST;
      } else if (buffer == "fftw") {
        result.type = dcct::ActuatorSpecifier::Type::FFTW;
      }
    }; break;
    default: {
      if (already_read_blockSize) {
        result.quality = std::stoi(buffer);
      } else {
        result.blockSize = std::stoi(buffer);
        already_read_blockSize = true;
      }
    }; break;
  }
}

dcct::ActuatorSpecifier dcct::ParseActuatorSpecifier(std::string pattern) {
  dcct::ActuatorSpecifier result = {
    .type = dcct::ActuatorSpecifier::Type::NONE,
    .blockSize = 10,
    .quality = 8
  };

  bool already_read_tol = false;
  std::string buffer;
  for (char c : pattern) {
    if (c == ':') {
      if (buffer.size() > 0) {
        _ProcessBufferForActuatorSpecifier(result, already_read_tol, buffer);
        buffer.clear();
      }
    } else {
      buffer += c;
    }
  }
  if (buffer.size() > 0) {
    _ProcessBufferForActuatorSpecifier(result, already_read_tol, buffer);
  }
  return result;
}

std::string dcct::ToString(dcct::ActuatorSpecifier::Type type) {
  constexpr const char* _variants[] = {
    #define X(_) #_
      ACTUATOR_VARIANTS
    #undef X
  };
  constexpr int _bound = (sizeof(_variants) / sizeof(const char *));

  // Int values can be coerced to component_t
  if (type < _bound)
    return _variants[type];
  return "<actuator-type>";
}
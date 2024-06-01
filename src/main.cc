#include <dcct.hh>
#include <cassert>
#include <iostream>
#include <chrono>

const std::string DEFAULT_MATRIX_PATTERN = "src:input";
const std::string DEFAULT_ACTUATOR_PATTERN = "fttw:10:8";

struct CliConfig {
  std::string matrix_pattern = DEFAULT_MATRIX_PATTERN;
  std::string actuator_pattern = DEFAULT_ACTUATOR_PATTERN;
  bool dry_run = false;
  bool verbose = false;
};

inline void PrintHelp(std::string executable) {
  std::cerr << "Usage: " << executable << " [options]" << std::endl;
  std::cerr << "Options:" << std::endl;
  std::cerr << "  -m/--matrix <matrix-pattern>          Expects a Matrix Specifer of form `src:<path>` or `<class>[:N[:density]]`" << std::endl;
  std::cerr << "  -a/--actuator <actuator-pattern>      Expects a Actuator Specifer of form `<class>[:tol[:maxIter]]`" << std::endl;
  std::cerr << "  -d/--dry-run                          Exit after parsing specifiers" << std::endl;
  std::cerr << "  -v/--verbose                          Verbose log" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Default <matrix-pattern> is `" << DEFAULT_MATRIX_PATTERN << "`" << std::endl;
  std::cerr << "Default <actuator-pattern> is `" << DEFAULT_ACTUATOR_PATTERN << "`" << std::endl;
  exit(0);
}

inline void ParseArguments(int argc, char** args, CliConfig& cli_config) {
  for (int32_t i = 1; i < argc; ++i) {
    std::string argument = args[i];
    if (argument == "-h" || argument == "--help") {
      PrintHelp(args[0]);
    } else if (argument == "-m" || argument == "--matrix") {
      if (i + 1 >= argc) {
        dcct::RaiseFatalError("expected matrix-pattern after " + argument);
      }
      argument = args[++i];
      cli_config.matrix_pattern = argument;
    } else if (argument == "-a" || argument == "--actuator") {
      if (i + 1 >= argc) {
        dcct::RaiseFatalError("expected actuator-pattern after " + argument);
      }
      argument = args[++i];
      cli_config.actuator_pattern = argument;
    } else if (argument == "-d" || argument == "--dry-run") {
      cli_config.dry_run = true;
    } else if (argument == "-v" || argument == "--verbose") {
      cli_config.verbose = true;
    } else {
      dcct::LogWarning("unknown option '" + argument + "'");
      PrintHelp(args[0]);
    }
  }
}

inline void UseSpecifierActuator(Eigen::MatrixXd& result,
                                 dcct::ActuatorSpecifier& actuator_specifier,
                                 Eigen::MatrixXd& M) {
  switch (actuator_specifier.type) {
    case actuator_specifier.SLOW: {
      dcct::SlowActuator actuator;
      result = actuator.dct2(M);
    }; break;
    case actuator_specifier.FAST: {
      dcct::FastActuator actuator;
      result = actuator.dct2(M);
    }; break;
    case actuator_specifier.FFTW: {
      dcct::FFTWActuator actuator;
      result = actuator.dct2(M);
    }; break;
    case actuator_specifier.NONE: {
    }; break;
  }
}

int main(int argc, char** args) {
  CliConfig cli_config;
  ParseArguments(argc, args, cli_config);

  dcct::MatrixSpecifier matrix_specifier = dcct::ParseMatrixSpecifier(cli_config.matrix_pattern);
  if (cli_config.verbose) {
    dcct::LogInfo("matrix.type:        " + dcct::ToString(matrix_specifier.type));
    dcct::LogInfo("matrix.ID:          " + matrix_specifier.ID);
    dcct::LogInfo("matrix.N:           " + std::to_string(matrix_specifier.N));
    dcct::LogInfo("matrix.M:           " + std::to_string(matrix_specifier.M));
  }
  
  dcct::ActuatorSpecifier actuator_specifier = dcct::ParseActuatorSpecifier(cli_config.actuator_pattern);
  if (cli_config.verbose) {
    dcct::LogInfo("actuator.type:      " + dcct::ToString(actuator_specifier.type));
    dcct::LogInfo("actuator.blockSize: " + std::to_string(actuator_specifier.blockSize));
    dcct::LogInfo("actuator.quality:   " + std::to_string(actuator_specifier.quality));
  }

  if (cli_config.dry_run)
    return 0;

  auto start = std::chrono::utc_clock::now();
  
  Eigen::MatrixXd X;
  dcct::FromMatrixSpecifier(X, matrix_specifier);
  
  auto round = std::chrono::utc_clock::now();
  std::cout << "Parse Matrix From Specifier | " << std::chrono::duration<double>(round - start) << std::endl;
  start = round;

  dcct::DumpMatrix(X, "output.X.mat");
  
  round = std::chrono::utc_clock::now();
  std::cout << "Dumped X Matrix             | " << std::chrono::duration<double>(round - start) << std::endl;
  start = round;

  Eigen::MatrixXd Y;
  UseSpecifierActuator(Y, actuator_specifier, X);
  
  round = std::chrono::utc_clock::now();
  std::cout << "Computed Y Matrix           | " << std::chrono::duration<double>(round - start) << std::endl;
  start = round;

  dcct::DumpMatrix(Y, "output.Y.mat");
  
  round = std::chrono::utc_clock::now();
  std::cout << "Dumped Y Matrix             | " << std::chrono::duration<double>(round - start) << std::endl;
  start = round;
  return 0;
}

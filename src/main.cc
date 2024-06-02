#include <dcct.hh>
#include <cassert>
#include <iostream>
#include <chrono>

const std::string DEFAULT_MATRIX_PATTERN = "src:input";
const std::string DEFAULT_ACTUATOR_PATTERN = "fttw:10:8";
const std::string DEFAULT_REPORT_FILEPATH = "report.json";

struct CliConfig {
  std::string matrix_pattern = DEFAULT_MATRIX_PATTERN;
  std::string actuator_pattern = DEFAULT_ACTUATOR_PATTERN;
  std::string report_filepath = DEFAULT_REPORT_FILEPATH;
  bool dry_run = false;
  bool verbose = false;
  bool benchmark = false;
  bool dct2 = false;
  bool idct2 = false;
};

inline void PrintHelp(std::string executable) {
  std::cerr << "Usage: " << executable << " [options]" << std::endl;
  std::cerr << "Options:" << std::endl;
  std::cerr << "  -m/--matrix <matrix-pattern>          Expects a Matrix Specifer of form `src:<path>` or `<class>[:N[:density]]`" << std::endl;
  std::cerr << "  -a/--actuator <actuator-pattern>      Expects a Actuator Specifer of form `<class>[:tol[:maxIter]]`" << std::endl;
  std::cerr << "  -r/--report <report-filepath>         Expects a json Report filepath" << std::endl;
  std::cerr << "  -d/--dry-run                          Exit after parsing specifiers" << std::endl;
  std::cerr << "  -v/--verbose                          Verbose log" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Actions:" << std::endl;
  std::cerr << "  -B/--benchmark                        Run a benchmark of Actuators" << std::endl;
  std::cerr << "  -D/--dct2                             Apply DCT2 to a given matrix" << std::endl;
  std::cerr << "  -I/--idct2                            Apply DCT2 to a given matrix" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Default settings:" << std::endl;
  std::cerr << "  <matrix-pattern>                      `" << DEFAULT_MATRIX_PATTERN << "`" << std::endl;
  std::cerr << "  <actuator-pattern>                    `" << DEFAULT_ACTUATOR_PATTERN << "`" << std::endl;
  std::cerr << "  <report-filepath>                     `" << DEFAULT_REPORT_FILEPATH << "`" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Possible Actuators are:" << std::endl;
  std::cerr << "  slow                                  Slow and \"naive\" implementation of DCT/IDCT trait" << std::endl;
  std::cerr << "  fast                                  More advanced but still N^3 implementation of DCT/IDCT trait" << std::endl;
  std::cerr << "  fftw                                  FFTW for C/C++ based implementation of DCT/IDCT trait" << std::endl;
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
    } else if (argument == "-r" || argument == "--report") {
      if (i + 1 >= argc) {
        dcct::RaiseFatalError("expected report-filepath after " + argument);
      }
      argument = args[++i];
      cli_config.report_filepath = argument;
    } else if (argument == "-d" || argument == "--dry-run") {
      cli_config.dry_run = true;
    } else if (argument == "-v" || argument == "--verbose") {
      cli_config.verbose = true;
    } else if (argument == "-B" || argument == "--benchmark") {
      cli_config.benchmark = true;
    } else if (argument == "-D" || argument == "--dct2") {
      cli_config.dct2 = true;
    } else if (argument == "-I" || argument == "--idct2") {
      cli_config.idct2 = true;
    } else {
      dcct::LogWarning("unknown option '" + argument + "'");
      PrintHelp(args[0]);
    }
  }
}

inline void UseDCT2(Eigen::MatrixXd& result,
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
      dcct::RaiseFatalError("actuator not specified");
    }; break;
  }
}

inline void UseIDCT2(Eigen::MatrixXd& result,
                    dcct::ActuatorSpecifier& actuator_specifier,
                    Eigen::MatrixXd& M) {
  switch (actuator_specifier.type) {
    case actuator_specifier.SLOW: {
      dcct::SlowActuator actuator;
      result = actuator.idct2(M);
    }; break;
    case actuator_specifier.FAST: {
      dcct::FastActuator actuator;
      result = actuator.idct2(M);
    }; break;
    case actuator_specifier.FFTW: {
      dcct::FFTWActuator actuator;
      result = actuator.idct2(M);
    }; break;
    case actuator_specifier.NONE: {
      dcct::RaiseFatalError("actuator not specified");
    }; break;
  }
}

int DoBenchmark(CliConfig& cli_config) {
  dcct::LogWarning("Not Implemented");

  dcct::Report report;
  // If doesn't exist, it will create it
  if (!dcct::LoadReport(report, cli_config.report_filepath)) {
    dcct::LogInfo("file " + cli_config.report_filepath + " doesn't exist, creating it");
    report.data.clear();
    report.filepath = cli_config.report_filepath;
  }

  dcct::Benchmark benchmark = {
    .minSize = 10,
    .maxSize = 100,
    .step = 10
  };
  dcct::ExecuteBenchmark(report, benchmark);

  dcct::DumpReport(report);
  
  return 0;
}

int DoDCT2(CliConfig& cli_config) {
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

  dcct::Timer timer;
  timer.reset();
  
  Eigen::MatrixXd X;
  dcct::FromMatrixSpecifier(X, matrix_specifier);
  if (cli_config.verbose)
    timer.round("Parse Matrix From Specifier");

  dcct::DumpMatrix(X, "output.X.bin", dcct::MatrixFileFormat::BIN);
  std::cout << X << std::endl;
  if (cli_config.verbose)
    timer.round("Dumped X Matrix");

  Eigen::MatrixXd Y;
  UseDCT2(Y, actuator_specifier, X);
  if (cli_config.verbose)
    timer.round("Computed Y Matrix");
  
  dcct::DumpMatrix(Y, "output.Y.bin", dcct::MatrixFileFormat::BIN);
  if (cli_config.verbose)
    timer.round("Dumped Y Matrix");
  
  return 0;
}

int DoIDCT2(CliConfig& cli_config) {
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

  dcct::Timer timer;
  timer.reset();
  
  Eigen::MatrixXd X;
  dcct::FromMatrixSpecifier(X, matrix_specifier);
  if (cli_config.verbose)
    timer.round("Parse Matrix From Specifier");

  dcct::DumpMatrix(X, "output.X.bin", dcct::MatrixFileFormat::BIN);
  if (cli_config.verbose)
    timer.round("Dumped X Matrix");

  Eigen::MatrixXd Y;
  UseIDCT2(Y, actuator_specifier, X);
  if (cli_config.verbose)
    timer.round("Computed Y Matrix");
  
  dcct::DumpMatrix(Y, "output.Y.bin", dcct::MatrixFileFormat::BIN);
  if (cli_config.verbose)
    timer.round("Dumped Y Matrix");
  
  return 0;
}

int main(int argc, char** args) {
  CliConfig cli_config;
  ParseArguments(argc, args, cli_config);

  if (cli_config.benchmark) {
    DoBenchmark(cli_config);
  } else if (cli_config.dct2) {
    DoDCT2(cli_config);
  } else if (cli_config.idct2) {
    DoIDCT2(cli_config);
  }

  return 0;
}
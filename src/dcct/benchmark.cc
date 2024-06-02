#include <dcct/benchmark.hh>
#include <dcct/logging.hh>
#include <dcct/actuator.hh>
#include <dcct/timer.hh>
#include <dcct/matrix.hh>
#include <dcct/slow_actuator.hh>
#include <dcct/fast_actuator.hh>
#include <dcct/fftw_actuator.hh>
#include <json/json.h>
#include <filesystem>
#include <fstream>
#include <cxxabi.h>
#include <string>

bool dcct::LoadReport(dcct::Report& report, const std::string& filepath) {
  if (!std::filesystem::exists(filepath)) {
    return false;
  }
  report.filepath = filepath;

  std::ifstream in;
  in.open(filepath);
  Json::Value data;
  Json::Reader text_reader;
  if (!(text_reader.parse(in, data))) {
    dcct::LogWarning("syntax error while reading " + filepath);
    return false;
  }
  in.close();

  if (!data.isObject()) {
    dcct::LogWarning("expected an object inside " + filepath);
    return false;
  }

  report.data.clear();
  for (auto subtable = data.begin(); subtable != data.end(); ++subtable) {
    std::string key = subtable.key().asString();
    report.data[key] = {};

    if (!subtable->isArray()) {
      dcct::LogWarning("expected an array as " + subtable->toStyledString() + " children of report inside " + filepath);
      return false;
    }

    for (auto cell = subtable->begin(); cell != subtable->end(); ++cell) {
      if (!cell->isObject()) {
        dcct::LogWarning("expected objects inside array " + subtable->toStyledString() + " child of report inside " + filepath);
        return false;
      }

      report.data[key].push_back(dcct::Report::Cell {
        .N = cell->operator[]("N").asUInt(),
        .M = cell->operator[]("M").asUInt(),
        .elapsed = cell->operator[]("elapsed").asFloat(),
      });
    }
  }

  return true;
}

bool dcct::DumpReport(const dcct::Report& report) {
  Json::Value dump = Json::objectValue;
  for (std::pair<std::string, std::vector<dcct::Report::Cell>> subtable : report.data) {
    Json::Value records = Json::arrayValue;
    for (dcct::Report::Cell cell : subtable.second) {
      Json::Value record = Json::objectValue;
      record["N"] = cell.N;
      record["M"] = cell.M;
      record["elapsed"] = cell.elapsed;
      records.append(record);
    }
    dump[subtable.first] = records;
  }

  Json::StreamWriterBuilder stream_writer_builder;
  Json::StreamWriter* json_writer = stream_writer_builder.newStreamWriter();
  std::ofstream out;
  out.open(report.filepath);
  json_writer->write(dump, &out);
  out.close();
  delete json_writer;

  return true;
}

bool dcct::LoadBenchmark(dcct::Benchmark& benchmark, const std::string& filepath) {
  if (!std::filesystem::exists(filepath)) {
    return false;
  }

  std::ifstream in;
  in.open(filepath);
  Json::Value data;
  Json::Reader text_reader;
  if (!(text_reader.parse(in, data))) {
    dcct::LogWarning("syntax error while reading " + filepath);
    return false;
  }
  in.close();

  if (!data.isObject()) {
    dcct::LogWarning("expected an object inside " + filepath);
    return false;
  }

  benchmark.minSize = data["minSize"].asUInt();
  benchmark.maxSize = data["maxSize"].asUInt();
  benchmark.step = data["step"].asUInt();

  return true;
}

bool dcct::DumpBenchmark(const dcct::Benchmark& benchmark, const std::string& filepath) {
  Json::Value dump = Json::objectValue;
  dump["minSize"] = benchmark.minSize;
  dump["maxSize"] = benchmark.maxSize;
  dump["step"] = benchmark.step;

  Json::StreamWriterBuilder stream_writer_builder;
  Json::StreamWriter* json_writer = stream_writer_builder.newStreamWriter();
  std::ofstream out;
  out.open(filepath);
  json_writer->write(dump, &out);
  out.close();
  delete json_writer;

  return true;
}

template<typename ActuatorImpl>
inline void RunAgainstMatrix(dcct::Report::Cell& result, Eigen::MatrixXd& matrix) {
  ImplTrait(ActuatorImpl, dcct::Actuator);
  dcct::Timer timer;
  timer.reset();
  ActuatorImpl actuator;
  (void)actuator.dct2(matrix);
  result.elapsed = timer.round();
}

template<typename ActuatorImpl>
inline void RunIfNotAlready(dcct::Report& report, Eigen::MatrixXd& benchmark_matrix) {
  ImplTrait(ActuatorImpl, dcct::Actuator);
  std::string label = abi::__cxa_demangle(typeid(ActuatorImpl).name(), nullptr, nullptr, nullptr);
  std::vector<dcct::Report::Cell>& cells = report.data[label];
  dcct::Report::Cell cell;
  cell.N = benchmark_matrix.rows();
  cell.M = benchmark_matrix.cols();

  auto it = std::find_if(cells.begin(), cells.end(), [&cell](const dcct::Report::Cell& existing_cell) {
    return (cell.N == existing_cell.N && cell.M == existing_cell.M);
  });
  if (it == cells.end()) {
    dcct::LogInfo("Executing for " + label + ": " + std::to_string(cell.N) + "x" + std::to_string(cell.M));
    RunAgainstMatrix<ActuatorImpl>(cell, benchmark_matrix);
    cells.push_back(cell);
  } else {
    dcct::LogInfo("Skipping for " + label + ": " + std::to_string(cell.N) + "x" + std::to_string(cell.M));
  }
}

void dcct::ExecuteBenchmark(dcct::Report& report, const dcct::Benchmark& benchmark) {
  dcct::MatrixSpecifier matrix_specifier = {
    .type = dcct::MatrixSpecifier::Type::RND,
    .ID = "",
    .N = 0,
    .M = 0
  };
  Eigen::MatrixXd benchmark_matrix;
  for (uint32_t size = benchmark.minSize; size <= benchmark.maxSize; size += benchmark.step) {
    matrix_specifier.N = size;
    matrix_specifier.M = size;
    dcct::LogInfo("Generating a Matrix: " + std::to_string(matrix_specifier.N) + "x" + std::to_string(matrix_specifier.M));
    dcct::FromMatrixSpecifier(benchmark_matrix, matrix_specifier);

    // RunIfNotAlready<dcct::SlowActuator>(report, benchmark_matrix);
    RunIfNotAlready<dcct::FastActuator>(report, benchmark_matrix);
    RunIfNotAlready<dcct::FFTWActuator>(report, benchmark_matrix);
  }
}

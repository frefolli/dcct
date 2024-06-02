#ifndef DCCT_BENCHMARK_HH
#define DCCT_BENCHMARK_HH
/** @file benchmark.hh benchmarking utility */
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <cmath>

namespace dcct {
  struct Report {
    struct Cell {
      uint32_t N;
      uint32_t M;
      double_t elapsed;
    };

    std::string filepath;
    std::unordered_map<std::string, std::vector<Cell>> data;
  };

  struct Benchmark {
    uint32_t minSize;
    uint32_t maxSize;
    uint32_t step;
  };

  /**
   * Loads a Report from json file
   * @param report output Report
   * @param filepath input file path
  */
  bool LoadReport(Report& report, const std::string& filepath);
  
  /**
   * Dumps a Report from json file
   * @param report input Report
   * @param filepath output file path
  */
  bool DumpReport(const Report& report);

  /**
   * Executes a benchmark and stores info inside the report
   * @param report output Report
   * @param benchmark input Benchmark
  */
  void ExecuteBenchmark(Report& report, const Benchmark& benchmark);
}
#endif//DCCT_BENCHMARK_HH
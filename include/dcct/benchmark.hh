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

  bool LoadReport(Report& report, const std::string& filepath);
  bool DumpReport(const Report& report);
}
#endif//DCCT_BENCHMARK_HH
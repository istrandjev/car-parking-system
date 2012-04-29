#ifndef UTILS_BENCHMARK_H
#define UTILS_BENCHMARK_H

#include <map>
#include <string>

namespace utils {
class Benchmark {
 public:
  Benchmark(const std::string& function_name);
  ~Benchmark();

  static void DumpBenchmarkingInfo();

 private:
  std::string functionName_;
  double startTime_;
  static std::map<std::string, double> timeMap_;
};

}  // namespace utils
#endif // UTILS_BENCHMARK_H

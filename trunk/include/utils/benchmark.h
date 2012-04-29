#ifndef UTILS_BENCHMARK_H
#define UTILS_BENCHMARK_H

#define DO_BENCHMARK

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

#ifdef DO_BENCHMARK
#define BENCHMARK(name) utils::Benchmark name(std::string(__FUNCTION__))
#define BENCHMARK_STR(name, x) utils::Benchmark name(std::string(__FUNCTION__)  + " [" + x + "]")
#else
#define BENCHMARK(name)
#define BENCHMARK_STR(name, x)
#endif
}  // namespace utils
#endif // UTILS_BENCHMARK_H

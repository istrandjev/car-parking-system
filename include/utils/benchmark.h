#ifndef UTILS_BENCHMARK_H
#define UTILS_BENCHMARK_H

#define DO_BENCHMARK

#include <vector>
#include <string>

namespace utils {
class Benchmark {
 public:
  struct BenchmarkItem {
    BenchmarkItem() : totalTime(0.0), numberOfTimes(0) {}
    BenchmarkItem(double total_time, int number_of_times) 
        : totalTime(total_time), numberOfTimes(number_of_times) {}

    double totalTime;
    int numberOfTimes;
    std::string name;
  };
  Benchmark(const std::string& function_name, int index);
  ~Benchmark();

  static void DumpBenchmarkingInfo();

 private:
  double startTime_;
  int index_;
  static std::vector<BenchmarkItem> timeTable_;
};

#define CONCATENATE_DIRECT(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_DIRECT(s1, s2)
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef DO_BENCHMARK
#define BENCHMARK_SCOPE utils::Benchmark ANONYMOUS_VARIABLE(bm)(\
    std::string(__FILE__) + ":"  + std::string(__FUNCTION__) +\
    "("TOSTRING(__LINE__)")", __COUNTER__)
#define BENCHMARK_STR(x) utils::Benchmark ANONYMOUS_VARIABLE(bm)(\
    std::string(__FILE__) + ":"  + std::string(__FUNCTION__)  +\
    "("TOSTRING(__LINE__)") [" + x + "]", __COUNTER__)
#else
#define BENCHMARK_SCOPE
#define BENCHMARK_STR(x)
#endif
}  // namespace utils
#endif // UTILS_BENCHMARK_H

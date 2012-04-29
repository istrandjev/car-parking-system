#include "utils/benchmark.h"

#include "utils/delay.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <string>

namespace utils {
// static declarations
std::map<std::string, double> Benchmark::timeMap_;

Benchmark::Benchmark(const std::string& function_name) : functionName_(function_name){
  startTime_ =  get_time();
}

Benchmark::~Benchmark() {
  double duration = get_time() - startTime_;
  std::map<std::string, double>::iterator it = timeMap_.find(functionName_);
  if (it == timeMap_.end()) {
    timeMap_[functionName_] = duration;
  } else {
    it->second += duration;
  }
}

// static
void Benchmark::DumpBenchmarkingInfo() {
  if (timeMap_.empty()) {
    return;
  }
  std::cerr << "### Dumping benchmark info ###\n";
  for (std::map<std::string, double>::iterator it = timeMap_.begin();
       it != timeMap_.end(); ++it) {
    std::cerr << it->first << ": " << std::setprecision(9) << it->second
              << std::endl;;
  }
}

}  // namespace utils

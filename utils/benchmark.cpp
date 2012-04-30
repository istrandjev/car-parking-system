#include "utils/benchmark.h"

#include "utils/delay.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

namespace utils {
// static declarations
std::map<std::string, Benchmark::BenchmarkItem> Benchmark::timeMap_;

Benchmark::Benchmark(const std::string& function_name) : functionName_(function_name){
  startTime_ =  get_time();
}

Benchmark::~Benchmark() {
  double duration = get_time() - startTime_;
  std::map<std::string, BenchmarkItem>::iterator it = timeMap_.find(functionName_);
  if (it == timeMap_.end()) {
    timeMap_[functionName_] = BenchmarkItem(duration, 1);
  } else {
    it->second.numberOfTimes++;
    it->second.totalTime += duration;
  }
}

// static
void Benchmark::DumpBenchmarkingInfo() {
  if (timeMap_.empty()) {
    return;
  }
  std::ofstream dump_file("../../resources/dump.txt", std::ios::app);
  std::cerr << "### Dumping benchmark info ###\n";
  dump_file << "### Dumping benchmark info ###\n";
  for (std::map<std::string, BenchmarkItem>::iterator it = timeMap_.begin();
       it != timeMap_.end(); ++it) {
    std::cerr << it->first << ": " << std::setprecision(9) 
        << it->second.totalTime << " (" << it->second.numberOfTimes << ")"
        << std::endl;
    dump_file << it->first << ": " << std::setprecision(9) 
        << it->second.totalTime << " (" << it->second.numberOfTimes << ")"
        << std::endl;;
  }
}

}  // namespace utils

#include "utils/benchmark.h"

#include "utils/delay.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace utils {
// static declarations
std::vector<Benchmark::BenchmarkItem> Benchmark::timeTable_;

Benchmark::Benchmark(const std::string& function_name, int index)
  : index_(index) {
  startTime_ =  get_time();
  if(index_ >= timeTable_.size()) {
    timeTable_.resize(index_ + 1);
  }
  if (timeTable_[index_].name.empty()) {
    timeTable_[index_].name = function_name;
  }
}

Benchmark::~Benchmark() {
  double duration = get_time() - startTime_;
  timeTable_[index_].numberOfTimes++;
  timeTable_[index_].totalTime += duration;
}

// static
void Benchmark::DumpBenchmarkingInfo() {
  if (timeTable_.empty()) {
    return;
  }
  std::ofstream dump_file("../../resources/dump.txt", std::ios::app);
  std::cerr << "### Dumping benchmark info ###\n";
  dump_file << "### Dumping benchmark info ###\n";
  for (unsigned index = 0; index < timeTable_.size(); ++index) {
    std::cerr << timeTable_[index].name << ": " << std::setprecision(9)
        << timeTable_[index].totalTime << " ("
        << timeTable_[index].numberOfTimes << ")"
        << std::endl;
    dump_file << timeTable_[index].name << ": " << std::setprecision(9)
        << timeTable_[index].totalTime << " ("
        << timeTable_[index].numberOfTimes << ")"
        << std::endl;
  }
}

}  // namespace utils

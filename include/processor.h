#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization() const;
  void UpdateCPUUtil();

 private:
  float CPUUtil_{};
  float prevIdleJiffies_{};
  float prevTotalJiffies_{};
};

#endif
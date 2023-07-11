#include "processor.h"

float Processor::Utilization() const { return CPUUtil_; }

void Processor::UpdateCPUUtil(){
  auto currentIdleJiffies= static_cast<float>(LinuxParser::IdleJiffies());
  auto currentTotalJiffies = static_cast<float>(LinuxParser::Jiffies());

  float totald = currentTotalJiffies - prevTotalJiffies_;
  float idled = currentIdleJiffies - prevIdleJiffies_;

  CPUUtil_ = (totald - idled)/totald;

  prevIdleJiffies_ = currentIdleJiffies;
  prevTotalJiffies_ = currentTotalJiffies;
}
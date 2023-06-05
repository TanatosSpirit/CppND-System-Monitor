#include "processor.h"

float Processor::Utilization() const { return CPUUtil_; }

void Processor::UpdateCPUUtil(std::vector<std::string> state){
  CPUState current_state(state);

  PrevState_.idle = PrevState_.idle + PrevState_.iowait;
  current_state.idle = current_state.idle + current_state.iowait;
  float PrevNonIdle = PrevState_.user + PrevState_.nice + PrevState_.system +
                      PrevState_.irq + PrevState_.softirq + PrevState_.steal;
  float NonIdle = current_state.user + current_state.nice + current_state.system +
                current_state.irq + current_state.softirq + current_state.steal;

  float PrevTotal = PrevState_.idle + PrevNonIdle;
  float Total = current_state.idle + NonIdle;

  float totald = Total - PrevTotal;
  float idled = current_state.idle - PrevState_.idle;

  CPUUtil_ = (totald - idled)/totald;
  PrevState_ = CPUState(state);
}
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization() const;
  void UpdateCPUUtil(std::vector<std::string> current_state);

 private:
  float CPUUtil_{};

  struct CPUState{
    float user{};
    float nice{};
    float system{};
    float idle{};
    float iowait{};
    float irq{};
    float softirq{};
    float steal{};
    float guest{};
    float guest_nice{};

    CPUState() = default;

    CPUState(std::vector<std::string> &state){
      user = stof(state.at(0));
      nice = stof(state.at(1));
      system = stof(state.at(2));
      idle = stof(state.at(3));
      iowait = stof(state.at(4));
      irq = stof(state.at(5));
      softirq = stof(state.at(6));
      steal = stof(state.at(7));
      guest = stof(state.at(8));
      guest_nice = stof(state.at(9));
    }
  };

  CPUState PrevState_{};
};

#endif
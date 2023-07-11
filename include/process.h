#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid){};
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  void setUser(std::string name);
  void setCommand(std::string command);
  void setRAMUsage(std::string ram);
  void setUpTime(long int uptime);


 private:
  int pid_;
  std::string name_;
  std::string command_;
  std::string ram_;
  long int uptime_;
};

#endif
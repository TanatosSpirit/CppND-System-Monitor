#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

System::System() {
  os_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
}

Processor& System::Cpu() {
  cpu_.UpdateCPUUtil(LinuxParser::CpuUtilization());
  return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  vector<Process> out{};
  out.reserve(pids.size());
  for(int const &pid:pids){
    Process tmp(pid);
    std::string uid = LinuxParser::Uid(pid);
    tmp.setUser(uid);
    out.emplace_back(tmp);
  }
  processes_ = out;
  return processes_;
//  return out;
}

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return os_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }

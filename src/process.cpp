#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
  command_ = LinuxParser::Command(pid);
  name_ = LinuxParser::User(LinuxParser::Uid(pid));
  ram_ = LinuxParser::Ram(pid);
  uptime_ = LinuxParser::UpTime(pid);
  // TODO calc current cpu util
  cpuutil_ = static_cast<float>(LinuxParser::ActiveJiffies(pid))/
             static_cast<float>(uptime_);
}

int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuutil_; }

string Process::Command() { return command_; }

string Process::Ram() const { return ram_; }

string Process::User() { return name_; }

long int Process::UpTime() const { return uptime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
//  return CpuUtilization() < a.CpuUtilization();
  long first = stol(Ram());
  long second = stol(a.Ram());
  return  first < second;
}

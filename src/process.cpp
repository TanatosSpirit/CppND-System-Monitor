#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
  command_ = LinuxParser::Command(pid);
  name_ = LinuxParser::User(LinuxParser::Uid(pid));
  ram_ = LinuxParser::Ram(pid);
  uptime_ = LinuxParser::UpTime(pid);
  auto current_active = static_cast<float>(LinuxParser::ActiveJiffies(pid));
  auto current_total = static_cast<float>(uptime_);

  cpuutil_ = (current_active - prev_active_) / (current_total - prev_total_);

  prev_active_ = current_active;
  prev_total_ = current_total;
}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuutil_; }

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() const { return ram_; }

// Return the user (name) that generated this process
string Process::User() { return name_; }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
  //  long first = stol(Ram());
  //  long second = stol(a.Ram());
  //  return  first < second;
}

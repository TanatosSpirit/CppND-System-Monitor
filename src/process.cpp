#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;


int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {

  return 0;
}

string Process::Command() { return command_; }

void Process::setCommand(std::string command) {command_ = command;}

string Process::Ram() { return ram_; }

void Process::setRAMUsage(string ram) {ram_ = ram;}

string Process::User() { return name_; }

void Process::setUser(std::string name) {name_ = std::move(name);}

long int Process::UpTime() { return uptime_; }

void Process::setUpTime(long int uptime) { uptime_ = uptime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
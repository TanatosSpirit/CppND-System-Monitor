#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// TODO: BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  float mem_util{};
  float mem_total, mem_free;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream1(line);
    linestream1 >> key >> value;
    mem_total = static_cast<float>(stoi(value));
    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> key >> value;
    mem_free = static_cast<float>(stoi(value));

    mem_util = (mem_total - mem_free)/mem_total;
  }
  return mem_util;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  long output{};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    output = stol(value);
    }
  return output;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> values;
  getProcessStat(pid, values);
  long out{};

  if(!values.empty()){
    long total = stol(values.at(13)) + stol(values.at(14)) +
                 stol(values.at(15)) + stol(values.at(16));

    out =  total / sysconf(_SC_CLK_TCK);
  }

  return out;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto jiffies = LinuxParser::CpuUtilization();

  return stol(jiffies[CPUStates::kUser_]) +
         stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) +
         stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto jiffies = LinuxParser::CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]) +
         stol(jiffies[CPUStates::kIOwait_]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    return {++begin, end};
  }
  return {};
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  int output{};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes"){
        output = stoi(value);
      }
    }
  }
  return output;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  int output{};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running"){
        output = stoi(value);
      }
    }
  }
  return output;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string();
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        return to_string(stoi(value)/1000);
      }
    }
  }
  return "0";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "Uid:") {
          return value;
        }
    }
  }
  return string();
}

// Read and return the user associated with a process
string LinuxParser::User(string pid) {
  string line, key, value, x;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> value >> x >> key;
        if (key == pid) {
          return value;
        }
    }
  }
  return string();
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  vector<string> values;
  getProcessStat(pid, values);
  long out{};
  if(!values.empty()){
    long uptime = stol(values.back());
    uptime = uptime / sysconf(_SC_CLK_TCK);
    out = LinuxParser::UpTime() - uptime;
  }
  return out;
}

// Read and return process stat
void LinuxParser::getProcessStat(int const pid, std::vector<std::string> &output){
  string line, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    size_t index_start{}, index_end{};
    for(size_t i = 0; i < line.size(); i++){
        if(line.at(i) == '(')
          index_start = i;
        if(line.at(i) == ')') {
          index_end = i;
          break;
        }
    }
    line.erase(index_start, index_end - index_start + 1);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
        linestream >> value;
        output.push_back(value);
    }
  }
}

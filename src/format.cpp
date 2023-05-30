#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string time{};
  long hh = seconds/(60*60);
  time += (hh < 10 ? "0" + std::to_string(hh) : std::to_string(hh)) + ":";
  long mm = (seconds/60)%60;
  time += (mm < 10 ? "0" + std::to_string(mm) : std::to_string(mm)) + ":";
  long ss = seconds%60;
  time += ss < 10 ? "0" + std::to_string(ss) : std::to_string(ss);
  return time;
}
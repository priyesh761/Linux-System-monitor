#include <string>

#include "format.h"

using std::string;

string pad(string t) { return (t.length() < 2) ? "0" + t : t; }
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) {
  long hours = seconds / 3600;
  seconds %= 3600;
  long minutes = seconds / 60;
  seconds %= 60;

  string time;
  time += pad(std::to_string(hours)) + ":";
  time += pad(std::to_string(minutes)) + ":";
  time += pad(std::to_string(seconds));

  return time;
}
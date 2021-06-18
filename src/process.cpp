#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) { Process::pid_ = pid; }

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  long upTime = this->UpTime();
  long activeTime = LinuxParser::ActiveJiffies(this->pid_);
  activeTime /= sysconf(_SC_CLK_TCK);
  float cpuUtilization = (activeTime);
  cpuUtilization /= (upTime);

  //  prevUptime = upTime;
  // prevActiveTime = activeTime;
  return cpuUtilization;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(this->pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process& a) {
  return (this->CpuUtilization()) > (a.CpuUtilization());
}
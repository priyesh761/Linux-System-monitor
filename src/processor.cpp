#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  totalJiffies = LinuxParser::Jiffies();
  idleJiffies = LinuxParser::IdleJiffies();

  long totalTime = (totalJiffies - prevTotalJiffies);
  long idleTime = (idleJiffies - prevIdleJiffies);
  float cpuUtilization = ((totalTime - idleTime) * 1.0);
  cpuUtilization /= totalTime;

  prevIdleJiffies = idleJiffies;
  prevTotalJiffies = totalJiffies;

  return cpuUtilization;
}
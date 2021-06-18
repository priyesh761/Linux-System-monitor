#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
// Cpu Frequency
long cpuFrequency = LinuxParser::CpuFrequency();
// Returns CPU frequency
long LinuxParser::CpuFrequency() {
  std::ifstream file(kProcDirectory + kCpuinfoFilename);
  string line, tmp;
  string key = "cpu MHz";
  float frequency = 0;
  if (file.is_open()) {
    while (getline(file, line)) {
      if (line.find(key) != string::npos) {
        int ind = line.find(":");
        frequency = stof(line.substr(ind + 1));
      }
    }
  }
  return frequency * 1000000;  // convert MHz to Hz
}

// read and return  Operating system name
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

// read and return kernal version
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

// BONUS: Update this to use std::filesystem
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
  int totalMemory = 0;
  int freeMemory = 0;
  float memUtilization = 0.0;
  std::ifstream file(kProcDirectory + kMeminfoFilename);

  std::string line;
  if (file.is_open()) {
    while (getline(file, line)) {
      std::istringstream stream(line);
      std::string key;
      long long value;
      stream >> key >> value;
      if (key == "MemTotal:") totalMemory = value;
      if (key == "MemFree:") freeMemory = value;
    }
  }
  memUtilization = ((totalMemory - freeMemory) * 1.0) / totalMemory;
  return memUtilization;
}

// Read and return system Uptime
long LinuxParser::UpTime() {
  std::ifstream file(kProcDirectory + kUptimeFilename);
  std::string line;
  long time = 0;
  if (file.is_open() && getline(file, line)) {
    std::istringstream stream(line);
    stream >> time;
  }
  return time;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<unsigned long long> allStats = CpuUtilization();
  long long jiffies = 0;
  for (int i = 1; i <= 8; i++) jiffies += allStats[i];
  return jiffies;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream file(kProcDirectory + '/' + to_string(pid) + kStatFilename);
  long long totalRunningTime = 0;  // utime+stime+cutime+sctime
  long long temp = 0;
  string tmp;
  if (file.is_open()) {
    string line;
    getline(file, line);
    std::istringstream stream(line);
    int i = 0;
    while (i < 17) {
      if (i != 13 && i != 14 && i != 15 && i != 16)
        stream >> tmp;
      else {
        stream >> temp;
        totalRunningTime += temp;
      }
      i++;
    }
  }
  return totalRunningTime;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<unsigned long long> allStats = CpuUtilization();
  long long activeJiffies = 0;

  activeJiffies = allStats[0] + allStats[2];
  return activeJiffies;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<unsigned long long> allStats = CpuUtilization();
  long long idleJiffies = 0;

  idleJiffies = allStats[3];
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<unsigned long long> LinuxParser::CpuUtilization() {
  vector<unsigned long long> V;
  std::ifstream file(kProcDirectory + kStatFilename);
  std::string line;
  unsigned long long temp;

  if (file.is_open() && getline(file, line)) {
    std::istringstream stream(line);
    int i = 0;
    stream >> line;
    while (i < 10) {
      stream >> temp;
      V.push_back(temp);
      i++;
    }
  }
  return V;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream file(kProcDirectory + kStatFilename);

  int noOfProcesses = 0;
  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      string key;
      std::istringstream stream(line);
      stream >> key;
      if (key == "processes") {
        stream >> noOfProcesses;
        break;
      }
    }
  }
  return noOfProcesses;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream file(kProcDirectory + kStatFilename);

  int noOfRunningProcesses = 0;
  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      string key;
      std::istringstream stream(line);
      stream >> key;
      if (key == "procs_running") {
        stream >> noOfRunningProcesses;
        break;
      }
    }
  }
  return noOfRunningProcesses;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream file(kProcDirectory + '/' + to_string(pid) + kCmdlineFilename);
  string command;
  if (file.is_open()) {
    getline(file, command);
    std::istringstream stream(command);
    stream >> command;
  }
  return command;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream file(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
  string key, line;
  long long value;
  if (file.is_open()) {
    while (getline(file, line)) {
      std::istringstream stream(line);
      stream >> key;
      if (key == "VmSize:") {
        stream >> value;
        break;
      }
    }
  }
  value /= 1024;  // Convert Kb to Mb
  return to_string(value);
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream file(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
  string key, value, line;
  if (file.is_open()) {
    while (getline(file, line)) {
      std::istringstream stream(line);
      stream >> key;
      if (key == "Uid:") {
        stream >> value;
        break;
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::ifstream file(kPasswordPath);
  string line, username, permission;
  int uid = std::stoi(Uid(pid));
  int compUid;
  while (getline(file, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream stream(line);
    stream >> username >> permission >> compUid;
    if (compUid == uid) break;
  }
  return username;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream file(kProcDirectory + '/' + to_string(pid) + kStatFilename);
  long long startTime = 0;  // utime+stime+cutime+sctime
  string tmp;
  if (file.is_open()) {
    string line;
    getline(file, line);
    std::istringstream stream(line);
    int i = 0;
    while (i < 21) {
      stream >> tmp;
      i++;
    }
    stream >> startTime;
  }
  return LinuxParser::UpTime() - (startTime / sysconf(_SC_CLK_TCK));
}

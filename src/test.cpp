/*#include "../include/linux_parser.h"
#include<fstream>
#include<iostream>
#include "../include/format.h"

long LinuxParser::UpTime() {
  std::ifstream file(kProcDirectory+kUptimeFilename);
  std::string line;
  long time=0;
  if(file.is_open() && getline(file,line))
  {
    std::istringstream stream(line);
    stream>>time;
  }
  return time;
}

int main(){

    std::cout<<("Up Time: " +
Format::ElapsedTime(LinuxParser::UpTime())).c_str()<<'\n';
}*/
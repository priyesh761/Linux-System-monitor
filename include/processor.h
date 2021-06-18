#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  unsigned long long totalJiffies{0};
  unsigned long long idleJiffies{0};
  unsigned long long prevTotalJiffies{0};
  unsigned long long prevIdleJiffies{0};
};

#endif
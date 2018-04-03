#ifndef GLOBAL_H
#define GLOBAL_H
#include "vector"
#include "queue"
#include "process.h"
#include "algorithm"

using namespace std;
extern vector<process> processesInput;
extern vector<process> PProcessesInput;
extern int TQ;
extern bool noPremption;
extern bool RoundRobin;
extern queue<process> deletedProcesses;// you can add undo easily later
extern double totalCPUTime;
extern double timeToStartNextProcess;
extern double AWT;
extern double AT;
extern double CPUB;

#endif // GLOBAL_H

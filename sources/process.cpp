#include "process.h"


process::process(int pn, double cpu, double at)
{
    processNumber=pn;
    CPUBurst=cpu;
    arrivalTime=at;
}

process::process()
{
    processNumber=-1;
}
int process::getProcessNumber()
{
    return processNumber;
}

double process::getCPUBurst()
{
    return CPUBurst;
}

double process::getCPUBurstLeft()
{
    return CPUBurstLeft;
}

double process::getArrivalTime()
{
    return arrivalTime;
}

int process::getPriorityVal()
{
    return priorityVal;
}

void process::setProcessNumber(int pn)
{
    processNumber=pn;
}

void process::setCPUBurst(double cpu)
{
    CPUBurst=cpu;
}

void process::setCPUBurstLeft(double cpuLeft)
{
    CPUBurstLeft=cpuLeft;
}

void process::setArrivalTime(double at)
{
    arrivalTime=at;
}

void process::setPriorityVal(int pr)
{
    priorityVal=pr;
}


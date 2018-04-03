#ifndef PROCESS_H
#define PROCESS_H
class process
{
    int processNumber;
    double CPUBurst;
    double arrivalTime;
    int priorityVal;
    double CPUBurstLeft;

public:
    process(int pn,double cpu,double at);
    process();

    int getProcessNumber();
    double getCPUBurst();
    double getCPUBurstLeft();
    double getArrivalTime();
    int getPriorityVal();

    void setProcessNumber(int pn);
    void setCPUBurst(double cpu);
    void setCPUBurstLeft(double cpuLeft);
    void setArrivalTime(double at);
    void setPriorityVal(int pr);

};
#endif // PROCESS_H

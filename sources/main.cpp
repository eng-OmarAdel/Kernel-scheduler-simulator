#include "widget.h"
#include <QApplication>
#include "global.h"

using namespace std;
vector<process> processesInput;
queue<process> deletedProcesses;
vector<process>PProcessesInput;
double totalCPUTime=0;
double timeToStartNextProcess=0;
bool noPremption;
bool RoundRobin;
int TQ;
double AWT=0;
double CPUB=0,AT=0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();


    return a.exec();
}

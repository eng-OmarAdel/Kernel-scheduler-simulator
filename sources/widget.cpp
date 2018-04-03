#include "widget.h"
#include "ui_widget.h"
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->ATLineEdit->setValidator(new QDoubleValidator(0,10000000,2,this));
    ui->CPUBLineEdit->setValidator(new QDoubleValidator(0,10000000,2,this));
    ui->TQLineEdit->setValidator(new QDoubleValidator(0,10000000,2,this));
    ui->PriorityLineEdit->setValidator(new QDoubleValidator(0,10000000,2,this));
}

Widget::~Widget()
{
    delete ui;
}

bool FCFS(process p1, process p2)
{
    return (p1.getArrivalTime()<p2.getArrivalTime());
}

bool SJF(process p1, process p2)
{
    return (p1.getCPUBurstLeft() < p2.getCPUBurstLeft());
}

bool prioritize(process p1, process p2)
{
    return (p1.getPriorityVal() <= p2.getPriorityVal());
}

void SJFSort(vector<process>::iterator it1,vector<process>::iterator it2)
{
    int j;
    sort(it1,it2,FCFS);
    timeToStartNextProcess+= (*it1).getCPUBurst();
    for(int i=1;i<(it2-it1);i++)
    {

        //loop until i find the last element that can be executed within this time range
        for(j=i;j<(it2-it1);j++)
        {
            if( (*(it1+j)).getArrivalTime()>timeToStartNextProcess)
                break;
        }
        sort(it1+i, it1+j,SJF);
        timeToStartNextProcess+=(*(it1+i)).getCPUBurst();
    }
    timeToStartNextProcess=0;
}
void prioritySort(vector<process>::iterator it1,vector<process>::iterator it2)
{
    int j;
        sort(it1, it2, FCFS);
        timeToStartNextProcess += (*it1).getCPUBurst();
        for (int i = 0; i<(it2 - it1); i++)
        {

            //loop until i find the last element that can be executed within this time range
            for (j = i; j<(it2 - it1); j++)
            {
                if ((*(it1 + j)).getArrivalTime()>timeToStartNextProcess)
                    break;
            }
            sort(it1 + i, it1 + j, prioritize);
            if( ( it1 + i + 1 ) != it2 )
                timeToStartNextProcess += (*(it1 + i +1)).getCPUBurst();
        }
        timeToStartNextProcess = 0;
}



void Widget::on_SIMButton_clicked()
{
    timeToStartNextProcess=0;
    if(processesInput.size()>0)
    {
        AT=0;
        CPUB=0;
        if(ui->FCFSRadioButton->isChecked())
        {
            sort(processesInput.begin(),processesInput.end(),FCFS);

            noPremption=true;
        }
        else if(ui->SJFNPRadioButton->isChecked())
        {
            noPremption=true;
            SJFSort(processesInput.begin(),processesInput.end());
            /*
            int j;
            sort(processesInput.begin(),processesInput.end(),FCFS);
            timeToStartNextProcess+=processesInput[0].getCPUBurst();
            for(int i=1;i<processesInput.size();i++)
            {

                //loop until i find the last element that can be executed within this time range
                for(j=i;j<processesInput.size();j++)
                {
                    if(processesInput[j].getArrivalTime()>timeToStartNextProcess)
                        break;
                }
                sort(processesInput.begin()+i,processesInput.begin()+j,SJF);
                timeToStartNextProcess+=processesInput[i].getCPUBurst();
            }
            timeToStartNextProcess=0;
            */
        }
        else if(ui->PriorityNPRadioButton->isChecked())
        {
            noPremption=true;
            timeToStartNextProcess=processesInput[0].getCPUBurst();
            prioritySort(processesInput.begin()+1,processesInput.end());
        }
        else if(ui->RRRadioButton->isChecked())
        {
            noPremption=false;
            RoundRobin=true;
            TQ=stod(ui->TQLineEdit->text().toStdString());
            PProcessesInput=processesInput;

            int flag=1;
            while(flag)
            {
                flag=0;
                for(int i=0;i<processesInput.size();i++)
                {
                    if(PProcessesInput[i].getCPUBurstLeft()>TQ)
                    {
                       PProcessesInput[i].setCPUBurstLeft(PProcessesInput[i].getCPUBurstLeft()-TQ);
                       PProcessesInput.push_back(PProcessesInput[i]);
                       flag=1;
                    }
                }
            }

            for(int i=0;i<processesInput.size();i++)
            {
                PProcessesInput[i].setCPUBurstLeft(PProcessesInput[i].getCPUBurst());
            }

        }
        else if(ui->SJFPRadioButton->isChecked())
        {
            noPremption=false;
            RoundRobin=false;
            sort(processesInput.begin(),processesInput.end(),FCFS);
            // summation of AT & CPUBurst for AWT
            for(int k=0;k<processesInput.size();k++)
            {
                AT+=processesInput[k].getArrivalTime();
                CPUB+=processesInput[k].getCPUBurst();
            }
            /******now we need to handle the preemption***/
            PProcessesInput=processesInput;
            timeToStartNextProcess+=PProcessesInput[0].getCPUBurst();
            for(int i=1;i<PProcessesInput.size();i++)//check how to prevent infinite loop here due to insertion
            {
                double arrivalTimeOfNextProcess=PProcessesInput[i].getArrivalTime();
                double nextProcessCPUBurstLeft=PProcessesInput[i].getCPUBurstLeft();
                double timeTakenByCurrentProcess = arrivalTimeOfNextProcess - PProcessesInput[i-1].getArrivalTime();//2
                if( (arrivalTimeOfNextProcess < timeToStartNextProcess) && ( nextProcessCPUBurstLeft < ( PProcessesInput[i-1].getCPUBurstLeft() - timeTakenByCurrentProcess ) )  )//preemption happens
                {
                    int j;
                    //remaining part of the process
                    process processAfterPreemption = PProcessesInput[i-1];
                    processAfterPreemption.setCPUBurstLeft(timeToStartNextProcess - arrivalTimeOfNextProcess);//8
                    processAfterPreemption.setArrivalTime( arrivalTimeOfNextProcess + nextProcessCPUBurstLeft);//7

                    timeToStartNextProcess = timeToStartNextProcess-processAfterPreemption.getCPUBurstLeft()+nextProcessCPUBurstLeft;

                    //insert it after next process

                    if(processAfterPreemption.getCPUBurstLeft() > 0)
                        PProcessesInput.insert(PProcessesInput.begin()+i+1,processAfterPreemption);

                    /**********Sorting the processes that can start NOW according to SJF**********/

                    for(j=i+1;j<PProcessesInput.size();j++)
                    {
                        if( PProcessesInput[j].getArrivalTime()>timeToStartNextProcess)
                            break;
                    }
                    //i don't remember why i made timeToStartNextProcess a global variable but i don't want to miss with it now
                    int saveTimeToStartNextProcess=timeToStartNextProcess;
                    SJFSort(PProcessesInput.begin()+i+1,PProcessesInput.begin()+j);//tempers with timeToStart
                    timeToStartNextProcess=saveTimeToStartNextProcess;

                    /***************************************************************/

                    //PProcessesInput[i-1] represents the executed part of the preempted process
                    if(PProcessesInput[i-1].getCPUBurstLeft()-processAfterPreemption.getCPUBurstLeft()>0)
                        PProcessesInput[i-1].setCPUBurstLeft(PProcessesInput[i-1].getCPUBurstLeft()-processAfterPreemption.getCPUBurstLeft() );//executed (wrong naming as CPUBurstLeft but it makes it easier for drawing code)
                    else
                        PProcessesInput[i-1].setCPUBurstLeft(0);
                }
                //if no preemption we have to update timeToStart
                //if no preemption due to higher CPUBurst we have to edit the arrivalTime and set it to executionTime
                else if((arrivalTimeOfNextProcess < timeToStartNextProcess))
                {
                    PProcessesInput[i].setArrivalTime(timeToStartNextProcess);
                    timeToStartNextProcess += nextProcessCPUBurstLeft;
                }
                else
                    timeToStartNextProcess = arrivalTimeOfNextProcess + nextProcessCPUBurstLeft;
                    //timeToStartNextProcess += nextProcessCPUBurstLeft;
            }
        }
        else if(ui->PriorityPRadioButton->isChecked())
        {
            noPremption=false;
            RoundRobin=false;

            sort(processesInput.begin(),processesInput.end(),FCFS);
            // summation of AT & CPUBurst for AWT
            for(int k=0;k<processesInput.size();k++)
            {
                AT+=processesInput[k].getArrivalTime();
                CPUB+=processesInput[k].getCPUBurst();
            }
            /******now we need to handle the preemption***/
            PProcessesInput=processesInput;
            timeToStartNextProcess+=PProcessesInput[0].getCPUBurst();//10
            for(int i=1;i<PProcessesInput.size();i++)
            {//i=2
                double arrivalTimeOfNextProcess=PProcessesInput[i].getArrivalTime();//6
                double nextProcessCPUBurstLeft=PProcessesInput[i].getCPUBurstLeft();//2
                //delwa2ty el moshkela en el ta3'yerat el 7asalet fel loop beta3et p1 kanet 3ala 2asas en el ba3deeha
                //p2 msh p3 fa lamma enta baddelt p3 ma3 p2 el ta3'yerat de matzabbatetsh 3ala 2asas el tarteeb el gdeed
                //fa enta me7tag tozbot el mawdoo3 da, (3'aleban el timeToStart we setArrivalTime homa el feehom el
                //moshkela)
                if( (arrivalTimeOfNextProcess/*6*/ < timeToStartNextProcess/*7*/) && ( PProcessesInput[i].getPriorityVal() < PProcessesInput[i-1].getPriorityVal() )  )//preemption happens
                {
                    int j;
                    //remaining part of the process
                    process processAfterPreemption = PProcessesInput[i-1];
                    processAfterPreemption.setCPUBurstLeft(timeToStartNextProcess - arrivalTimeOfNextProcess);//1
                    processAfterPreemption.setArrivalTime( arrivalTimeOfNextProcess + nextProcessCPUBurstLeft);//8

                    timeToStartNextProcess = timeToStartNextProcess-processAfterPreemption.getCPUBurstLeft()+nextProcessCPUBurstLeft;
//8
                    //insert it after next process

                    if(processAfterPreemption.getCPUBurstLeft() > 0)
                        PProcessesInput.insert(PProcessesInput.begin()+i+1,processAfterPreemption);

                    /**********Sorting the processes that may preempt next process or not according to PriorityVal**********/
//delwa2ty fe moshkela en P2 etra7alet fel vector bas el arrivalTime beta3ha matzabbatsh(etbaddelet ma3 P3)
                    for(j=i+1;j<PProcessesInput.size();j++)
                    {
                        if( PProcessesInput[j].getArrivalTime()>timeToStartNextProcess)
                            break;
                    }
                    //i don't remember why i made timeToStartNextProcess a global variable but i don't want to miss with it now
                    int saveTimeToStartNextProcess=timeToStartNextProcess;
                    prioritySort(PProcessesInput.begin()+i+1,PProcessesInput.begin()+j);//tempers with timeToStart
                    timeToStartNextProcess=saveTimeToStartNextProcess;

                    /***************************************************************/

                    //PProcessesInput[i-1] represents the executed part of the preempted process
                    if(PProcessesInput[i-1].getCPUBurstLeft()-processAfterPreemption.getCPUBurstLeft()>0)
                        PProcessesInput[i-1].setCPUBurstLeft(PProcessesInput[i-1].getCPUBurstLeft()-processAfterPreemption.getCPUBurstLeft() );//executed (wrong naming as CPUBurstLeft but it makes it easier for drawing code)
                    else
                        PProcessesInput[i-1].setCPUBurstLeft(0);
                }
                //if no preemption we have to update timeToStart
                //if no preemption due to higher CPUBurst we have to edit the arrivalTime and set it to executionTime
                else if((arrivalTimeOfNextProcess/*4*/ < timeToStartNextProcess/*7*/))//efred gat heya heya tany? zabbat flags
                {
                    int j;
                    for(j=i+1;j<PProcessesInput.size();j++)
                    {
                        if( PProcessesInput[j].getArrivalTime()>timeToStartNextProcess)
                            break;
                    }
                    //at 6 P3 will preempt P1

                    int saveTimeToStartNextProcess=timeToStartNextProcess;
                    prioritySort(PProcessesInput.begin()+i,PProcessesInput.begin()+j);
                    timeToStartNextProcess=saveTimeToStartNextProcess;

                    //No preemption should happen (all processes within time range have lower priority(higher value))
                    if(PProcessesInput[i].getPriorityVal() >= PProcessesInput[i-1].getPriorityVal() )
                    {
                        PProcessesInput[i].setArrivalTime(timeToStartNextProcess);
                        timeToStartNextProcess += nextProcessCPUBurstLeft;
                    }
                    //Preemption should happen
                    else
                        i--;//There's a process that should make a preemption to the current process but it's not the
                    //next one in the vector so we sorted the vector and started the loop from same position
                }
                else
                    timeToStartNextProcess = arrivalTimeOfNextProcess + nextProcessCPUBurstLeft;

            }
        }
    }
    simulation simWindow;
    simWindow.setModal(true);
    simWindow.exec();
}


void Widget::on_AddButton_clicked()
{
    process tempProcess;

    tempProcess.setArrivalTime(std::stod(ui->ATLineEdit->text().toStdString()));
    tempProcess.setCPUBurst(std::stod(ui->CPUBLineEdit->text().toStdString()));
    tempProcess.setCPUBurstLeft(std::stod(ui->CPUBLineEdit->text().toStdString()));
    if(ui->PriorityLineEdit->isEnabled())
        tempProcess.setPriorityVal(std::stod(ui->PriorityLineEdit->text().toStdString()));
    if(deletedProcesses.empty())
    {
        tempProcess.setProcessNumber( ui->PNComboBox->currentIndex());

        if(ui->PNComboBox->currentIndex() >= ui->PNComboBox->count()-1)
        {
            processesInput.push_back(tempProcess);
            ui->PNComboBox->addItem(QString::number(ui->PNComboBox->count()));
            ui->PNComboBox->setCurrentIndex(ui->PNComboBox->count()-1);
        }
        else
            processesInput[ui->PNComboBox->currentIndex()]=tempProcess;
    }
    else
    {
        tempProcess.setProcessNumber(deletedProcesses.front().getProcessNumber());
        if(ui->PNComboBox->currentIndex() >= ui->PNComboBox->count()-1)
        {
            processesInput.push_back(tempProcess);
            ui->PNComboBox->addItem(QString::number(deletedProcesses.front().getProcessNumber()));
            ui->PNComboBox->setCurrentIndex(ui->PNComboBox->count()-1);
        }
        else
            processesInput[ui->PNComboBox->currentIndex()]=tempProcess;

        deletedProcesses.pop();
    }

    totalCPUTime+=tempProcess.getCPUBurst();
}

void Widget::on_PriorityNPRadioButton_clicked()
{
    ui->PriorityLineEdit->setEnabled(true);
    ui->TQLineEdit->setDisabled(true);
}

void Widget::on_PriorityPRadioButton_clicked()
{
    ui->PriorityLineEdit->setEnabled(true);
    ui->TQLineEdit->setDisabled(true);
}

void Widget::on_RRRadioButton_clicked()
{
    ui->PriorityLineEdit->setDisabled(true);
    ui->TQLineEdit->setEnabled(true);
}

void Widget::on_FCFSRadioButton_clicked()
{
    ui->PriorityLineEdit->setDisabled(true);
    ui->TQLineEdit->setDisabled(true);
}

void Widget::on_SJFNPRadioButton_clicked()
{
    ui->PriorityLineEdit->setDisabled(true);
    ui->TQLineEdit->setDisabled(true);
}

void Widget::on_SJFPRadioButton_clicked()
{
    ui->PriorityLineEdit->setDisabled(true);
    ui->TQLineEdit->setDisabled(true);
}

void Widget::on_clearButton_clicked()
{
    ui->PNComboBox->clear();
    ui->PNComboBox->addItem(QString::number(0));
    processesInput.clear();
    totalCPUTime=0;
}

void Widget::on_exitButton_clicked()
{
    exit(0);
}

void Widget::on_DeleteButton_clicked()
{
    sort(processesInput.begin(),processesInput.end(),FCFS);
    int index = ui->PNComboBox->currentIndex();
    if(index < processesInput.size())
    {

        ui->PNComboBox->removeItem(index);
        deletedProcesses.push(processesInput[index]);
        totalCPUTime -=processesInput[index].getCPUBurst();
        processesInput.erase(processesInput.begin() + index);

    }
}

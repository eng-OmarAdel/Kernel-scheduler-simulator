#include "simulation.h"
#include "ui_simulation.h"
#include <stdlib.h>

simulation::simulation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::simulation)
{
    ui->setupUi(this);
}

simulation::~simulation()
{
    delete ui;
}

void simulation::paintEvent(QPaintEvent *event)
{
    if(1)
    {
        QPainter painter(this);
        QRect rect;
        QPoint vertex(30,30);
        rect.setTopLeft(vertex);
        if(noPremption)
        {
            double lastArrivalTime=0;
            for(int i=0;i<processesInput.size();i++)
            {
                int w = 500* processesInput[i].getCPUBurst()/ totalCPUTime;
                rect.setSize(QSize(w , 30));

                int r= rand()%200 + 20, g= rand()%200 +20, b= rand()%200 +20;
                QBrush brush(QColor(r,g,b));
                QString text(" P" + QString::number(processesInput[i].getProcessNumber()));

                painter.fillRect(rect,brush);
                painter.drawRect(rect);
                painter.drawText(rect,text);


                lastArrivalTime+=processesInput[i].getCPUBurst();

                AWT += lastArrivalTime - processesInput[i].getArrivalTime() - processesInput[i].getCPUBurst();

                QPoint arrivalPoint(rect.bottomRight());
                arrivalPoint.setY(arrivalPoint.y()+ 16);
                QString arrivalText(QString::number(lastArrivalTime));
                painter.drawText(arrivalPoint,arrivalText);


                vertex.setX(vertex.x() + w );
                rect.setTopLeft(vertex);
            }

            painter.drawText(QPoint(30,76),QString('0'));

            AWT/=processesInput.size();
            QString awtText = QString("Avg waiting time: " + QString::number(AWT) );
            painter.drawText(QPoint(30,126),awtText);
            AWT=0;
        }
        else if(RoundRobin)
        {
            double lastArrivalTime=0;
            for(int i=0;i<PProcessesInput.size();i++)
            {
                int w;
                if(TQ < PProcessesInput[i].getCPUBurstLeft())
                {
                    w = 500* TQ/ totalCPUTime;
                    lastArrivalTime+=TQ;
                }

                else
                {
                    w = 500* PProcessesInput[i].getCPUBurstLeft()/ totalCPUTime;
                    lastArrivalTime+=PProcessesInput[i].getCPUBurstLeft();
                    AWT += lastArrivalTime - PProcessesInput[i].getArrivalTime() - PProcessesInput[i].getCPUBurst();
                }

                painter.drawText(QPoint(30,76),QString('0'));

                rect.setSize(QSize(w , 30));

                int r= rand()%200 + 20, g= rand()%200 +20, b= rand()%200 +20;
                QBrush brush(QColor(r,g,b));
                QString text(" P" + QString::number(PProcessesInput[i].getProcessNumber()));

                painter.fillRect(rect,brush);
                painter.drawRect(rect);
                painter.drawText(rect,text);

                QPoint arrivalPoint(rect.bottomRight());
                arrivalPoint.setY(arrivalPoint.y()+ 16);
                QString arrivalText(QString::number(lastArrivalTime));
                painter.drawText(arrivalPoint,arrivalText);

                vertex.setX(vertex.x() + w );
                rect.setTopLeft(vertex);
            }
            AWT/=processesInput.size();
            QString awtText = QString("Avg waiting time: " + QString::number(AWT) );
            painter.drawText(QPoint(30,126),awtText);
            AWT=0;
        }
        else
        {
            int lastArrivalTime=0;
            vector<double> departure(processesInput.size());
            for(int i=0;i<PProcessesInput.size();i++)
            {
                int w;
                w = 500* PProcessesInput[i].getCPUBurstLeft()/ totalCPUTime;
                rect.setSize(QSize(w , 30));
                lastArrivalTime+=PProcessesInput[i].getCPUBurstLeft();

                for(int k=0;k<processesInput.size();k++)
                {
                    if(PProcessesInput[i].getProcessNumber()==processesInput[k].getProcessNumber())
                    {
                        departure[k]=lastArrivalTime;
                        break;
                    }
                }
                int r= rand()%200 + 20, g= rand()%200 +20, b= rand()%200 +20;
                QBrush brush(QColor(r,g,b));
                painter.fillRect(rect,brush);
                painter.drawRect(rect);

                QString processName(" P" + QString::number(PProcessesInput[i].getProcessNumber()));
                painter.drawText(rect,processName);

                QPoint arrivalPoint=rect.bottomLeft();
                arrivalPoint.setY(arrivalPoint.y()+15);
                QString arrivalTime(QString::number(PProcessesInput[i].getArrivalTime()));
                painter.drawText(arrivalPoint,arrivalTime);

                QPoint finishPoint=rect.topRight();
                finishPoint.setX(finishPoint.x()-6);
                finishPoint.setY(finishPoint.y()-3);
                QString finishTime(QString::number(PProcessesInput[i].getArrivalTime()+PProcessesInput[i].getCPUBurstLeft()));
                painter.drawText(finishPoint,finishTime);

                vertex.setX(vertex.x() + w );
                rect.setTopLeft(vertex);
            }
            double sumDep=accumulate(departure.begin(),departure.end(),0);
            AWT+=sumDep-AT-CPUB;
            AWT /=processesInput.size();
            QString awtText = QString("Avg waiting time: " + QString::number(AWT) );
            painter.drawText(QPoint(30,126),awtText);
            AWT=0;
        }
    }
}

void simulation::on_okButton_clicked()
{
    accept();
}

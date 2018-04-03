#ifndef SIMULATION_H
#define SIMULATION_H
#include "global.h"
#include <QDialog>
#include <QtGui>
#include <QtCore>

namespace Ui {
class simulation;
}

class simulation : public QDialog
{
    Q_OBJECT

public:
    explicit simulation(QWidget *parent = 0);
    ~simulation();

private:
    Ui::simulation *ui;
protected:
    void simulation::paintEvent(QPaintEvent *event);
private slots:
    void on_okButton_clicked();
};

#endif // SIMULATION_H

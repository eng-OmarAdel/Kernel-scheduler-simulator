#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "simulation.h"
#include "global.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_SIMButton_clicked();

    void on_AddButton_clicked();

    void on_PriorityNPRadioButton_clicked();

    void on_PriorityPRadioButton_clicked();

    void on_RRRadioButton_clicked();

    void on_FCFSRadioButton_clicked();

    void on_SJFNPRadioButton_clicked();

    void on_SJFPRadioButton_clicked();

    void on_clearButton_clicked();

    void on_exitButton_clicked();

    void on_DeleteButton_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

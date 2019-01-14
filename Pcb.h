#ifndef PCD_H
#define PCD_H

#include <QObject>
#include <QThread>
#include <QtGlobal>
#include <QTime>
#include <ui_mainwindow.h>
#include <QDebug>

class MainWindow;
class Dispatcher;

class Pcb: public QObject
{
    Q_OBJECT
public :

    void sendToConsole(QString sendText,QString colour);
    Pcb();
    Pcb(Dispatcher *dispatcher);
    int run(Ui::MainWindow *ui,int runningTime);
    int getName();
    int getPriority();
    int getTime();
    void decreasePriority();
public slots:

signals:
    void newText(QString &name,QString &colour);
    void runDown(Pcb* pcb,int runningTime);

private:
    int name;
    int status;//0:wait 1:executive
    int priority;
    int time;

};

#endif// PCD_H

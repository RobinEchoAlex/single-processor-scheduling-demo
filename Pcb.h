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

    void sendToConsole(QString sendText,QString colour,QString target);
    Pcb();
    Pcb(Dispatcher *dispatcher,MainWindow *mainWindow);
    int run(MainWindow *mainWindow,Dispatcher *dispatcher,int runningTime,int method);
    void runDown(MainWindow *mainWindow,Dispatcher *dispatcher,int runningTime,int method);
    int getName();
    int getPriority();
    int getTime();
    int getOriginTime();
    int getEnterTime();
    int calculateAverageNormalisedTurnaroundTime(Ui::MainWindow *ui,Dispatcher *dispatcher);//Refactor:should lies in dispatcher class
    void decreasePriority();
public slots:

signals:
    void newText(QString &name,QString &colour,QString &target);

private:
    int name;
    int status;//0:wait 1:executive
    int priority;
    int time;
    int originTime;
    int enterTime;

};

#endif// PCD_H

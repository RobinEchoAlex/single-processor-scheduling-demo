#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Pcb.h"
#include <QObject>
#include <QThread>
#include <QVector>
#include <QDebug>
#include <QTimer>

class MainWindow;

class Dispatcher: public QObject
{
    Q_OBJECT
    QThread workerThread;
public :
    void sendToConsole(QString sendText,QString colour,QString target);
    Dispatcher();
    Dispatcher(MainWindow *mainwindow);

    void setMaxPcb(int maxpcb);
    void setPause(bool pause);
    void setStop(bool stop);
    void setAverageNTT(double NTT);
    void qDebugOut();
    void clockTick(Ui::MainWindow *ui);
    int getMaxPcb();
    double getAverageNTT();
    int getPcbNumber();
    double getClock();
    int getPcbNo();
    bool inquirePause();
    bool inquireStop();
    bool inquireFree();
    QVector<Pcb*> pcbArray;

    void roundRobin(Ui::MainWindow *ui,Pcb *newPcb);
    void priorityDispatch(MainWindow *mainWindow,Pcb *newPcb);
    void ShortestProcessNext(Ui::MainWindow *ui,Pcb *newPcb);
    void ShortestRemainingTime(Ui::MainWindow *ui,Pcb *newPcb);
    void priorityInsert(MainWindow *mainWindow,Pcb *newPcb);
    void ShortestProcessNextInsert(Ui::MainWindow *ui,Pcb *newPcb);
    void ShortestRemainingTimeInsert(Ui::MainWindow *ui,Pcb *newPcb);
    void createNewPcb(MainWindow *mainWindow);
    void upDateLineup(MainWindow *mainWindow);

public slots:

signals:
    void newText(QString &name,QString &colour,QString &target);

private:
    int maxPcb;
    bool free;
    int pcbNumber;
    int pcbNo;
    bool pause;
    bool stop;
    double clock;
    double AverageNTT;
};

#endif // DISPATCHER_H

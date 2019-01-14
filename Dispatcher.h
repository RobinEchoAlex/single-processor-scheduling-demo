#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Pcb.h"
#include <QObject>
#include <QThread>
#include <QVector>
#include <QDebug>

class MainWindow;

class Dispatcher: public QObject
{
    Q_OBJECT
    QThread workerThread;
public :
    void sendToConsole(QString sendText,QString colour);
    Dispatcher();

    void setMaxPcb(int maxpcb);
    void setPause(bool pause);
    void setStop(bool stop);
    int getMaxPcb();
    int getPcbNumber();
    int getPcbNo();
    bool inquirePause();
    bool inquireStop();
    bool inquireFree();
    QVector<Pcb*> pcbArray;

    void roundRobin(Ui::MainWindow *ui,Pcb *newPcb);
    void priority(Pcb *newPcb);
    void ShortestProcessNext(Pcb *newPcb);
    void ShortestRemainingTime(Pcb *newPcb);
    void createNewPcb();

public slots:

signals:
    void newText(QString &name,QString &colour);

private:
    int maxPcb;
    bool free;
    int pcbNumber;
    int pcbNo;
    bool pause;
    bool stop;

};

#endif // DISPATCHER_H

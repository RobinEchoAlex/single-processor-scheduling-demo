#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Pcb.h"
#include <QObject>
#include <QThread>
#include <QVector>
#include <QDebug>


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
    int getpcbNo();
    bool inquirePause();
    bool inquireStop();
    bool inquireFree();
    QVector<Pcb*> pcbArray;

    void roundRobin(QVector<Pcb*> pcbArray,Pcb *newPcb);
    void priority(QVector<Pcb*> pcbArray,Pcb *newPcb);
    void ShortestProcessNext(QVector<Pcb*> pcbArray,Pcb *newPcb);
    void ShortestRemainingTime(QVector<Pcb*> pcbArray,Pcb *newPcb);
    void createNewPcb();

public slots:

signals:
    void newText(QString &name,QString &colour);

private:
    int maxPcb;
    bool free;
    int pcbNo;
    bool pause;
    bool stop;

};

#endif // DISPATCHER_H

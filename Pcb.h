#ifndef PCD_H
#define PCD_H

#include <QObject>
#include <QThread>
#include <QtGlobal>
#include <QTime>
#include <QDebug>


class Dispatcher;

class Pcb: public QObject
{
    Q_OBJECT
public :

    void sendToConsole(QString sendText,QString colour);
    Pcb();
    Pcb(Dispatcher *dispatcher);
    int run(int runningTime);

public slots:

signals:
    void newText(QString &name,QString &colour);
    void runDown(int runningTime);

private:
    int name;
    int status;//0:wait 1:executive
    int priority;
    int time;

};

#endif// PCD_H

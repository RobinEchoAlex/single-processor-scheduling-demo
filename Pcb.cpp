#include "Pcb.h"
#include "dispatcher.h"
#include "mainwindow.h"

Pcb::Pcb(){
    this->name=0;
    this->status=0;
    this->priority=0;
    this->time=0;
}

Pcb::Pcb(Dispatcher *dispatcher){
    if(dispatcher->inquireFree()==0){
        sendToConsole(tr("Fail to create a new process due to full PCB area"),"black");
        return;
    }
    this->name=dispatcher->getpcbNo();
    this->status=0;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    this->priority=qrand()%5;//priority: 0 - 5
    this->time=0;
}

void Pcb::sendToConsole(QString sendText,QString colour)//send the word to the text display window
{
    //qDebug()<<status<<endl;
    emit newText(sendText,colour);
}

int Pcb::run(int runningTime){
    this->status=1;
    if(this->time>runningTime){
        this->time=this->time-runningTime;
        emit runDown(runningTime);
        this->status=0;
        return 1;
    }
    else{
        this->time=0;
        emit runDown(time);
        this->status=0;
        return 0;
    }

}

#include "Dispatcher.h"

Dispatcher::Dispatcher(){
    QVector<Pcb> pcbArray(100);
}

void Dispatcher::setMaxPcb(int maxPcb){
    this->maxPcb=maxPcb;
}

int Dispatcher::getMaxPcb(){
    return this->maxPcb;
}

int Dispatcher::getpcbNo(){
    pcbNo++;
    return pcbNo-1;
}

bool Dispatcher::inquirePause(){
    return this->pause;
}

bool Dispatcher::inquireStop(){
    return this->stop;
}

bool Dispatcher::inquireFree(){
    return this->free;
}

void Dispatcher::sendToConsole(QString sendText,QString colour)//send the word to the text display window
{
    //qDebug()<<status<<endl;
    emit newText(sendText,colour);
}

void Dispatcher::roundRobin(QVector<Pcb*> pcbArray, Pcb *newPcb){
    bool finished;
    pcbArray.append(newPcb);
    if(pcbArray.isEmpty()){
        return;
    }
    finished=pcbArray.first()->run(1);
    if(finished==1) {
        pcbArray.append(pcbArray.takeFirst());
    }
    else{
        pcbArray.removeFirst();
    }
    for(int i=1;i<pcbArray.length();i++){
        pcbArray.move(i,i-1);
    }
}

void Dispatcher::createNewPcb(){
    if(this->pcbArray.length()>=this->getMaxPcb()){
        qDebug()<<"Pcb Array is full so suspending create new PCB"<<endl;
        return;
    }
    else{

    }
}

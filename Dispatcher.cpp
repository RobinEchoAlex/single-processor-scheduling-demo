#include "Dispatcher.h"

Dispatcher::Dispatcher(){
    QVector<Pcb> pcbArray;
    setMaxPcb(6);
    this->free=true;
    this->stop=false;
    this->pause=false;
    this->pcbNo=0;
    this->pcbNumber=0;
}

void Dispatcher::setMaxPcb(int maxPcb){
    this->maxPcb=maxPcb;
}

int Dispatcher::getMaxPcb(){
    return this->maxPcb;
}

int Dispatcher::getPcbNumber(){
    return pcbNumber;
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

int Dispatcher::getPcbNo(){
    return pcbNo;
}

void Dispatcher::sendToConsole(QString sendText,QString colour)//send the word to the text display window
{
    //qDebug()<<status<<endl;
    emit newText(sendText,colour);
}

void Dispatcher::roundRobin(Ui::MainWindow *ui, Pcb *newPcb){
    bool finished;
    if(newPcb!=nullptr) pcbArray.append(newPcb);
    qDebug()<<pcbArray.isEmpty()<<" "<<pcbArray.length()<<endl;
    if(pcbArray.isEmpty()){
        return;
    }
    finished=pcbArray.first()->run(ui,2);
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
        int possibility[]={100,60,30,10,8,5,0};
        QTime t;
        t= QTime::currentTime();
        qsrand(t.msec()+t.second()*1000);
        int probability=qrand()%100;
        qDebug()<<"Probability is "<<probability<<endl;
        qDebug()<<"PCB number is<<"<<this->getPcbNumber()<<endl;
        if(possibility[this->getPcbNumber()]<probability){
            return;
        }
    }
    Pcb *p= new Pcb(this);
    pcbArray.append(p);
    this->pcbNumber++;
    this->pcbNo++;

    return;
}

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
    this->name=dispatcher->getPcbNo();
    this->status=0;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    this->priority=qrand()%5;//priority: 0 - 5
    this->time=qrand()%9+1;//time 1-10
    qDebug()<<"a new pcb,#"<<this->name<<"has been created"<<endl;
}

int Pcb::getName(){
    return this->name;
}

int Pcb::getPriority(){
    return this->priority;
}

int Pcb::getTime(){
    return this->time;
}

void Pcb::sendToConsole(QString sendText,QString colour)//send the word to the text display window
{
    //qDebug()<<status<<endl;
    emit newText(sendText,colour);
}

int Pcb::run(Ui::MainWindow *ui,int runningTime){
    double percentage;
    this->status=1;
    ui->CurrentProcessNoValue->setText(QString::number(this->getName()));
    ui->CurrentProcessPriorityValue->setText(QString::number(this->getPriority()));
    if(this->time>runningTime){
        this->time=this->time-runningTime;
        for(int i=0;i<runningTime;i++){
            QEventLoop eventloop;
            QTimer::singleShot(1000, &eventloop, SLOT(quit()));
            eventloop.exec();
            qDebug()<<"PCB::RUN"<<runningTime<<" gettime "<<time;
            percentage = (runningTime-i)/ time *100;
            ui->progressBar->setValue(percentage);
        }
        this->status=0;
        return 1;
    }
    else{
        this->time=0;
        this->status=0;
        return 0;
    }

}

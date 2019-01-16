#include "Pcb.h"
#include "dispatcher.h"
#include "mainwindow.h"

Pcb::Pcb(){
    this->name=0;
    this->status=0;
    this->priority=0;
    this->time=0;
    this->originTime=0;
    this->enterTime=0;
}

Pcb::Pcb(Dispatcher *dispatcher,MainWindow *mainWindow){
    connect(this,&Pcb::newText,mainWindow,&MainWindow::print);
    if(dispatcher->inquireFree()==0){
        sendToConsole(tr("Fail to create a new process due to full PCB area"),"black","console");
        return;
    }
    name=dispatcher->getPcbNo();
    status=0;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    priority=qrand()%5;//priority: 0 - 5
    time=qrand()%9+1;//time 1-10
    originTime=time;
    enterTime=dispatcher->getClock();
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

int Pcb::getEnterTime(){
    return  this->enterTime;
}

int Pcb::getOriginTime(){
    return this->originTime;
}

void Pcb::decreasePriority(){
    if(priority>0) priority--;
}

void Pcb::sendToConsole(QString sendText,QString colour,QString target)//send the word to the text display window
{
    //qDebug()<<status<<endl;
    emit newText(sendText,colour,target);
}

int Pcb::run(Ui::MainWindow *ui,Dispatcher *dispatcher,int runningTime,int method){
    status=1;
    ui->CurrentProcessNoValue->setText(QString::number(getName()));
    ui->CurrentProcessPriorityValue->setText(QString::number(getPriority()));
    ui->CurrentProcessLeftTimeValue->setText(QString::number(getTime()));
    if(time>runningTime){
        time=time-runningTime;
        runDown(ui,dispatcher,runningTime);
        status=0;
        return 0;
    }
    else if (time<=runningTime){
        runDown(ui,dispatcher,time);
        time=0;
        status=0;
        calculateAverageNormalisedTurnaroundTime(ui,dispatcher);
        QString message="PCB#"+ QString::number(getName())+" quit";
        sendToConsole(message,"black","finished");
        return 1;//finished
    }
    return -1;
}

void Pcb::runDown(Ui::MainWindow *ui,Dispatcher *dispatcher,int runningTime){
    double percentage=0;
    for(int i=runningTime;i>=0;i--){
        dispatcher->clockTick(ui);
        percentage = (double)(time+i)/ (double)originTime *100;
        ui->progressBar->setValue(percentage);
        ui->CurrentProcessLeftTimeValue->setText(QString::number(time+i));
        QEventLoop eventloop;
        QTimer::singleShot(1000, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
}

int Pcb::calculateAverageNormalisedTurnaroundTime(Ui::MainWindow *ui,Dispatcher *dispatcher){
    double NTT;
    NTT =(dispatcher->getClock()-getEnterTime()) /(getOriginTime()-getTime());
    dispatcher->setAverageNTT(NTT);
    ui->NormalizeTurnaroundTime->display(dispatcher->getAverageNTT());
}

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

int Pcb::run(MainWindow *mainWindow,Dispatcher *dispatcher,int runningTime,int method){
    status=1;
    mainWindow->ui->CurrentProcessNoValue->setText(QString::number(getName()));
     mainWindow->ui->CurrentProcessPriorityValue->setText(QString::number(getPriority()));
     mainWindow->ui->CurrentProcessLeftTimeValue->setText(QString::number(getTime()));
    if(method!=2 && time>runningTime){
        time=time-runningTime;
        runDown( mainWindow,dispatcher,runningTime,method);
        status=0;
        return 0;
    }
    else if (method!=2 && time<=runningTime){
        runDown( mainWindow,dispatcher,time,method);
        time=0;
        status=0;
        calculateAverageNormalisedTurnaroundTime(mainWindow->ui,dispatcher);
        QString message="PCB#"+ QString::number(getName())+" quit";
        sendToConsole(message,"black","finished");
        return 1;//finished
    }
    else if(method==2)//SPN
    {
        runDown( mainWindow,dispatcher,time,method);
        time=0;
        return 1;
    }
    return -1;
}

void Pcb::runDown(MainWindow *mainwindow,Dispatcher *dispatcher,int runningTime,int method){
    double percentage=0;
    for(int i=runningTime;i>=0;i--){
        if(method== 2) {
            dispatcher->createNewPcb(mainwindow);
            time=0;
        }
        dispatcher->clockTick(mainwindow->ui);
        percentage = (double)(time+i)/ (double)originTime *100;
        mainwindow->ui->progressBar->setValue(percentage);
        mainwindow->ui->CurrentProcessLeftTimeValue->setText(QString::number(time+i));
        MainWindow::displayPause();
    }
    mainwindow->ui->CurrentProcessNoValue->setText("");
    mainwindow->ui->CurrentProcessLeftTimeValue->setText("");
    mainwindow->ui->CurrentProcessPriorityValue->setText("");
}

int Pcb::calculateAverageNormalisedTurnaroundTime(Ui::MainWindow *ui,Dispatcher *dispatcher){
    double NTT;
    NTT =(dispatcher->getClock()-getEnterTime()) /(getOriginTime()-getTime());
    dispatcher->setAverageNTT(NTT);
    ui->NormalizeTurnaroundTime->display(dispatcher->getAverageNTT());
}

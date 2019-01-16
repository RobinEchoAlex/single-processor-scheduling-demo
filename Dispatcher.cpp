#include "dispatcher.h"
#include "mainwindow.h"

void Dispatcher::qDebugOut(){
    int i=0;
    for(i=0;i!=pcbArray.length();i++){
        qDebug()<<"the "<<i<<" pcb's name=" << QString::number(pcbArray[i]->getName())<<" time="<<QString::number(pcbArray[i]->getTime())<<endl;
    }
}

Dispatcher::Dispatcher(){
    QVector<Pcb> pcbArray(6);
    setMaxPcb(6);
    this->free=true;
    this->stop=false;
    this->pause=false;
    this->pcbNo=0;
    this->pcbNumber=0;
    this->clock=0.001;
}

Dispatcher::Dispatcher(MainWindow *mainwindow){
    connect(this,&Dispatcher::newText,mainwindow,&MainWindow::print);
    QVector<Pcb> pcbArray(6);
    setMaxPcb(6);
    this->free=true;
    this->stop=false;
    this->pause=false;
    this->pcbNo=0;
    this->pcbNumber=0;
    this->clock=0.001;
}


void Dispatcher::setMaxPcb(int maxPcb){
    this->maxPcb=maxPcb;
}

void Dispatcher::setPause(bool pause){
    this->pause=pause;
}

void Dispatcher::setStop(bool stop){
    this->stop=stop;
}

void Dispatcher::setAverageNTT(double NTT){
    AverageNTT = (NTT+AverageNTT*pcbNo)/(AverageNTT*pcbNo+1);
}

int Dispatcher::getMaxPcb(){
    return this->maxPcb;
}

int Dispatcher::getPcbNumber(){
    return pcbNumber;
}

double Dispatcher::getClock(){
    return clock;
}

double Dispatcher::getAverageNTT(){
    return AverageNTT;
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

void Dispatcher::clockTick(Ui::MainWindow *ui){
    if(clock-0.001<1e-4) clock=1;
    else clock++;
    ui->RunningTime->display(clock);
}

int Dispatcher::getPcbNo(){
    return pcbNo;
}

void Dispatcher::sendToConsole(QString sendText,QString colour,QString target)//send the word to the text display window
{
    emit newText(sendText,colour,target);
}

void Dispatcher::roundRobin(MainWindow *mainWindow,Pcb *newPcb){
    bool finished;
    Pcb* pcb;
    if(newPcb!=nullptr) pcbArray.append(newPcb);
    qDebug()<<pcbArray.isEmpty()<<" "<<pcbArray.length()<<endl;
    if(pcbArray.isEmpty()){
        return;
    }
    pcb=pcbArray.takeFirst();
    upDateLineup(mainWindow);
    finished=pcb->run(mainWindow->ui,this,2,0);
    if(finished==0) {
        pcbArray.append(pcb);

    }
    else{
        pcbNumber--;
    }
    upDateLineup(mainWindow);
}

void Dispatcher::priorityDispatch(MainWindow *mainWindow,Pcb *newPcb){
    bool finished=0;
    if(pcbArray.isEmpty()){
        return;
    }
    pcbArray.first()->decreasePriority();
    finished=(pcbArray.first()->run(mainWindow->ui,this,2,1));
    if(finished==1) {
        qDebugOut();
        QString message = "PCB no"+QString::number(pcbArray.first()->getName())+"Finished";
        pcbArray.removeFirst();
        qDebugOut();
        sendToConsole(message,"black","console");
        pcbNumber--;
    }
    else {
        qDebugOut();
        Pcb *p= pcbArray.takeFirst();
        pcbArray.append(p);
    }
    upDateLineup(mainWindow);
}

void Dispatcher::ShortestProcessNext(MainWindow *mainWindow, Pcb *newPcb){
    bool finished;
    if(newPcb!=nullptr) pcbArray.append(newPcb);
    if(pcbArray.isEmpty()){
        return;
    }
    int i;
    int shortestPcb=0;
    for (i=0;i<pcbArray.length();i++)
    {
        if (pcbArray[i]->getTime() < pcbArray[shortestPcb]->getTime()){
            shortestPcb=i;
        }
    }
    finished=pcbArray[shortestPcb]->run(mainWindow->ui,this,0,2);
    pcbArray.remove(shortestPcb);
    i++;
    pcbNumber--;
    for (;i<pcbArray.length();i++)
    {
         pcbArray.move(i,i-1);
    }

}

void Dispatcher::ShortestRemainingTime(MainWindow *mainWindow,Pcb *newPcb){

}

void Dispatcher::createNewPcb(MainWindow *mainWindow){
    if(this->pcbNumber>=this->getMaxPcb()){
        qDebug()<<"Pcb Array is full so suspending create new PCB"<<endl;
        return;
    }
    else{
        int possibility[]={100,60,30,10,8,5,0};
        QTime t;
        t= QTime::currentTime();
        qsrand(t.msec()+t.second()*1000);
        int probability=qrand()%100;
        QEventLoop eventloop;
        QTimer::singleShot(1000, &eventloop, SLOT(quit()));
        eventloop.exec();
        if(possibility[this->getPcbNumber()]<probability){
            return;
        }
    }
    Pcb *p= new Pcb(this,mainWindow);
    if(mainWindow->policy.checkedId()==0) pcbArray.append(p);
    else if(mainWindow->policy.checkedId()==1) priorityInsert(mainWindow,p);
    pcbNumber++;
    pcbNo++;
    QString message = "A new PCB #"+QString::number(pcbNo-1)+" is created";
    sendToConsole(message,"black","console");
    message = "priority="+QString::number(p->getPriority())+" time request="+QString::number(p->getTime());
    sendToConsole(message,"black","console");
    return;
}

void Dispatcher::priorityInsert(MainWindow *mainWindow,Pcb *newPcb){
    if(pcbArray.isEmpty()==1) { pcbArray.append(newPcb); return;}
    else{
        int i;
        qDebugOut();

        for(i=0;i<pcbArray.length();i++)
        {
            if(newPcb->getPriority()>pcbArray[i]->getPriority()){
                pcbArray.insert(i,newPcb);
                qDebugOut();
                upDateLineup(mainWindow);
                return;
            }
        }
    }
    pcbArray.append(newPcb);
}

void Dispatcher::upDateLineup(MainWindow *mainWindow){
    for(int i=0;i<pcbArray.length();i++)
    {
        mainWindow->label[0][i]->setText(QString::number(pcbArray[i]->getName()));
     mainWindow->label[1][i]->setText(QString::number(pcbArray[i]->getPriority()));
        mainWindow->label[2][i]->setText(QString::number(pcbArray[i]->getTime()));
        mainWindow->label[3][i]->setText(QString::number(pcbArray[i]->getOriginTime()));
        mainWindow->label[4][i]->setText(QString::number((clock-pcbArray[i]->getEnterTime())/(pcbArray[i]->getOriginTime()-pcbArray[i]->getTime())));
    }
    for(int i=pcbArray.length();i<=5;i++){
        mainWindow->label[0][i]->setText("");
        mainWindow->label[1][i]->setText("");
        mainWindow->label[2][i]->setText("");
        mainWindow->label[3][i]->setText("");
        mainWindow->label[4][i]->setText("");
    }
    mainWindow->ui->NextPcbNameValue->setText(QString::number(pcbNo));
    mainWindow->ui->NumberOfPCBRunning->setText(QString::number(pcbNumber));
    return;
}


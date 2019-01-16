#include "dispatcher.h"
#include "mainwindow.h"

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

void Dispatcher::roundRobin(Ui::MainWindow *ui, Pcb *newPcb){
    bool finished;
    if(newPcb!=nullptr) pcbArray.append(newPcb);
    qDebug()<<pcbArray.isEmpty()<<" "<<pcbArray.length()<<endl;
    if(pcbArray.isEmpty()){
        return;
    }
    finished=pcbArray.first()->run(ui,this,2,0);
    if(finished==0) {
        pcbArray.append(pcbArray.takeFirst());
    }
    else{
        pcbArray.removeFirst();
        pcbNumber--;
    }
    for(int i=1;i<pcbArray.length();i++){
        pcbArray.move(i,i-1);
    }
}

void Dispatcher::priority(Ui::MainWindow *ui,Pcb *newPcb){
    bool finished;
    if(newPcb!=nullptr) pcbArray.append(newPcb);
    if(pcbArray.isEmpty()){
        return;
    }
    int i;
    int topPriorityPcb=0;
    for (i=0;i<pcbArray.length();i++)
    {
        if (pcbArray[i]->getPriority() > pcbArray[topPriorityPcb]->getPriority()){
            topPriorityPcb=i;
        }
    }
    finished=pcbArray[topPriorityPcb]->run(ui,this,1,1);
    pcbArray[topPriorityPcb]->decreasePriority();
    i=topPriorityPcb;
    i++;
    if(finished==1) {
         pcbArray.remove(topPriorityPcb);
         pcbNumber--;
         for (;i<pcbArray.length();i++)
         {
             pcbArray.move(i,i-1);
         }
    }
    else{
        pcbArray.append(pcbArray.takeAt(topPriorityPcb));
    }

}

void Dispatcher::ShortestProcessNext(Ui::MainWindow *ui, Pcb *newPcb){
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
    finished=pcbArray[shortestPcb]->run(ui,this,0,2);
    pcbArray.remove(shortestPcb);
    i++;
    pcbNumber--;
    for (;i<pcbArray.length();i++)
    {
         pcbArray.move(i,i-1);
    }

}

void Dispatcher::ShortestRemainingTime(Ui::MainWindow *ui,Pcb *newPcb){

}

void Dispatcher::createNewPcb(MainWindow *mainWindow){
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
        QEventLoop eventloop;
        QTimer::singleShot(1000, &eventloop, SLOT(quit()));
        eventloop.exec();
        //qDebug()<<"Probability is "<<probability<<endl;
        qDebug()<<"PCB number is<<"<<this->getPcbNumber()<<endl;
        if(possibility[this->getPcbNumber()]<probability){
            return;
        }
    }
    Pcb *p= new Pcb(this,mainWindow);
    pcbArray.append(p);
    this->pcbNumber++;
    this->pcbNo++;

    return;
}

void Dispatcher::upDateLineup(Ui::MainWindow *ui){
     if(pcbNumber>=1){
    ui->Name1->setText(QString::number(pcbArray[0]->getName()));
     ui->Priority1->setText(QString::number(pcbArray[0]->getPriority()));
     ui->TimeLeft1->setText(QString::number(pcbArray[0]->getTime()));
     ui->TimeRequired1->setText(QString::number(pcbArray[0]->getOriginTime()));
     qDebug()<<"UPDATELINEUP:"<<pcbArray[0]->getTime()<<" "<<pcbArray[0]->getOriginTime()<<" "<<clock<<" "<<pcbArray[0]->getEnterTime();
     ui->TurnAround1->setText(QString::number((clock-pcbArray[0]->getEnterTime())/(pcbArray[0]->getOriginTime()-pcbArray[0]->getTime())));
     }else return;
     if(pcbNumber>=2){
     ui->Name1_2->setText(QString::number(pcbArray[1]->getName()));
     ui->Priority1_2->setText(QString::number(pcbArray[1]->getPriority()));
     ui->TimeLeft1_2->setText(QString::number(pcbArray[1]->getTime()));
     ui->TimeRequired1_2->setText(QString::number(pcbArray[1]->getOriginTime()));
     ui->TurnAround1_2->setText(QString::number((clock-pcbArray[1]->getEnterTime())/(pcbArray[1]->getOriginTime()-pcbArray[1]->getTime())));
     }else return;
      if(pcbNumber>=3){
    ui->Name1_3->setText(QString::number(pcbArray[2]->getName()));
    ui->Priority1_3->setText(QString::number(pcbArray[2]->getPriority()));
    ui->TimeLeft1_3->setText(QString::number(pcbArray[2]->getTime()));
    ui->TimeRequired1_3->setText(QString::number(pcbArray[2]->getOriginTime()));
    ui->TurnAround1_3->setText(QString::number((clock-pcbArray[2]->getEnterTime())/(pcbArray[2]->getOriginTime()-pcbArray[2]->getTime())));
    }else return;
 if(pcbNumber>=4){
    ui->Name1_4->setText(QString::number(pcbArray[3]->getName()));
    ui->Priority1_4->setText(QString::number(pcbArray[3]->getPriority()));
    ui->TimeLeft1_4->setText(QString::number(pcbArray[3]->getTime()));
    ui->TimeRequired1_4->setText(QString::number(pcbArray[3]->getOriginTime()));
    ui->TurnAround1_4->setText(QString::number((clock-pcbArray[3]->getEnterTime())/(pcbArray[3]->getOriginTime()-pcbArray[3]->getTime())));
    }else return;
 if(pcbNumber>=5){
    ui->Name1_5->setText(QString::number(pcbArray[4]->getName()));
    ui->Priority1_5->setText(QString::number(pcbArray[4]->getPriority()));
    ui->TimeLeft1_5->setText(QString::number(pcbArray[4]->getTime()));
    ui->TimeRequired1_5->setText(QString::number(pcbArray[4]->getOriginTime()));
    ui->TurnAround1_5->setText(QString::number((clock-pcbArray[4]->getEnterTime())/(pcbArray[4]->getOriginTime()-pcbArray[4]->getTime())));
    }else return;
 if(pcbNumber>=6){
    ui->Name1_6->setText(QString::number(pcbArray[5]->getName()));
    ui->Priority1_6->setText(QString::number(pcbArray[5]->getPriority()));
    ui->TimeLeft1_6->setText(QString::number(pcbArray[5]->getTime()));
    ui->TimeRequired1_6->setText(QString::number(pcbArray[5]->getOriginTime()));
    ui->TurnAround1_6->setText(QString::number((clock-pcbArray[5]->getEnterTime())/(pcbArray[5]->getOriginTime()-pcbArray[5]->getTime())));
    }else return;
    return; 
}


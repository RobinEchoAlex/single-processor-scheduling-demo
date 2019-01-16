#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Pcb.h"
#include "Dispatcher.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Uniprocessor Dispatcher"));

    Dispatcher dispathcer;
    policy.addButton(ui->RRChosen,0);
    policy.addButton(ui->PRChosen,1);
    policy.addButton(ui->SPNChosen,2);
    policy.addButton(ui->SRTChosen,3);
    ui->RRChosen->setChecked(true);
    on_RRChosen_clicked();

}

MainWindow::~MainWindow()
{
    dispatcher.setStop(1);
    delete ui;
}

void MainWindow::on_RRChosen_clicked()
{
    ui->DispatchTacticsExplanation->setText(tr("  time slices are assigned to each process in equal portions and in circular order, handling all processes without priority"));
}

void MainWindow::on_PRChosen_clicked()
{
    ui->DispatchTacticsExplanation->setText(tr("  the top-priority process is chosen to execute."));
}

void MainWindow::on_SPNChosen_clicked()
{
    ui->DispatchTacticsExplanation->setText(tr("  a non-preemptive algorithm that selects for execution the waiting process with the smallest execution time."));
}

void MainWindow::on_SRTChosen_clicked()
{
    ui->DispatchTacticsExplanation->setText(tr("  a preemptive version of shortest job next scheduling, process with the smallest amount of time remaining until completion is selected to execute."));
}

void MainWindow::on_StartButton_clicked()
{
    dispatcher.setStop(false);
    dispatcher.setPause(false);
    QString message=tr("Simulation starts");
    QString colour="red";
    QString target="console";
    print(message,colour,target);
    while(dispatcher.inquireStop()==false && dispatcher.inquirePause()==false){
        if(policy.checkedId()==0){
            dispatcher.roundRobin(ui,nullptr);
            dispatcher.upDateLineup(ui);
            dispatcher.createNewPcb(this);
            dispatcher.upDateLineup(ui);
        }
        if(policy.checkedId()==1){
            dispatcher.priority(ui,nullptr);
            dispatcher.upDateLineup(ui);
            dispatcher.createNewPcb(this);
            dispatcher.upDateLineup(ui);
        }
        if(policy.checkedId()==2){
            dispatcher.ShortestProcessNext(ui,nullptr);
            dispatcher.upDateLineup(ui);
            dispatcher.createNewPcb(this);
            dispatcher.upDateLineup(ui);
        }
        if(policy.checkedId()==3){
            dispatcher.ShortestRemainingTime(ui,nullptr);
            dispatcher.upDateLineup(ui);
        }
    }

}

void MainWindow::print(QString &name,QString &colour,QString &target)
{
    QColor clrR(0,0,0);
    if(colour.compare("red",Qt::CaseInsensitive)==0)
    {
       clrR.setRgb(255,0,0);//Colour is set to red
    }
    else if(colour.compare("black",Qt::CaseInsensitive)==0)
    {
        clrR.setRgb(0,0,0);//Colour is set to black
    }
    stringToHtmlFilter(name);
    stringToHtml(name,clrR);
    if(target.compare("console",Qt::CaseInsensitive)==0)
    { ui->Console->insertHtml("<br />");
        ui->Console->insertHtml(name);
        ui->Console->moveCursor(QTextCursor::End);
    }
    else if(target.compare("finished",Qt::CaseInsensitive)==0)
    { ui->ProcessFinished->insertHtml("<br />");
        ui->ProcessFinished->insertHtml(name);
        ui->ProcessFinished->moveCursor(QTextCursor::End);
    }

}

void MainWindow::stringToHtmlFilter(QString &str)
{
    str.replace("&","&amp;");
    str.replace(">","&gt;");
    str.replace("<","&lt;");
    str.replace("\"","&quot;");
    str.replace("\'","&#39;");
    str.replace(" ","&nbsp;");
    str.replace("\n","<br>");
    str.replace("\r","<br>");
}

void MainWindow::stringToHtml(QString &str,QColor colour)
{
    QByteArray array;
    array.append(colour.red());
    array.append(colour.green());
    array.append(colour.blue());
    QString strC(array.toHex());
    str = QString("<spanT style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
}

/*
void MainWindow::runDown(Pcb* pcb,int runningTime){
    double percentage;
    ui->CurrentProcessNo->setText(QString::number(pcb->getName()));
    ui->CurrentProcessPriority->setText(QString::number(pcb->getPriority()));
    for(int i=0;i<runningTime;i++){
        QEventLoop eventloop;
        QTimer::singleShot(1000, &eventloop, SLOT(quit()));
        eventloop.exec();
        percentage = (runningTime-i)/ pcb->getTime();
    }
}
*/

void MainWindow::on_PauseButton_clicked()
{
    dispatcher.setPause(true);
    QString message="Simulation paused";
    QString colour="black";
    QString target="console";
    print(message,colour,target);
}

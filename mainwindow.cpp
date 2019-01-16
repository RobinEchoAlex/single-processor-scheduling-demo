#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Pcb.h"
#include "dispatcher.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Uniprocessor Dispatcher"));
    policy.addButton(ui->RRChosen,0);
    policy.addButton(ui->PRChosen,1);
    policy.addButton(ui->SPNChosen,2);
    policy.addButton(ui->SRTChosen,3);
    ui->RRChosen->setChecked(true);
    label[0][0]= ui->Name1;
    label[0][1]= ui->Name1_2;
    label[0][2]= ui->Name1_3;
    label[0][3]= ui->Name1_4;
    label[0][4]= ui->Name1_5;
    label[0][5]= ui->Name1_6;
    label[1][0]= ui->Priority1;
    label[1][1]= ui->Priority1_2;
    label[1][2]= ui->Priority1_3;
    label[1][3]= ui->Priority1_4;
    label[1][4]= ui->Priority1_5;
    label[1][5]= ui->Priority1_6;
    label[2][0]= ui->TimeLeft1;
    label[2][1]= ui->TimeLeft1_2;
    label[2][2]= ui->TimeLeft1_3;
    label[2][3]= ui->TimeLeft1_4;
    label[2][4]= ui->TimeLeft1_5;
    label[2][5]= ui->TimeLeft1_6;
    label[3][0]= ui->TimeRequired1;
    label[3][1]= ui->TimeRequired1_2;
    label[3][2]= ui->TimeRequired1_3;
    label[3][3]= ui->TimeRequired1_4;
    label[3][4]= ui->TimeRequired1_5;
    label[3][5]= ui->TimeRequired1_6;
    label[4][0]= ui->TurnAround1;
    label[4][1]= ui->TurnAround1_2;
    label[4][2]= ui->TurnAround1_3;
    label[4][3]= ui->TurnAround1_4;
    label[4][4]= ui->TurnAround1_5;
    label[4][5]= ui->TurnAround1_6;

    on_RRChosen_clicked();
}

MainWindow::~MainWindow()
{
    dispatcher->setStop(1);
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
    dispatcher->setStop(false);
    dispatcher->setPause(false);
    QString message=tr("Simulation starts");
    QString colour="red";
    QString target="console";
    print(message,colour,target);
    while(dispatcher->inquireStop()==false && dispatcher->inquirePause()==false){
        if(policy.checkedId()==0){
            dispatcher->roundRobin(this,nullptr);
            dispatcher->upDateLineup(this);
            dispatcher->createNewPcb(this);
            dispatcher->upDateLineup(this);
        }
        if(policy.checkedId()==1){
            dispatcher->priorityDispatch(this,nullptr);
            dispatcher->upDateLineup(this);
            dispatcher->createNewPcb(this);
            dispatcher->upDateLineup(this);
        }
        if(policy.checkedId()==2){
            dispatcher->ShortestProcessNext(this,nullptr);
            dispatcher->upDateLineup(this);
            dispatcher->createNewPcb(this);
            dispatcher->upDateLineup(this);
        }
        if(policy.checkedId()==3){
            dispatcher->ShortestRemainingTime(this,nullptr);
            dispatcher->upDateLineup(this);
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

void MainWindow::on_PauseButton_clicked()
{
    dispatcher->setPause(true);
    QString message="Simulation paused";
    QString colour="black";
    QString target="console";
    print(message,colour,target);
}

void MainWindow::on_ResetButton_clicked()
{
    dispatcher->setStop(1);
    dispatcher->pcbArray.clear();
    dispatcher->upDateLineup(this);
}

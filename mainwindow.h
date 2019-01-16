#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QCoreApplication>
#include <QTimer>
#include "Dispatcher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void print(QString & name,QString &colour,QString &target);
    void stringToHtmlFilter(QString &str);
    void stringToHtml(QString &str,QColor colour);
    //void runDown(Pcb* pcb,int runningTime);
    QButtonGroup policy;
    Dispatcher *dispatcher = new Dispatcher(this);
    Ui::MainWindow *ui;
    QLabel *label[5][6];
    static void displayPause();
    static void displayPause(int pauseSecond);

private slots:
    void on_RRChosen_clicked();

    void on_PRChosen_clicked();

    void on_SPNChosen_clicked();

    void on_SRTChosen_clicked();

    void on_StartButton_clicked();

    void on_PauseButton_clicked();

    void on_ResetButton_clicked();

private:

};

#endif // MAINWINDOW_H

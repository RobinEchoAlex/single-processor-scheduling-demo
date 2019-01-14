#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "dispatcher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void print(QString & name,QString &colour);
    void stringToHtmlFilter(QString &str);
    void stringToHtml(QString &str,QColor colour);
    QButtonGroup policy;
    Dispatcher dispatcher;

private slots:
    void on_RRChosen_clicked();

    void on_PRChosen_clicked();

    void on_SPNChosen_clicked();

    void on_SRTChosen_clicked();

    void on_StartButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

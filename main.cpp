#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    QTranslator translator;
    translator.load(":/language/tr_zh.qm");
    a.installTranslator(&translator);
    QLocale locale;
    if( locale.language() == QLocale::English )  //Automatically choose the language, in accordance with system enviornment
        {
            translator.load(QString("://language/tr_en.qm"));
            a.installTranslator(&translator);
        }
    else if( locale.language() == QLocale::Chinese )
        {
            qDebug() << "中文系统";
            translator.load(QString("://language/tr_zh.qm"));
            a.installTranslator(&translator);
        }

    MainWindow w;
    w.show();

    return a.exec();
}

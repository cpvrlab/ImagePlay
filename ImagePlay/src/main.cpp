#if _WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>
#include <QSplashScreen>
#include <QThread>

#ifdef QT_DEBUG
//#include <vld.h>
#include <QDebug>
#endif

#include "IPApplication.h"
#include "MainWindow.h"

#include "IPL_processes.h"

#include <math.h>

static MainWindow* mainWindow = 0;

void customMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (mainWindow)
        mainWindow->logMessage(type, context, msg);
}

int main(int argc, char *argv[])
{
    try
    {
        IPApplication a(argc, argv);

        // set language to english
        QLocale::setDefault(QLocale::English);
        a.setStyle(QStyleFactory::create("Fusion"));

        qInstallMessageHandler(customMessageOutput); // redirect qDebug to Log
        mainWindow = new MainWindow;
        mainWindow->show();

        // HSL test
        /*float hsl[3];
        for(float i=0.0; i<=1.0; i += FACTOR_TO_FLOAT)
        {
            qDebug() << "RGB: " << i << ", 1, 0";
            IPLColor::rgbToHsl(i, 1, 0, hsl);
            qDebug() << "HUE: " << hue;
            //qDebug() << "SAT: " << hsl[1];
            //qDebug() << "LIG: " << hsl[2];
        }*/

        //FLT_EPSILON
/*
        qDebug() << "E: " << 1.0f / 1.192092896e-07F;

        // accuracy check
        for(int e = 1; e <= 32; e++)
        {
            float x = 0.0;
            long long base = std::pow(2.0,e);

            float inc = 1.0 / base;

            qDebug() << "Testing\tBase\t" << base << "\t" << e << "bit\tincrement\t" << inc;

            if(inc < 0)
            {
                qDebug() << "Increment overflow!";
                break;
            }

            for(double i=0; i<=base; i++)
            {
                double y = (i/base);
                if(x-y != 0)
                {
                    qDebug() << "INEXACT: " << x << " != " << y;
                    break;
                }
                else if(i==base)
                {
                    qDebug() << "EXACT";
                    break;
                }
                x += inc;
            }
        }*/
/*
        long long times = 100000000;
        {
            srand(0);
            unsigned int x = 1;
            QElapsedTimer timer;
            timer.start();
            for(long long i=0; i<times; i++)
            {
                int r = rand() % UINT_MAX + 1;
                x += r;
                r = rand() % UINT_MAX + 1;
                x *= r;
                r = rand() % UINT_MAX + 1;
                x -= r;
                r = rand() % UINT_MAX + 1;
                x /= r;
            }
            qDebug() << "X: " << x;
            qDebug() << "BENCHMARK: " << timer.elapsed();
        }
        {
            srand(0);
            uchar x = 1;
            QElapsedTimer timer;
            timer.start();
            for(long long i=0; i<times; i++)
            {
                uchar r = rand() % UCHAR_MAX + 1;
                x += r;
                r = rand() % UCHAR_MAX + 1;
                x *= r;
                r = rand() % UCHAR_MAX + 1;
                x -= r;
                r = rand() % UCHAR_MAX + 1;
                x /= r;
            }
            qDebug() << "X: " << x;
            qDebug() << "BENCHMARK: " << timer.elapsed();
        }
        {
            srand(0);
            float x = 1.0f;
            QElapsedTimer timer;
            timer.start();
            for(long long i=0; i<times; i++)
            {
                float r = (float) rand() / INT_MAX + 1;
                x += r;
                r = (float) rand() / INT_MAX + 1;
                x *= r;
                r = (float) rand() / INT_MAX + 1;
                x -= r;
                r = (float) rand() / INT_MAX + 1;
                x /= r;
            }
            qDebug() << "X: " << x;
            qDebug() << "BENCHMARK: " << timer.elapsed();
        }
        {
            srand(0);
            double x = 1.0;
            QElapsedTimer timer;
            timer.start();
            for(long long i=0; i<times; i++)
            {
                double r = (double) rand() / INT_MAX + 1;
                x += r;
                r = (double) rand() / INT_MAX + 1;
                x *= r;
                r = (double) rand() / INT_MAX + 1;
                x -= r;
                r = (double) rand() / INT_MAX + 1;
                x /= r;
            }
            qDebug() << "X: " << x;
            qDebug() << "BENCHMARK: " << timer.elapsed();
        }*/

        //        QPalette darkPalette;
        //        darkPalette.setColor(QPalette::Window, QColor(53,53,53));
        //        darkPalette.setColor(QPalette::WindowText, Qt::white);
        //        darkPalette.setColor(QPalette::Base, QColor(25,25,25));
        //        darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        //        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        //        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        //        darkPalette.setColor(QPalette::Text, Qt::white);
        //        darkPalette.setColor(QPalette::Button, QColor(53,53,53));
        //        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        //        darkPalette.setColor(QPalette::BrightText, Qt::red);
        //        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        //        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        //        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        //        a.setPalette(darkPalette);

        //    a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");


        //    QMessageBox::information((QMainWindow*) &w, QString("Styles"), s);

        return a.exec();
    }
    catch(...)
    {
        QMessageBox::critical(NULL, "Error", "Main error");
        return -1;
    }
}

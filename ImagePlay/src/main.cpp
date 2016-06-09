//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

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

#ifdef USE_FERVOR_UPDATER
    #include "fvupdater.h"
#endif

#include <math.h>

static MainWindow* mainWindow = 0;

void customMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (mainWindow)
        mainWindow->logMessage(type, context, msg);
}

int main(int argc, char *argv[])
{
    IPApplication a(argc, argv);

    QApplication::setApplicationName(APP_PRODUCT);
    QApplication::setApplicationVersion(APP_VERSION);
    QApplication::setOrganizationName(APP_COMPANY);
    QApplication::setOrganizationDomain(IMAGEPLAY_URL);

#ifdef USE_FERVOR_UPDATER
    // Fervor update checker
    FvUpdater::sharedUpdater()->SetFeedURL(IMAGEPLAY_APPCAST_URL);
    FvUpdater::sharedUpdater()->CheckForUpdatesSilent();
#endif

    // set language to english
    QLocale::setDefault(QLocale::English);
    a.setStyle(QStyleFactory::create("Fusion"));

    qInstallMessageHandler(customMessageOutput); // redirect qDebug to Log
    mainWindow = new MainWindow;
    mainWindow->show();

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

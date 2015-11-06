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

#include "IPApplication.h"

IPApplication::IPApplication(int &argc, char **argv) : QApplication(argc, argv)
{
#ifdef Q_OS_MACX
    setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
}
/*
bool IPApplication::notify(QObject* rec, QEvent* ev)
{
    try
    {
        return QApplication::notify(rec, ev);
    }
    catch(std::exception &e)
    {
        QString msg("Exception: ");
        msg.append(e.what());
        QMessageBox::critical(0, "Error", msg);
    }
    catch(...)
    {
        QMessageBox::critical(0, "Error", "Unknown error");
    }
    return false;
}*/

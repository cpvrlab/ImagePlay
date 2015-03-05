#include "IPApplication.h"

IPApplication::IPApplication(int &argc, char **argv) : QApplication(argc, argv)
{
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

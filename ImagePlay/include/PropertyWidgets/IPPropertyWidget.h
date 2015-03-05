#ifndef IPPROPERTYWIDGET_H
#define IPPROPERTYWIDGET_H

#include <QWidget>

#include "IPL_processes.h"




class IPLColorPickHandler
{
public:
    IPLColorPickHandler()
    {

    }
    virtual void pickColor(IPLColor color) = 0;
};

class IPLCoordinatePickHandler
{
public:
    IPLCoordinatePickHandler()
    {

    }
    virtual void pickCoordinates(int x, int y) = 0;
};

class IPPropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IPPropertyWidget(QWidget* = 0) {}
    virtual void saveValue() = 0;

signals:
    void changed();
public slots:

};

#endif // IPPROPERTYWIDGET_H

#ifndef IPPROPERTYRADIOINT_H
#define IPPROPERTYRADIOINT_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyRadioInt : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyRadioInt(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        _property = property;

        setLayout(new QVBoxLayout);
        layout()->setMargin(0);

        int value = property->value();

        _buttonGroup = new QButtonGroup(this);

        // split the properties
        QString rawName(property->name());
        QString name = rawName.split(":").at(0);
        QString rawOptions = rawName.split(":").at(1);
        QStringList options = rawOptions.split("|");

        for(int i=0; i<options.size(); i++)
        {
            QRadioButton* btn = new QRadioButton(options[i], this);
            btn->setChecked(i==value);
            _buttonGroup->addButton(btn, i);
            layout()->addWidget(btn);


            connect(btn, &QRadioButton::clicked, this, &IPPropertyRadioInt::valueChanged);
        }

    }
    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    int value()             { return _buttonGroup->checkedId(); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void valueChanged()
    {
        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyInt*    _property;
    QButtonGroup*               _buttonGroup;
};

#endif // IPPROPERTYRADIOINT_H

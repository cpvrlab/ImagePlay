#ifndef IPPROPERTYCOMBOBOX_H
#define IPPROPERTYCOMBOBOX_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QComboBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyCombobox : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyCombobox(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        _property = property;

        int value = property->value();

        setLayout(new QVBoxLayout(this));

        _combobox = new QComboBox(this);
        layout()->addWidget(_combobox);
        layout()->setMargin(0);

        // split the properties
        QString rawName(property->name());
        QString name = rawName.split(":").at(0);
        QString rawOptions = rawName.split(":").at(1);
        QStringList options = rawOptions.split("|");

        _combobox->addItems(options);

        _combobox->setCurrentIndex(value);

        connect(_combobox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &IPPropertyCombobox::valueChanged);
    }
    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    int value()             { return _combobox->currentIndex(); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void valueChanged()
    {
        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyInt*  _property;
    QComboBox*              _combobox;
};

#endif // IPPROPERTYCOMBOBOX_H

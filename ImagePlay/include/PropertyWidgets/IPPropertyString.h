#ifndef IPPROPERTYSTRING_H
#define IPPROPERTYSTRING_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include "IPPropertyWidget.h"
#include "IPLProcessProperty.h"

class IPPropertyString : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyString(IPLProcessPropertyString* property, QWidget* parent) : IPPropertyWidget(parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        std::string value = ((IPLProcessPropertyString*) property)->value();

        _lineEdit = new QLineEdit(QString::fromStdString(value));

        layout()->addWidget(_lineEdit);

        connect(_lineEdit, &QLineEdit::editingFinished, this, &IPPropertyString::onEditingFinished);

        _property = property;
    }
    QString value()         { return _lineEdit->text(); }
    void saveValue()        { _property->setValue(value().toStdString()); }

signals:

public slots:
    void onEditingFinished()
    {
        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyString* _property;
    QLineEdit* _lineEdit;

};

#endif // IPPROPERTYSTRING_H

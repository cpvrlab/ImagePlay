#ifndef IPPROPERTYFILECREATE_H
#define IPPROPERTYFILECREATE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include "IPPropertyWidget.h"
#include "IPLProcessProperty.h"

class IPPropertyFileSave : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyFileSave(IPLProcessPropertyString* property, QWidget* parent) : IPPropertyWidget(parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        std::string value = ((IPLProcessPropertyString*) property)->value();


        // split the properties
        QString rawName(property->name());
        _filetypeString = rawName.split(":").at(1);

        _lineEdit = new QLineEdit(QString::fromStdString(value));
        _lineEdit->setReadOnly(true);

        _button = new QPushButton("...");
        _button->setMaximumWidth(30);

        layout()->addWidget(_lineEdit);
        layout()->addWidget(_button);

        connect(_button, &QPushButton::pressed,
                this, &IPPropertyFileSave::onPressButton );

        _property = property;
    }
    QString value()         { return _lineEdit->text(); }
    void saveValue()        { _property->setValue(value().toStdString()); }

signals:

public slots:
    void onPressButton()
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", _filetypeString);

        if(fileName.length() > 0)
        {
            _lineEdit->setText(fileName);
            saveValue();

            emit changed();
        }
    }

private:
    IPLProcessPropertyString*   _property;
    QLineEdit*                  _lineEdit;
    QPushButton*                _button;
    QString                     _filetypeString;
};

#endif // IPPROPERTYFILECREATE_H

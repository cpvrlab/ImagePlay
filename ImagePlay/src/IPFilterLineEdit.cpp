#include "IPFilterLineEdit.h"

IPFilterLineEdit::IPFilterLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    _resetButton = new QPushButton("", this);
    _resetButton->setMinimumSize(QSize(16,16));
    _resetButton->setMaximumSize(QSize(16,16));
    _resetButton->setIcon(QIcon(":/Delete-32.png"));
    _resetButton->setStyleSheet("border:none;");
    setCursor(Qt::ArrowCursor);

    setLayout(new QHBoxLayout);

    this->layout()->addWidget(_resetButton);
    this->layout()->setContentsMargins(0,0,3,0);
    this->layout()->setAlignment(_resetButton, Qt::AlignRight);

    connect(_resetButton, &QPushButton::pressed, this, &IPFilterLineEdit::resetPressed);
    connect(_resetButton, &QPushButton::released, this, &IPFilterLineEdit::resetReleased);
    connect(_resetButton, &QPushButton::clicked, this, &IPFilterLineEdit::resetClicked);
}

void IPFilterLineEdit::resetPressed()
{
    _resetButton->setIcon(QIcon(":/Delete-32-over.png"));
}

void IPFilterLineEdit::resetReleased()
{
    _resetButton->setIcon(QIcon(":/Delete-32.png"));
}

void IPFilterLineEdit::resetClicked()
{
    setText("");
}

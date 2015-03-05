#include "IPMorphologyCheckbox.h"

IPMorphologyCheckbox::IPMorphologyCheckbox(QWidget *parent) :
    QCheckBox(parent)
{
//    setMouseTracking(true);
}


void IPMorphologyCheckbox::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor fillColor = Qt::black;    // 0
    if(checkState() == Qt::PartiallyChecked)
    {
        fillColor = Qt::lightGray;      // 1
    }
    else if(checkState() == Qt::Checked)
    {
        fillColor = Qt::white;          // 2
    }

    // invisible if disabled
    if(isEnabled())
    {
        QBrush brush(fillColor);

        painter.fillRect(0, 0, width(), height(), brush);
        painter.drawRect(0, 0, width(), height());
    }
    else
    {
        QPen pen(Qt::lightGray);
        painter.setPen(pen);
        painter.drawRect(0, 0, width(), height());
    }
}


// enhance weird click behaviour
void IPMorphologyCheckbox::mousePressEvent(QMouseEvent *)
{
    qDebug() << "mousePressEvent";
    click();
}

void IPMorphologyCheckbox::enterEvent(QEvent*)
{
//    qDebug() << "enterEvent";
//    click();
}


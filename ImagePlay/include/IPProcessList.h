//#############################################################################
//  File:      IPProcessList.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPPROCESSLIST_H
#define IPPROCESSLIST_H

#include <QListWidget>
#include <QDrag>
#include <QMimeData>
#include <QtGui>
#include <QPixmap>
#include <QPainter>

#include "IPLProcess.h"

//-----------------------------------------------------------------------------
//!Custom process list
/*!
 * Renders a process list with icon and process name.
 * Provides filtering by process name.
 */
class IPProcessList : public QListWidget
{
    Q_OBJECT
public:
    explicit            IPProcessList   (QWidget *parent = 0);
    void                filter          (QString text);
    void                addProcessItem  (QString processID, QString text, QString keywords, IPLProcess::IPLProcessCategory category);

private:
    std::vector<QColor> _categoryColors;

    // QAbstractItemView interface
protected:
    void                startDrag       (Qt::DropActions supportedActions);
};

#endif // IPPROCESSLIST_H

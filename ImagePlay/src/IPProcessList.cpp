#include "IPProcessList.h"

IPProcessList::IPProcessList(QWidget *parent) :
    QListWidget(parent)
{
    setDragEnabled(true);
    setIconSize(QSize(32, 32));

    // define category colors
    /*_categoryColors.push_back(QColor(255, 255, 255));       // CATEGORY_UNDEFINED
    _categoryColors.push_back(QColor(0, 182, 189));         // CATEGORY_IO
    _categoryColors.push_back(QColor(27, 117, 188));       // CATEGORY_CONVERSIONS
    _categoryColors.push_back(QColor(69, 79, 161));        // CATEGORY_ARITHMETIC
    _categoryColors.push_back(QColor(116, 72, 157));         // CATEGORY_GEOMETRY
    _categoryColors.push_back(QColor(170, 85, 161));        // CATEGORY_POINTOPERATIONS
    _categoryColors.push_back(QColor(240, 78, 77));        // CATEGORY_LOCALOPERATIONS
    _categoryColors.push_back(QColor(243, 113, 90));         // CATEGORY_MORPHOLOGY
    _categoryColors.push_back(QColor(247, 148, 72));        // CATEGORY_HISTOGRAM
    _categoryColors.push_back(QColor(253, 185, 77));        // CATEGORY_EDGES
    _categoryColors.push_back(QColor(255, 244, 80));         // CATEGORY_GRADIENTS
    _categoryColors.push_back(QColor(137, 199, 101));        // CATEGORY_FOURIER
    _categoryColors.push_back(QColor(0, 178, 116));         // CATEGORY_OBJECTS*/
    _categoryColors.push_back(QColor(255, 255, 255));       // CATEGORY_UNDEFINED
    _categoryColors.push_back(QColor(173,213,138));         // CATEGORY_IO
    _categoryColors.push_back(QColor(101,194,149));         // CATEGORY_CONVERSIONS
    _categoryColors.push_back(QColor(89,197,199));          // CATEGORY_ARITHMETIC
    _categoryColors.push_back(QColor(94,138,199));          // CATEGORY_GEOMETRY
    _categoryColors.push_back(QColor(85,101,175));          // CATEGORY_POINTOPERATIONS
    _categoryColors.push_back(QColor(130,106,175));         // CATEGORY_LOCALOPERATIONS
    _categoryColors.push_back(QColor(189,124,181));         // CATEGORY_MORPHOLOGY
    _categoryColors.push_back(QColor(243,123,112));         // CATEGORY_HISTOGRAM
    _categoryColors.push_back(QColor(246,142,118));         // CATEGORY_EDGES
    _categoryColors.push_back(QColor(249,168,112));         // CATEGORY_GRADIENTS
    _categoryColors.push_back(QColor(253,197,120));         // CATEGORY_FOURIER
    _categoryColors.push_back(QColor(225,246,113));         // CATEGORY_OBJECTS
}

void IPProcessList::filter(QString text)
{
    for(int i=0; i < count(); i++)
    {
        QListWidgetItem *item = this->item(i);

        // filter by name OR keywords
        if(item->text().contains(text, Qt::CaseInsensitive) || item->statusTip().contains(text, Qt::CaseInsensitive))
        {
            item->setHidden(false);
        }
        else
        {
            item->setHidden(true);
        }
    }
}

void IPProcessList::addProcessItem(QString processID, QString text, QString keywords, IPLProcess::IPLProcessCategory category)
{
    // load icon from png file and add background color based on the process category

    QFileInfo iconFile(QString("process_icons/") + processID + QString(".png"));

    if(!iconFile.exists())
    {
        iconFile = QFileInfo(QString("process_icons/Plugin.png"));
    }

    QPixmap transparentIcon(iconFile.absoluteFilePath());
    QPixmap finalIcon(25,25);

    QPainter painter(&finalIcon);
    painter.fillRect(0,0,25,25,_categoryColors.at(category));
    painter.drawPixmap(0,0,25,25,transparentIcon);

    QListWidgetItem* newItem = new QListWidgetItem(finalIcon, text);
    newItem->setToolTip(processID);
    newItem->setStatusTip(keywords);
    newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

    addItem(newItem);
}

void IPProcessList::startDrag(Qt::DropActions)
{
    QListWidgetItem* item = currentItem();
    QMimeData* mimeData = new QMimeData;
    QByteArray processID;
    processID.append(item->toolTip());
    mimeData->setData("application/x-imageplay", processID);

    QPixmap dragPixmap = item->icon().pixmap(32,32);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(dragPixmap);
    drag->setHotSpot(QPoint(16,16));

    drag->exec(Qt::MoveAction);

    //QListWidget::startDrag(supportedActions);
}

//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

#include "IPProcessListTabWidget.h"

IPProcessListTabWidget::IPProcessListTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    // set up styling
    setTabPosition(West);
}

void IPProcessListTabWidget::init(MainWindow *mainWindow)
{
    _mainWindow = mainWindow;

    clear();

    // items
    QStringList categoryIcons;
    categoryIcons.append(":/icons/16.png");
    categoryIcons.append(":/icons/12.png");
    categoryIcons.append(":/icons/27.png");
    categoryIcons.append(":/icons/26.png");
    categoryIcons.append(":/icons/26.png");
    categoryIcons.append(":/icons/8.png");
    categoryIcons.append(":/icons/8.png");
    categoryIcons.append(":/icons/8.png");
    categoryIcons.append(":/icons/8.png");
    categoryIcons.append(":/icons/8.png");
    categoryIcons.append(":/icons/8.png");
    categoryIcons.append(":/icons/8.png");

    QStringList processCategories;                      // CATEGORY_UNDEFINED
    processCategories.append("Input/Output");           // CATEGORY_IO
    processCategories.append("Conversions");            // CATEGORY_CONVERSIONS
    processCategories.append("Geometry");               // CATEGORY_GEOMETRY
    processCategories.append("Arithmetic Operations");  // CATEGORY_ARITHMETIC
    processCategories.append("Point Operations");       // CATEGORY_POINTOPERATIONS
    processCategories.append("Local Operations");       // CATEGORY_LOCALOPERATIONS
    processCategories.append("Morphology & Rank Order");// CATEGORY_MORPHOLOGY
    processCategories.append("Histogram");              // CATEGORY_HISTOGRAM
    processCategories.append("Edges");                  // CATEGORY_GRADIENTS
    processCategories.append("Gradients");              // CATEGORY_GRADIENTS
    processCategories.append("Fourier");                // CATEGORY_FOURIER
    processCategories.append("Objects");                // CATEGORY_OBJECTS
    processCategories.append("Plugins");                //

    //list process names by process category
    QList<QStringList> processLibrary;
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_IO));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_CONVERSIONS));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_GEOMETRY));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_ARITHMETIC));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_POINTOPERATIONS));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_LOCALOPERATIONS));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_MORPHOLOGY));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_HISTOGRAM));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_EDGES));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_GRADIENTS));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_FOURIER));
    processLibrary.append(mainWindow->factory()->getProcessNamesByCategory(IPLProcess::CATEGORY_OBJECTS));

    QStringList plugins;
    for(int i=0; i<mainWindow->pluginManager()->loadedPlugins()->size(); i++)
    {
        plugins.append(mainWindow->pluginManager()->loadedPlugins()->at(i));
    }
    processLibrary.append(plugins);

    // all processes tab
    _allProcessesList = new IPProcessList(mainWindow);

    QWidget* tabAll = new QWidget(this);
    tabAll->setLayout(new QVBoxLayout);
    QLabel* labelAll = new QLabel("All Processes", tabAll);
    labelAll->setStyleSheet("font-size: 12px; font-weight: bold;");
    tabAll->layout()->addWidget(labelAll);
    tabAll->layout()->addWidget(_allProcessesList);

    // rotate icon
    QPixmap pixmap(":/home.png");
    QMatrix transform;
    transform.rotate(90);
    addTab(tabAll, QIcon(pixmap.transformed(transform)), "");

    setIconSize(QSize(20, 20));

    // add all other tabs
    for(int i=0; i<processCategories.length(); i++)
    {
        QWidget* tab = new QWidget(this);
        tab->setLayout(new QVBoxLayout);
        QLabel* label = new QLabel(processCategories[i], tab);
        label->setStyleSheet("font-size: 12px; font-weight: bold;");
        IPProcessList* list = new IPProcessList(mainWindow, this);

        QPixmap categoryIcon;

        for(int j=0; j < processLibrary[i].length(); j++)
        {
            QString processID = processLibrary[i].at(j);

            IPLProcess* tmpProcess = mainWindow->factory()->getInstance(processID);

            // only add if instance exists
            if(tmpProcess)
            {
                QString title = QString::fromStdString(tmpProcess->title());
                QString keywords = QString::fromStdString(tmpProcess->keywords());

                // add to list
                list->addProcessItem(processID, title, keywords, tmpProcess->category());

                // and add to all processes list
                _allProcessesList->addProcessItem(processID, title, keywords, tmpProcess->category());

                // use icon of first process for the category tab
                if(categoryIcon.width() == 0)
                {
                    QFileInfo iconFile(_mainWindow->processIconPath(processID));

                    if(iconFile.exists())
                    {
                        categoryIcon = QPixmap(iconFile.absoluteFilePath());
                    }
                }
            }

            delete tmpProcess;
        }

        // sort items
        list->sortItems();

        // add GUI elements
        tab->layout()->addWidget(label);
        tab->layout()->addWidget(list);

        // for the last tab (plugins) add buttons
        if(i == processCategories.length()-1)
        {
            QPushButton* btnReloadPlugins = new QPushButton("Reload Plugins");
            connect(btnReloadPlugins, &QPushButton::clicked, mainWindow, &MainWindow::reloadPlugins);
            tab->layout()->addWidget(btnReloadPlugins);
        }

        // rotate icon
        //QPixmap pixmap(categoryIcons[i]);
        QMatrix transform;
        transform.rotate(90);

        // only add categories with entries
        addTab(tab, QIcon(categoryIcon.transformed(transform)), "");
        setTabToolTip(i+1, processCategories[i]);
    }

    // set plugins tab icon
    QPixmap pluginIcon(":/plugins.png");
    setTabIcon(count()-1, QIcon(pluginIcon.transformed(transform)));

    // sort all process list
    _allProcessesList->sortItems();
}

void IPProcessListTabWidget::filter(QString text)
{
    // display first tab and filter list
    setCurrentIndex(0);
    _allProcessesList->filter(text);
}

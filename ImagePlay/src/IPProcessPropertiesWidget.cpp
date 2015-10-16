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

#include "IPProcessPropertiesWidget.h"

IPProcessPropertiesWidget::IPProcessPropertiesWidget(QWidget *parent) :
    QWidget(parent)
{
    _mainWindow = NULL;
    _processStep = NULL;

    // add settings to panel
    QFormLayout* layout = new QFormLayout;
    layout->setVerticalSpacing(0);

    setLayout(layout);
    setBackgroundRole(QPalette::Midlight);
}

IPProcessPropertiesWidget::~IPProcessPropertiesWidget()
{

}

bool IPProcessPropertiesWidget::sortByPosition(IPLProcessProperty* x, IPLProcessProperty* y)
{
    return x->position() < y->position();
}

void IPProcessPropertiesWidget::init(IPProcessStep* processStep)
{
    _processStep = processStep;

    // remove all children
    while (layout()->count() > 0)
    {
        QLayoutItem* item = layout()->takeAt(0);
        if(item  != NULL )
        {
            delete item->widget();
            delete item;
        }
    }
//  delete layout();

    auto* processSettings = _processStep->process()->properties();

    if(processSettings->size() == 0)
    {
        addPropertyWidget("This step has no properties.", "", NULL);
    }

    // sort the properties by user set position
    std::vector<IPLProcessProperty*> orderedProperties;
    orderedProperties.reserve(processSettings->size());

    for (auto &entry: *processSettings)
        orderedProperties.push_back(entry.second.get());

    std::sort(orderedProperties.begin(), orderedProperties.end(), IPProcessPropertiesWidget::sortByPosition);

    // create all property widgets
    for (auto &property: orderedProperties)
    {
        // generate GUI based on the property type

        if (property->widget() == IPL_WIDGET_HIDDEN) {} //Don't process hidden widgets
        else if (property->widget() == IPL_WIDGET_LABEL) //Labels are type independent
        {
            QFormLayout* layout = (QFormLayout*) this->layout();

            QLabel* lblDescription = new QLabel(property->description());
            lblDescription->setWordWrap(true);
            lblDescription->setStyleSheet("color: #666; font-size: 10px");
            layout->addRow("", lblDescription);

            // add widget to list
            //_propertyWidgets.append(lblDescription);
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyInt*>(property))
        {
            IPPropertyWidget *widget = NULL;
            QString rawName, name;
            switch(p->widget())
            {
            case IPL_WIDGET_SLIDER:
                widget = new IPPropertySliderInt(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            case IPL_WIDGET_SLIDER_ODD:
                widget = new IPPropertySliderIntOdd(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            case IPL_WIDGET_SLIDER_EVEN:
                widget = new IPPropertySliderIntEven(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            case IPL_WIDGET_RADIOBUTTONS:
                widget = new IPPropertyRadioInt(p, this);
                rawName = property->title(); // name:value1|value2
                name = rawName.split(":").at(0);
                addPropertyWidget(name, property->description(), widget);
                break;

            case IPL_WIDGET_COMBOBOX:
                widget = new IPPropertyCombobox(p, this);
                rawName = property->title(); // name:value1|value2
                name = rawName.split(":").at(0);
                addPropertyWidget(name, property->description(), widget);
                break;

            case IPL_WIDGET_GROUP:
                widget = new IPPropertyGroup(p, this);
                rawName = property->title(); // name:value1|value2
                name = rawName.split(":").at(0);
                addPropertyWidget(name, "", widget);

                // connect widget events to grid execution
                connect((IPPropertyGroup*)widget, &IPPropertyGroup::groupChanged, this, &IPProcessPropertiesWidget::showPropertyGroup);

                break;

            case IPL_WIDGET_BUTTON:
                widget = new IPPropertyButtonInt(p, this);
                addPropertyWidget(name, property->description(), widget);
                break;

            default: //IPL_WIDGET_SPINNER
                IPPropertySpinnerInt* widget = new IPPropertySpinnerInt(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;
            }
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyUnsignedInt*>(property))
        {
            IPPropertyWidget *widget = NULL;
            QString rawName, name;


            switch(p->widget())
            {
            //TODO: Implement Sliders, Checkboxes etc.

            case IPL_WIDGET_CHECKBOXES:
                widget = new IPPropertyCheckboxInt(p, this);
                rawName = property->title(); // name:value1|value2
                name = rawName.split(":").at(0);
                addPropertyWidget(name, property->description(), widget);
                break;

            case IPL_WIDGET_BUTTON:
                widget = new IPPropertyButtonUnsignedInt(p, this);
                addPropertyWidget(name, property->description(), widget);
                break;

            default: //IPL_WIDGET_SPINNER
                IPPropertySpinnerUnsignedInt* widget = new IPPropertySpinnerUnsignedInt(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;
            }
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyDouble*>(property))
        {
            IPPropertyWidget *widget = NULL;
            switch(p->widget())
            {
            default: //IPL_WIDGET_SLIDER
                widget = new IPPropertySliderDouble(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;
            }
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyString*>(property))
        {
            IPPropertyWidget *widget = NULL;
            QString rawName, name;
            QString defaultDirectory =  _mainWindow->defaultImagePath();
            switch(p->widget())
            {
            case IPL_WIDGET_FILE_OPEN:
                widget = new IPPropertyFileOpen(p, defaultDirectory, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            case IPL_WIDGET_FILE_SAVE:
                widget = new IPPropertyFileSave(p, this);
                // name:value1|value2
                rawName = property->title();
                name = rawName.split(":").at(0);
                addPropertyWidget(name, property->description(), widget);
                break;

            case IPL_WIDGET_FOLDER:
                widget = new IPPropertyFolder(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            default: //IPL_WIDGET_TEXTFIELD
                widget = new IPPropertyString(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;
            }
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyColor*>(property))
        {
            IPPropertyWidget *widget = NULL;
            switch(p->widget())
            {
            case IPL_WIDGET_COLOR_HSL:
                widget = new IPPropertyColorHSL(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            case IPL_WIDGET_COLOR_HSV:
                widget = new IPPropertyColorHSV(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            default: //IPL_WIDGET_COLOR_RGB
                widget = new IPPropertyColorRGB(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;
            }

            if (auto picker = dynamic_cast<IPLColorPickHandler*>(widget)) // connect to image viewer for color picking
                _mainWindow->imageViewer()->setColorPickHandler(picker);
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyBool*>(property))
        {
            IPPropertyWidget *widget = NULL;
            switch(p->widget())
            {
            default: //IPL_WIDGET_CHECKBOXES
                widget = new IPPropertyCheckbox(p, property->title(), this);
                addPropertyWidget("", property->description(), widget);
                break;
            }
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyVectorInt*>(property))
        {
            IPPropertyWidget *widget = NULL;
            switch(p->widget())
            {
            case IPL_WIDGET_BINARY_MORPHOLOGY:
                widget = new IPPropertyBinaryMorphologyInt(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            case IPL_WIDGET_BINARY_MORPHOLOGY_TRISTATE:
                widget = new IPPropertyBinaryMorphologyTristateInt(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            case IPL_WIDGET_GRAYSCALE_MORPHOLOGY:
                widget = new IPPropertyGrayscaleMorphologyInt(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;

            default: //IPL_WIDGET_KERNEL
                widget = new IPPropertyKernelInt(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;
            }
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyVectorDouble*>(property))
        {
            IPPropertyWidget *widget = NULL;
            QString rawName, name;
            switch(p->widget())
            {
            case IPL_WIDGET_MATRIX:
                widget = new IPPropertyMatrixDouble(p, this);
                rawName = property->title(); // name:rows|cols
                name = rawName.split(":").at(0);
                addPropertyWidget(name, property->description(), widget);
                break;

            default: //IPL_WIDGET_MATRIX
                widget = new IPPropertyMatrixDouble(p, this);
                rawName = property->title(); // name:rows|cols
                name = rawName.split(":").at(0);
                addPropertyWidget(name, property->description(), widget);
                break;
            }
        }

        else if (auto p = dynamic_cast<IPLProcessPropertyPoint*>(property))
        {
            IPPropertyWidget *widget = NULL;
            switch(p->widget())
            {
            default: //IPL_WIDGET_POINT
                widget = new IPPropertyPoint(p, this);
                addPropertyWidget(property->title(), property->description(), widget);
                break;
            }

            if (auto picker = dynamic_cast<IPLCoordinatePickHandler*>(widget)) // connect to image viewer for coordinate picking
                _mainWindow->imageViewer()->setCoordinatePickHandler(picker);
        }
    }
}

void IPProcessPropertiesWidget::closeSettings()
{
    // remove groupChanged events
    for(IPPropertyWidget* widget : _propertyWidgets)
    {
        IPPropertyGroup* groupWidget = dynamic_cast<IPPropertyGroup*>(widget);
        if(groupWidget)
            disconnect(widget, 0, 0, 0);
    }

    _propertyWidgets.clear();

    _mainWindow->imageViewer()->clearColorPickHandler();
    _mainWindow->imageViewer()->clearCoordinatePickHandler();
    QApplication::restoreOverrideCursor();

    if(_processStep)
    {
        _processStep->setEditing(false);
        _processStep = NULL;
    }
}

void IPProcessPropertiesWidget::resetSettings()
{
    for(IPPropertyWidget* widget : _propertyWidgets)
    {
        widget->resetValue();
    }
}

void IPProcessPropertiesWidget::addPropertyWidget(QString label, QString description, IPPropertyWidget *widget)
{
    QFormLayout* layout = (QFormLayout*) this->layout();

    layout->addRow(label, widget);

    // add widget to list
    _propertyWidgets.append(widget);

    // connect widget events to grid execution
    connect(widget, &IPPropertyWidget::changed, _mainWindow, &MainWindow::setParamsHaveChanged);

    QLabel* lblDescription = new QLabel(description);
    lblDescription->setWordWrap(true);
    lblDescription->setStyleSheet("color: #666; font-size: 10px");
    layout->addRow("", lblDescription);
    //    layout->addRow(new QLabel("")); // add a little space
}

/*!
 * \brief Iterates over all process property widgets and shows/hides
 *        them based on group prefix
 *
 *        Example for 2 groups:
 *        -gauss_gamma
 *        -gauss_deviation
 *        -impulse_deviation
 *        -impulse_type
 *
 *        use * to show all groups
 *
 * \param prefix
 */
void IPProcessPropertiesWidget::showPropertyGroup(QString prefix)
{
    for (int i = 0; i < layout()->count(); ++i)
    {
        IPPropertyWidget* widget = dynamic_cast<IPPropertyWidget*>(layout()->itemAt(i)->widget());

        if(!widget || !widget->processProperty())
            continue;

        QString name(widget->processProperty()->name());

        if(name.contains("_"))
        {
            QStringList nameParts = name.split("_");

            // QFormLayout doesn't allow to show/hide rows,
            // so it is easier to enable/disable the controls
            if(nameParts[0] == prefix || prefix == "*")
            {
                widget->setEnabled(true);
            }
            else
            {
                widget->setEnabled(false);
            }
        }
    }
}

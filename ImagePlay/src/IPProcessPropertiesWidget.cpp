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
        QLayoutItem* item = (item = layout()->takeAt(0) );
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
        orderedProperties.push_back(entry.second);

    std::sort(orderedProperties.begin(), orderedProperties.end(), &IPProcessPropertiesWidget::sortByPosition);

    // create all property widgets
    for (auto it = orderedProperties.begin(); it != orderedProperties.end(); ++it)
    {
        IPLProcessProperty* property = *it;

        // generate GUI based on the property type
        if(property->type() == IPL_INT_SLIDER)
        {
            IPPropertySliderInt* widget = new IPPropertySliderInt((IPLProcessPropertyInt*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_INT)
        {
            IPPropertySpinnerInt* widget = new IPPropertySpinnerInt((IPLProcessPropertyInt*) property, this);
            addPropertyWidget(property->name(),property->description(), widget);
        }
        else if(property->type() == IPL_DOUBLE_SLIDER)
        {
            IPPropertySliderDouble* widget = new IPPropertySliderDouble((IPLProcessPropertyDouble*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_DOUBLE_GAMMA)
        {
            IPPropertySliderDouble* widget = new IPPropertySliderDouble((IPLProcessPropertyDouble*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_FILE_OPEN)
        {
            QString defaultDirectory =  _mainWindow->defaultImagePath();
            IPPropertyFileOpen* widget = new IPPropertyFileOpen((IPLProcessPropertyString*) property, defaultDirectory, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_FILE_SAVE)
        {
            IPPropertyFileSave* widget = new IPPropertyFileSave((IPLProcessPropertyString*) property, this);
            // name:value1|value2
            QString rawName(property->name());
            QString name = rawName.split(":").at(0);

            addPropertyWidget(name, property->description(), widget);
        }
        else if(property->type() == IPL_FOLDER)
        {
            IPPropertyFolder* widget = new IPPropertyFolder((IPLProcessPropertyString*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_COLOR_RGB)
        {
            IPPropertyColorRGB* widget = new IPPropertyColorRGB((IPLProcessPropertyColor*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);

            // connect to image viewer for color picking
            _mainWindow->imageViewer()->setColorPickHandler(widget);
        }
        else if(property->type() == IPL_COLOR_HSL)
        {
            IPPropertyColorHSL* widget = new IPPropertyColorHSL((IPLProcessPropertyColor*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_COLOR_HSV)
        {
            IPPropertyColorHSV* widget = new IPPropertyColorHSV((IPLProcessPropertyColor*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_BOOL_CHECKBOX)
        {
            IPPropertyCheckbox* widget = new IPPropertyCheckbox((IPLProcessPropertyBool*) property, property->name(), this);
            addPropertyWidget("", property->description(), widget);
        }
        else if(property->type() == IPL_INT_KERNEL)
        {
            IPPropertyKernelInt* widget = new IPPropertyKernelInt((IPLProcessPropertyVectorInt*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_INT_BINARY_MORPHOLOGY)
        {
            IPPropertyBinaryMorphologyInt* widget = new IPPropertyBinaryMorphologyInt((IPLProcessPropertyVectorInt*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_INT_BINARY_MORPHOLOGY_TRISTATE)
        {
            IPPropertyBinaryMorphologyTristateInt* widget = new IPPropertyBinaryMorphologyTristateInt((IPLProcessPropertyVectorInt*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_INT_GRAYSCALE_MORPHOLOGY)
        {
            IPPropertyGrayscaleMorphologyInt* widget = new IPPropertyGrayscaleMorphologyInt((IPLProcessPropertyVectorInt*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_INT_RADIOBUTTONS)
        {
            IPPropertyRadioInt* widget = new IPPropertyRadioInt((IPLProcessPropertyInt*) property, this);

            // name:value1|value2
            QString rawName(property->name());
            QString name = rawName.split(":").at(0);
            addPropertyWidget(name, property->description(), widget);
        }
        else if(property->type() == IPL_INT_COMBOBOX)
        {
            IPPropertyCombobox* widget = new IPPropertyCombobox((IPLProcessPropertyInt*) property, this);

            // name:value1|value2
            QString rawName(property->name());
            QString name = rawName.split(":").at(0);
            addPropertyWidget(name, property->description(), widget);
        }
        else if(property->type() == IPL_INT_CHECKBOXES)
        {
            IPPropertyCheckboxInt* widget = new IPPropertyCheckboxInt((IPLProcessPropertyUnsignedInt*) property, this);

            // name:value1|value2
            QString rawName(property->name());
            QString name = rawName.split(":").at(0);
            addPropertyWidget(name, property->description(), widget);
        }
        else if(property->type() == IPL_STRING)
        {
            IPPropertyString* widget = new IPPropertyString((IPLProcessPropertyString*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);
        }
        else if(property->type() == IPL_LABEL)
        {
            QFormLayout* layout = (QFormLayout*) this->layout();

            QLabel* lblDescription = new QLabel(property->description());
            lblDescription->setWordWrap(true);
            lblDescription->setStyleSheet("color: #666; font-size: 10px");
            layout->addRow("", lblDescription);

            // add widget to list
            //_propertyWidgets.append(lblDescription);
        }
        else if(property->type() == IPL_POINT)
        {
            IPPropertyPoint* widget = new IPPropertyPoint((IPLProcessPropertyPoint*) property, this);
            addPropertyWidget(property->name(), property->description(), widget);

            // connect to image viewer for color picking
            _mainWindow->imageViewer()->setCoordinatePickHandler(widget);
        }
    }
}

void IPProcessPropertiesWidget::closeSettings()
{
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

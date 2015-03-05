#include "IPProcessScript.h"

void IPProcessScript::init()
{
    // init
    _result     = NULL;
    //_scriptEngine = new QScriptEngine;

    // basic settings
    setClassName("IPProcessScript");
    setTitle("Process Script");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLData::IMAGE_GRAYSCALE);
}

void IPProcessScript::destroy()
{
    delete _result;
}

bool IPProcessScript::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    /*delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    QString script("int result = 0; for(int i=0; i < array.length; i++) { array[i] = i } return result;");

    //QScriptValue image = qScriptValueFromSequence();
    //_scriptEngine->globalObject().setProperty("image", image);

    QScriptValue result = _scriptEngine->evaluate(script, "TestScript");

    qDebug() << "Result: " << result.toInteger();

    //for(int i=0; i<array.)

    /*int progress = 0;
    int maxProgress = image->height();

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);

        for(int x=0; x<width; x++)
        {
            newplane->p(x,y) = plane->p(x,y);
        }
    } */

    return false;
}

IPLImage* IPProcessScript::getResultData( int )
{
    return _result;
}

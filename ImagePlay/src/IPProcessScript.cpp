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
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Gray Image", IPL_IMAGE_GRAYSCALE);
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

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

#include "IPLScreenshot.h"

void IPLScreenshot::init()
{
    // init
    _result         = NULL;
    _winId          = 0;

    // basic settings
    setClassName("IPLScreenshot");
    setTitle("Load from another running window/desktop");
    setCategory(IPLProcess::CATEGORY_IO);
    setIsSource(true);

    // inputs and outputs
    addOutput("Image", IPL_IMAGE_COLOR);

    addProcessPropertyUnsignedInt("trigger", "Trigger Image", "", 0, IPL_WIDGET_BUTTON);
    addProcessPropertyBool("continuous", "Run continuously", "", false, IPL_WIDGET_CHECKBOXES);
    // addProcessPropertyInt("wid", "Window Selector", "", ...);
}

void IPLScreenshot::destroy()
{
    delete _result;
}

bool IPLScreenshot::processInputData(IPLData*, int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;
    _continuous = getProcessPropertyBool("continuous");

    notifyProgressEventHandler(-1);

    // if (const QWindow *window = windowHandle())
    // screen = window->screen();
    QScreen *screen = QGuiApplication::primaryScreen();
    // QDesktopWidget* dw = QApplication::desktop();
    if (!screen)
        return NULL;
    _pixmap = screen->grabWindow(0);

    _qimage = _pixmap.toImage();
    _mat = cv::Mat(_qimage.height(), _qimage.width(), CV_8UC4, (uchar*)_qimage.bits(), _qimage.bytesPerLine());
    _result = new IPLImage(_mat);

    if(!_result)
    {
        addError("Could not fetch screenshot.");
        return false;
    }

    /*
    std::stringstream s;
    s << "Foo ";
    addInformation(s.str());
    */

    return true;
}

IPLImage *IPLScreenshot::getResultData(int)
{
    return _result;
}

void IPLScreenshot::afterProcessing()
{
    if(_continuous)
    {
        notifyPropertyChangedEventHandler();
    }
}

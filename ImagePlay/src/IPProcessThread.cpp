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

#include "IPProcessThread.h"

IPProcessThread::IPProcessThread(IPLProcess *process, IPLImage *image, int inputIndex, bool useOpenCV)
{
    _process    = process;
    _image      = image;
    _inputIndex = inputIndex;
    _success    = false;
    _useOpenCV  = useOpenCV;
}

IPProcessThread::~IPProcessThread()
{
}

void IPProcessThread::run()
{
    _process->registerProgressEventHandler(this);

    try
    {
        _success = _process->processInputData(_image, _inputIndex, _useOpenCV);
    }
    catch(std::exception &e)
    {
        _process->addError(e.what());
    }
    catch(...)
    {
        _process->addError("UNKNOWN ERROR IN THREAD");
    }
}

void IPProcessThread::updateProgress(int percent)
{
    emit progressUpdated(percent);
}

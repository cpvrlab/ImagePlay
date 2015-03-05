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
        _process->resetMessages();
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

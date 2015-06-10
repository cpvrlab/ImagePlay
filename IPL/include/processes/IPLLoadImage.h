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

#ifndef IPLLOADIMAGE_H
#define IPLLOADIMAGE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLFileIO.h"

/**
 * @brief The IPLLoadImage class
 */
class IPLSHARED_EXPORT IPLLoadImage : public IPLClonableProcess<IPLLoadImage>
{
public:
    IPLLoadImage() : IPLClonableProcess() { init(); }
    ~IPLLoadImage()  { destroy(); }

    void                init();
    virtual void        destroy();
    virtual void        beforeProcessing();
    virtual bool        processInputData    (IPLImage* data, int index, bool useOpenCV);
    virtual IPLImage*   getResultData       (int outNr);
    virtual void        afterProcessing();
protected:
    IPLImage*           _result;
    std::string         _path;
};

#endif // IPLLOADIMAGE_H

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

#ifndef IPLLoadVideo_H
#define IPLLoadVideo_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLVideoIO.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


/**
 * @brief The IPLLoadVideo class
 */
class IPLSHARED_EXPORT IPLLoadVideo : public IPLClonableProcess<IPLLoadVideo>
{
public:
                            IPLLoadVideo() : IPLClonableProcess() { init(); }
                            ~IPLLoadVideo()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr);
    virtual void            afterProcessing         ();
protected:
    IPLImage*               _result;
    bool                    _continuous;
    std::string             _path;
};

#endif // IPLLoadVideo_H

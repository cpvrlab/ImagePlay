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
#ifndef IPLSAVEIMAGE_H
#define IPLSAVEIMAGE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLFileIO.h"

/**
 * @brief The IPLSaveImage class
 */
class IPLSHARED_EXPORT IPLSaveImage : public IPLClonableProcess<IPLSaveImage>
{
public:
    IPLSaveImage() : IPLClonableProcess() { init(); }
    ~IPLSaveImage()  { destroy(); }

    void init();
    virtual void        destroy();
    virtual bool processInputData(IPLData*, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outNr );
private:
    bool stringEndsWith(const std::string& haystack, const std::string& needle);
protected:
    IPLImage*   _result;
    std::string _path;
    int         _jpeg_quality;
    bool        _jpeg_progressive;
    int         _bmp_type;
    int         _png_type;
    int         _pnm_type;
    bool        _preview;
};

#endif // IPLSAVEIMAGE_H

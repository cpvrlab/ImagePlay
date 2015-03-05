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
    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
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

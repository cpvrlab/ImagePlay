#include "IPLSaveImage.h"

#include "FreeImage.h"

void IPLSaveImage::init()
{
    // init
    _result = NULL;
    _path = "";
    _jpeg_quality = 75;
    _jpeg_progressive = false;
    _bmp_type = 0;
    _png_type = 0;
    _pnm_type = 0;
    _preview = false;

    // basic settings
    setClassName("IPLSaveImage");
    setTitle("Save Image");
    setCategory(IPLProcess::CATEGORY_IO);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // all properties which can later be changed by gui
    addProcessPropertyString("path", "File:Bitmap (*.bmp);;OpenEXR (*.exr);;JPEG (*.jpg);;PNG (*.png);;Portable BitMap (*.pbm);;Portable GrayMap (*.pgm);;Portable PixMap (*.ppm)",
                                                                             "Bitmap (*.bmp); OpenEXR (*.exr); JPEG (*.jpg); PNG (*.png); Portable BitMap (*.pbm); Portable GrayMap (*.pgm); Portable PixMap (*.ppm)", IPL_FILE_SAVE, _path);
    addProcessPropertyInt("jpeg_quality", "JPEG Quality", "0-100", IPL_INT_SLIDER, _jpeg_quality, 1, 100);
    addProcessPropertyInt("jpeg_progressive", "JPEG Progressive", "", IPL_BOOL_CHECKBOX, _jpeg_progressive);

    addProcessPropertyInt("bmp_type", "BMP Type:DEFAULT|RLE", "", IPL_INT_RADIOBUTTONS, _bmp_type);
    addProcessPropertyInt("png_type", "PNG Type:DEFAULT|INTERLACED", "", IPL_INT_RADIOBUTTONS, _png_type);
    addProcessPropertyInt("pnm_type", "PNM Type:RAW|ASCII", "", IPL_INT_RADIOBUTTONS, _pnm_type);

    addProcessPropertyBool("preview", "Don't save, only Preview", "", IPL_BOOL_CHECKBOX, _preview);

    // BMP_DEFAULT|BMP_SAVE_RLE|EXR|J2K|JPEG|JPEG_PROGRESSIVE|
    // PNG_DEFAULT|PNG_INTERLACED|PNM_SAVE_RAW|PNM_SAVE_ASCII|
    // TIFF_DEFAULT
}

void IPLSaveImage::destroy()
{
    delete _result;
}

bool IPLSaveImage::processInputData(IPLImage* image, int, bool)
{
    //IPLImage* colorImage = new IPLImage(IPLData::COLOR, image->width(), image->height());

    // get properties
    _path           = getProcessPropertyString("path");
    _jpeg_quality   = getProcessPropertyInt("jpeg_quality");
    _jpeg_progressive=getProcessPropertyInt("jpeg_progressive");
    _bmp_type       = getProcessPropertyInt("bmp_type");
    _png_type       = getProcessPropertyInt("png_type");
    _pnm_type       = getProcessPropertyInt("pnm_type");
    _preview        = getProcessPropertyBool("preview");

    if(_path.length() == 0)
    {
        return false;
    }

    // determine format from file extension
    int format = FIF_UNKNOWN;
    int flags = 0;
    if(stringEndsWith(_path, std::string(".bmp")))
    {
        format = FIF_BMP;
        if(_bmp_type == 0)
            flags = BMP_DEFAULT;
        else
            flags = BMP_SAVE_RLE;
    }
    else if (stringEndsWith(_path, std::string(".exr")))
    {
        format = FIF_EXR;
    }
    else if (stringEndsWith(_path, std::string(".jpg")))
    {
        format = FIF_JPEG;
        flags = _jpeg_quality;

    }
    else if (stringEndsWith(_path, std::string(".png")))
    {
        format = FIF_PNG;
        if(_png_type == 0)
            flags = PNG_DEFAULT;
        else
            flags = PNG_INTERLACED;
    }
    else if (stringEndsWith(_path, std::string(".pbm")))
    {
        format = FIF_PBM;
        if(_pnm_type == 0)
            flags = PNM_SAVE_RAW;
        else
            flags = PNM_SAVE_ASCII;
    }
    else if (stringEndsWith(_path, std::string(".pgm")))
    {
        format = FIF_PGM;
        if(_pnm_type == 0)
            flags = PNM_SAVE_RAW;
        else
            flags = PNM_SAVE_ASCII;
    }
    else if (stringEndsWith(_path, std::string(".ppm")))
    {
        format = FIF_PPM;
        if(_pnm_type == 0)
            flags = PNM_SAVE_RAW;
        else
            flags = PNM_SAVE_ASCII;
    }



    notifyProgressEventHandler(50);

    delete _result;
    _result = new IPLImage(image->type(), image->width(), image->height());
    bool success = IPLFileIO::saveFile(_path, image, format, flags, _result, _preview);

    //delete colorImage;

    return success;
}

IPLImage* IPLSaveImage::getResultData(int)
{
    return _result;
}

bool IPLSaveImage::stringEndsWith(const std::string& haystack, const std::string& needle)
{
    uint found = static_cast<int>(haystack.rfind(needle));

    if(found == haystack.length() - needle.length())
        return true;
    else
        return false;
}

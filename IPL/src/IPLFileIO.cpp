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

#include "IPLFileIO.h"

#include "FreeImage.h"

IPLFileIO::IPLFileIO()
{
}

/*!
 * \brief IPLFileIO::loadFile
 * \param filename
 * \param image pass by pointer reference, because we need to change the pointer
 * \return
 */
bool IPLFileIO::loadFile(const std::string filename, IPLImage*& image, std::string& information)
{
    std::string formatNames[37] =  {"BMP", "ICO", "JPEG", "JNG",
                                    "KOALA", "LBM", "MNG", "PBM",
                                    "PBMRAW", "PCD", "PCX", "PGM", "PGMRAW",
                                    "PNG", "PPM", "PPMRAW", "RAS",
                                    "TARGA", "TIFF", "WBMP", "PSD", "CUT",
                                    "XBM", "XPM", "DDS", "GIF", "HDR",
                                    "FAXG3", "SGI", "EXR", "J2K", "JP2",
                                    "PFM", "PICT", "RAW", "WEBP", "JXR"};

    std::string typeNames[13] =   {"UNKNOWN", "BITMAP", "UINT16", "INT16",
                                    "UINT32", "INT32", "FLOAT", "DOUBLE",
                                    "COMPLEX", "RGB16", "RGBA16", "RGBF",
                                    "RGBAF"};

    FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
    format = FreeImage_GetFileType(filename.c_str());
    if(format == FIF_UNKNOWN)
    {
        return false;
    }

    FIBITMAP *dib = FreeImage_Load(format, filename.c_str());
    int width = FreeImage_GetWidth(dib);
    int height = FreeImage_GetHeight(dib);

    // all files need to be flipped
    FreeImage_FlipVertical(dib);

    if(FreeImage_GetBPP(dib) == 8)
    {
        // grayscale images

        // convert to 32 bit
        FIBITMAP *dib2 = FreeImage_ConvertToGreyscale(dib);

        // clear old image
        delete image;
        // create new instance with the right dimensions
        image = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);

        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                BYTE value;
                FreeImage_GetPixelIndex(dib2, x, y, &value);
                image->plane(0)->p(x, y) = (value  * FACTOR_TO_FLOAT);
            }
        }

        FreeImage_Unload(dib2);
    }
    else
    {
        // color images

        // convert to 32 bit
        FIBITMAP *dib2 = FreeImage_ConvertTo32Bits(dib);

        // clear old image
        delete image;
        // create new instance with the right dimensions
        image = new IPLImage(IPLData::IMAGE_COLOR, width, height);

        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                RGBQUAD rgb;
                FreeImage_GetPixelColor(dib2, x, y, &rgb);
                image->plane(0)->p(x, y) = (rgb.rgbRed   * FACTOR_TO_FLOAT);    // R
                image->plane(1)->p(x, y) = (rgb.rgbGreen * FACTOR_TO_FLOAT);    // G
                image->plane(2)->p(x, y) = (rgb.rgbBlue  * FACTOR_TO_FLOAT);    // B
            }
        }

        FreeImage_Unload(dib2);
    }

    FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);

    // collect information
    std::stringstream s;
    s << "<b>Type: </b>" << typeNames[type] << "\n";
    s << "<b>Format: </b>" << formatNames[format] << "\n";
    s << "<b>Bits per Pixel: </b>" << FreeImage_GetBPP(dib) << "\n";
    s << "<b>Width: </b>" << width << "\n";
    s << "<b>Height: </b>" << height << "";

    information = s.str();

    // free temporary memory
    FreeImage_Unload(dib);

    return true;
}


bool IPLFileIO::loadMemory(void* mem, IPLImage*& image)
{
    FIMEMORY* hmem = (FIMEMORY*) mem;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FreeImage_SeekMemory(hmem, 0L, SEEK_SET);
    fif = FreeImage_GetFileTypeFromMemory(hmem, 0);

    if(fif == FIF_UNKNOWN)
    {
        // always close the memory stream
        FreeImage_CloseMemory(hmem);
        return false;
    }

    FIBITMAP *dib = FreeImage_LoadFromMemory(fif, hmem);
    int width = FreeImage_GetWidth(dib);
    int height = FreeImage_GetHeight(dib);

    // all files need to be flipped
    FreeImage_FlipVertical(dib);

    if(FreeImage_GetBPP(dib) == 8)
    {
        // grayscale images

        // convert to 32 bit
        FIBITMAP *dib2 = FreeImage_ConvertToGreyscale(dib);

        // clear old image
        delete image;
        // create new instance with the right dimensions
        image = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);

        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                BYTE value;
                FreeImage_GetPixelIndex(dib2, x, y, &value);
                image->plane(0)->p(x, y) = (value  * FACTOR_TO_FLOAT);
            }
        }

        FreeImage_Unload(dib2);
    }
    else
    {
        // color images

        // convert to 32 bit
        FIBITMAP *dib2 = FreeImage_ConvertTo32Bits(dib);

        // clear old image
        delete image;
        // create new instance with the right dimensions
        image = new IPLImage(IPLData::IMAGE_COLOR, width, height);

        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                RGBQUAD rgb;
                FreeImage_GetPixelColor(dib2, x, y, &rgb);
                image->plane(0)->p(x, y) = (rgb.rgbRed   * FACTOR_TO_FLOAT);    // R
                image->plane(1)->p(x, y) = (rgb.rgbGreen * FACTOR_TO_FLOAT);    // G
                image->plane(2)->p(x, y) = (rgb.rgbBlue  * FACTOR_TO_FLOAT);    // B
            }
        }

        FreeImage_Unload(dib2);
    }

    // always close the memory stream
    FreeImage_CloseMemory(hmem);


    // free temporary memory
    FreeImage_Unload(dib);

    return true;
}

bool IPLFileIO::saveFile(const std::string path, IPLImage* image, int format, int flags, IPLImage* result, bool preview)
{
    int width = image->width();
    int height = image->height();

    FIBITMAP *dib = FreeImage_Allocate(width, height, 24);

    if(image->type() == IPLData::IMAGE_COLOR)
    {
        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                RGBQUAD rgb;
                rgb.rgbRed      = static_cast<BYTE>(image->plane(0)->p(x, y) * FACTOR_TO_UCHAR); // R
                rgb.rgbGreen    = static_cast<BYTE>(image->plane(1)->p(x, y) * FACTOR_TO_UCHAR); // G
                rgb.rgbBlue     = static_cast<BYTE>(image->plane(2)->p(x, y) * FACTOR_TO_UCHAR); // B
                FreeImage_SetPixelColor(dib, x, y, &rgb);
            }
        }
    }
    else
    {
        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                unsigned char value = image->plane(0)->p(x, y) * FACTOR_TO_UCHAR;
                RGBQUAD rgb;
                rgb.rgbRed      = value; // R
                rgb.rgbGreen    = value; // G
                rgb.rgbBlue     = value; // B
                FreeImage_SetPixelColor(dib, x, y, &rgb);
            }
        }
    }

    // all files need to be flipped
    FreeImage_FlipVertical(dib);

    bool success = false;

    if(preview)
    {
        // only save to memory for preview
        FIMEMORY* hmem = NULL;
        // open and allocate a memory stream
        hmem = FreeImage_OpenMemory();

        success = FreeImage_SaveToMemory((FREE_IMAGE_FORMAT)format, dib, hmem, flags) != 0;

        // write to result
        if(result)
        {
            loadMemory((void*)hmem, result);
        }

        //FreeImage_CloseMemory(hmem);

    }
    else
    {
        // actually save file
        success = FreeImage_Save((FREE_IMAGE_FORMAT)format, dib, path.c_str(), flags) != 0;

        // write to result
        if(result)
        {
            std::string information;
            loadFile(path, result, information);
        }
    }


    // free temporary memory
    FreeImage_Unload(dib);

    return success;
}

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

#ifndef IPLFILEIO_H
#define IPLFILEIO_H

#include "IPL_global.h"
#include "IPLImage.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

/**
 * @brief The IPLFileIO class
 */
class IPLSHARED_EXPORT IPLFileIO
{
public:
    IPLFileIO();

    static bool loadFile(const std::string filename, IPLImage*& image, std::string& information);
    static bool loadMemory(void* hmem, IPLImage*& image);
    static bool saveFile(const std::string path, IPLImage* image, int format, int flags, IPLImage* result = 0, bool preview = false);

    static bool loadRawFile(const std::string filename, IPLImage*& image, int width, int height, IPLRawImageType format, bool interleaved, std::string& information);
    static bool readRaw8bit(int stride, IPLImage *&image, std::ifstream &file);
    static bool readRaw24BitInterleaved(int stride, IPLRawImageType format, IPLImage *&image, std::ifstream &file);
    static bool readRaw32BitInterleaved(int stride, IPLRawImageType format, IPLImage *&image, std::ifstream &file);
    static bool readRaw24BitPlanar(int stride, IPLRawImageType format, IPLImage *&image, std::ifstream &file);
    static bool readRaw32BitPlanar(int stride, IPLRawImageType format, IPLImage *&image, std::ifstream &file);

    static void setBasedir(std::string dir) { _baseDir = dir; }

    // Naive approach:
    // win: C:\... D://...
    // unix: /var/...
    static bool isAbsolutePath(std::string filename) { return (filename.substr(0, 1) == "/" || filename.substr(1, 1) == ":"); }

    static std::string _baseDir;
};

#endif // IPLFILEIO_H

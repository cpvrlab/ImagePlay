#ifndef IPLFILEIO_H
#define IPLFILEIO_H

#include "IPL_global.h"
#include "IPLImage.h"

#include <string>
#include <vector>

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
};

#endif // IPLFILEIO_H

#pragma once

#include <string>
#include <fstream>

namespace PPMImage
{
    void writeAsciiImage(std::string const& path, const char* data, uint32_t width, uint32_t height, uint32_t bpp);
    void writeBinaryImage(std::string const& path, const char* data, uint32_t width, uint32_t height, uint32_t bpp);
};

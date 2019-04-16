#include <iostream>
#include "ppm.hpp"

void PPMImage::writeAsciiImage(std::string const& path, const char* data, uint32_t width, uint32_t height, uint32_t bpp)
{
    std::fstream file(path, std::ios_base::trunc | std::ios_base::out);

    file << "P3" << std::endl;
    file << width << " " << height << std::endl;
    file << "255" << std::endl;
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            auto const* color = data + ((y * width) + x) * bpp;
            if (x != 0)
                file << "    ";
            file << int(color[0]) << " " << int(color[1]) << " " << int(color[2]);
        }
        file << std::endl;
    }
}

void PPMImage::writeBinaryImage(std::string const& path, const char* data, uint32_t width, uint32_t height, uint32_t bpp)
{
    std::fstream file(path, std::ios_base::trunc | std::ios_base::out | std::ios_base::binary);

    file << "P6" << std::endl;
    file << width << " " << height << std::endl;
    file << "255" << std::endl;
    if (bpp == 3)
    {
        file.write(data, width * height * 3);
    }
    else if (bpp > 3)
    {
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                auto const* tmp = data + ((y * width) + x) * 3;
                file.write(tmp, sizeof(uint8_t) * 3);
            }
        }
    }
    else
        std::cerr << "PPMImage: invalid bits per pixel format" << std::endl;
}

#include <iostream>
#include <string>
#include "utils/drawable.hpp"
#include "utils/ppm.hpp"

int main(int argc, char** argv)
{
    uint32_t width = 256;
    uint32_t height = 256;
    Drawable gd(width, height);
    uint32_t color_tab[6]{ BLUE, BROWN, RED, ORANGE, GREEN, YELLOW};

    gd.opaquerect(Rect(0, 0, width, height), gd.u32_to_color(BLACK));
    size_t colorId = 0;
    for (uint32_t y = 0; y < height; y += 32)
    {
        for (uint32_t x = 0; x < width; x += 32)
        {
            gd.opaquerect(Rect(x, y, 32, 32), gd.u32_to_color(color_tab[colorId]));
            ++colorId;
            if (colorId >= 6)
                colorId = 0;
        }
    }

    // save in ppm file
    PPMImage::writeBinaryImage("pentamino.ppm", reinterpret_cast<char const*>(gd.data()), gd.width(), gd.height(), gd.bpp() / 8);
    return 0;
}

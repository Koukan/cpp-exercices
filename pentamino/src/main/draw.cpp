#include "utils/drawable.hpp"
#include "utils/png.hpp"

void save_to_png(const char * filename, const Drawable & gd)
{
    std::FILE * file = std::fopen(filename, "w+");
    dump_png24(file, gd.data(), gd.width(), gd.height(), gd.rowsize(), true);
    std::fclose(file);
}


int main(int argc, char** argv)
{
    // Create a simple capture image and dump it to file
    uint16_t width = 640;
    uint16_t height = 480;
    Rect screen_rect(0, 0, width, height);
    Drawable gd(width, height);
    gd.opaquerect(screen_rect, gd.u32bgr_to_color(WHITE));
    gd.opaquerect(screen_rect.shrink(5), gd.u32bgr_to_color(BLACK));

    // uncomment to see result in png file
    save_to_png("./sample.png", gd);
}


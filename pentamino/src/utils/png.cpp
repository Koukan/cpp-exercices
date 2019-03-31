#include "utils/png.hpp"
#include "utils/drawable.hpp"
#include "utils/sugar/array_view.hpp"
#include "utils/sugar/numerics/safe_conversions.hpp"

#include <cassert>
#include <cstdint>

#include <png.h>

namespace detail {
    

    template<class IsOk>
    static void dump_png24_impl(
        png_struct * ppng, png_info * pinfo,
        const uint8_t * data, const size_t width, const size_t height, const size_t rowsize,
        const bool bgr, IsOk is_ok
    ) {
        assert(align4(rowsize) == rowsize);

        png_set_IHDR(ppng, pinfo, width, height, 8,
                    PNG_COLOR_TYPE_RGB,
                    PNG_INTERLACE_NONE,
                    PNG_COMPRESSION_TYPE_BASE,
                    PNG_FILTER_TYPE_BASE);
        png_write_info(ppng, pinfo);

        // send image buffer to file, one pixel row at once
        const uint8_t * row = data;

        if (bgr) {
            uint8_t bgrtmp[8192*4];
            for (size_t k = 0 ; k < height && is_ok(); ++k) {
                const uint8_t * s = row;
                uint8_t * t = bgrtmp;
                uint8_t * e = t + (width / 4) * 12;
                for (; t < e; s += 12, t += 12){
                    t[0] = s[2];
                    t[1] = s[1];
                    t[2] = s[0];

                    t[3] = s[5];
                    t[4] = s[4];
                    t[5] = s[3];

                    t[6] = s[8];
                    t[7] = s[7];
                    t[8] = s[6];

                    t[9] = s[11];
                    t[10] = s[10];
                    t[11] = s[9];
                }
                png_write_row(ppng, const_cast<unsigned char*>(bgrtmp));
                row += rowsize;
            }
        }
        else {
            for (size_t k = 0 ; k < height && is_ok(); ++k) {
                png_write_row(ppng, const_cast<unsigned char*>(row));
                row += rowsize;
            }
        }
    }

    struct PngWriteStruct
    {
        png_struct * ppng = nullptr;
        png_info * pinfo = nullptr;

        PngWriteStruct()
        {
            ppng = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            pinfo = png_create_info_struct(ppng);
        }

        ~PngWriteStruct()
        {
            png_destroy_write_struct(&ppng, &pinfo);
        }
    };
}

void dump_png24(
    std::FILE * fd, uint8_t const * data,
    const size_t width, const size_t height, const size_t rowsize,
    const bool bgr
) {
    detail::PngWriteStruct png;

    // prepare png header
    png_init_io(png.ppng, fd);

    detail::dump_png24_impl(png.ppng, png.pinfo, data, width, height, rowsize, bgr, []{return true;});

    png_write_end(png.ppng, png.pinfo);

    // commented line below it to create row capture
    // fwrite(this->data, 3, this->width * this->height, fd);
}



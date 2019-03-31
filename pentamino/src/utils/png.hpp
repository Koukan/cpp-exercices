#pragma once

#include <cstddef>
#include <cstdio> // FILE
#include "utils/sugar/array_view.hpp"

class ConstImageDataView;

void dump_png24(
    std::FILE * fd, uint8_t const * data,
    const size_t width, const size_t height, const size_t rowsize,
    const bool bgr
);


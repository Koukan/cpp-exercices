#pragma once

enum {
    ERR_RECORDER_EMPTY_IMAGE = 1,
    ERR_RECORDER_FRAME_ALLOCATION_FAILED = 2,
};

#define LOG2(XXX, YYY, ZZZ) printf(YYY, ZZZ)
#define LOG1(XXX, YYY) printf(YYY)

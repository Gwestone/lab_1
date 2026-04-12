#include "digits.h"

int digits[10] = {
    SEG_TOP_LEFT | SEG_TOP | SEG_TOP_RIGHT | SEG_BOTTOM_LEFT | SEG_BOTTOM | SEG_BOTTOM_RIGHT,               // 0
    SEG_TOP_LEFT | SEG_BOTTOM_LEFT,                                                                         // 1
    SEG_TOP | SEG_TOP_RIGHT | SEG_MIDDLE | SEG_BOTTOM_LEFT | SEG_BOTTOM,                                    // 2
    SEG_TOP | SEG_TOP_LEFT | SEG_MIDDLE | SEG_BOTTOM_LEFT | SEG_BOTTOM,                                     // 3
    SEG_TOP_LEFT | SEG_MIDDLE | SEG_BOTTOM_LEFT | SEG_BOTTOM_RIGHT,                                         // 4
    SEG_TOP | SEG_TOP_LEFT | SEG_MIDDLE | SEG_BOTTOM_RIGHT | SEG_BOTTOM,                                    // 5
    SEG_TOP | SEG_TOP_LEFT | SEG_TOP_RIGHT | SEG_MIDDLE | SEG_BOTTOM_RIGHT | SEG_BOTTOM,                    // 6
    SEG_TOP_LEFT | SEG_BOTTOM_LEFT | SEG_BOTTOM,                                                            // 7
    SEG_TOP | SEG_TOP_LEFT | SEG_TOP_RIGHT | SEG_MIDDLE | SEG_BOTTOM_LEFT | SEG_BOTTOM | SEG_BOTTOM_RIGHT,  // 8
    SEG_TOP | SEG_TOP_LEFT | SEG_MIDDLE | SEG_BOTTOM_LEFT | SEG_BOTTOM | SEG_BOTTOM_RIGHT,                  // 9
};

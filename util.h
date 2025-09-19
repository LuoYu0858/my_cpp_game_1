#ifndef PLANTSVSPLANTS_UTIL_H
#define PLANTSVSPLANTS_UTIL_H

#include "atlas.h"

#include <graphics.h>

#pragma comment(lib, "MSIMG32.LIB")

inline void flip_image(IMAGE* src, IMAGE* dst) {
    int w = src->getwidth(), h = src->getheight();
    Resize(dst, w, h);
    DWORD* src_buffer = GetImageBuffer(src);
    DWORD* dst_buffer = GetImageBuffer(dst);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int idx_src = y * w + x;
            int idx_dst = y * w + (w - x - 1);
            dst_buffer[idx_dst] = src_buffer[idx_src];
        }
    }
}

inline void flip_atlas(Atlas& src, Atlas& dst) {
    dst.clear();
    for (int i = 0; i < src.get_size(); ++i) {
        IMAGE img_flipped;
        flip_image(src.get_image(i), &img_flipped);
        dst.add_image(img_flipped);
    }
}

inline void putimage_alpha(int dst_x, int dst_y, IMAGE* img) {
    int w = img->getwidth(), h = img->getheight();
    AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h,
               GetImageHDC(img), 0, 0, w, h,
               {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}

#endif //PLANTSVSPLANTS_UTIL_H

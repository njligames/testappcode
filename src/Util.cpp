//
//  Util.cpp
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#include "Util.h"

#ifndef STB_RECT_PACK_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATIONs
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#endif

#include "curl/curl.h"

void *Util::loadImage(const std::string &filePath, int *width, int *height,
                         int *channels_in_file) {

    const std::string base_path(SDL_GetBasePath());

    std::string fullFilePath = base_path + filePath;

    void *buffer = (void *)stbi_load(fullFilePath.c_str(), width, height,
                                     channels_in_file, 4);
    return buffer;
}

void *Util::loadDefaultCardImage(int *width, int *height,
                                    int *channels_in_file, size_t *fileSize) {

    std::string base_path(SDL_GetBasePath());
    std::string defaultImage("assets/Default.png");
    base_path += defaultImage;

    void *buffer = (void *)stbi_load(base_path.c_str(), width, height,
                                     channels_in_file, 0);

    if (nullptr != buffer) {
        *fileSize = ((*width) * (*height) * (*channels_in_file));
    }

    return buffer;
}

//
// Created by souvik on 10/2/25.
//

#include "image_io.h"
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

unsigned char* load_image_data(const std::string& path, int& width, int& height, int& channels) {
    return stbi_load(path.c_str(), &width, &height, &channels, 0);
}

bool save_image_data(const std::string& path, unsigned char* data, int width, int height, int channels) {
    return stbi_write_png(path.c_str(), width, height, channels, data, width * channels);
}
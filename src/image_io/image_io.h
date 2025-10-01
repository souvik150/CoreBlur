//
// Created by souvik on 10/2/25.
//

#ifndef MYPROJECT_IMAGE_IO_H
#define MYPROJECT_IMAGE_IO_H
#include <string>

unsigned char* load_image_data(const std::string& path, int& width, int& height, int& channels);
bool save_image_data(const std::string& path, unsigned char* data, int width, int height, int channels);

#endif //MYPROJECT_IMAGE_IO_H
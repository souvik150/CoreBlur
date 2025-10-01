//
// Created by souvik on 10/1/25.
//

#include "gaussian_blur.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>


std::vector<float> GaussianBlur::createKernel(int size, double sigma) {
    std::vector<float> kernel(size * size);
    double sum = 0.0;
    int half = size / 2;

    for (int y = -half; y <= half; y++) {
        for (int x = -half; x <= half; x++) {
            double value = std::exp(-(x * x + y * y) / (2 * sigma * sigma)) /
                           (2 * M_PI * sigma * sigma);
            kernel[(y + half) * size + (x + half)] = static_cast<float>(value);
            sum += value;
        }
    }

    // normalize kernel
    for (float& v : kernel) v /= static_cast<float>(sum);
    return kernel;
}

void GaussianBlur::applyKernel(const unsigned char* input,
                               unsigned char* output,
                               int width, int height, int channels,
                               const std::vector<float>& kernel) {
    int kSize = static_cast<int>(std::sqrt(kernel.size()));
    int half = kSize / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                double sum = 0.0;

                for (int ky = -half; ky <= half; ky++) {
                    for (int kx = -half; kx <= half; kx++) {
                        int px = std::ranges::clamp(x + kx, 0, width - 1);
                        int py = std::ranges::clamp(y + ky, 0, height - 1);

                        float kval = kernel[(ky + half) * kSize + (kx + half)];
                        sum += kval * input[(py * width + px) * channels + c];
                    }
                }

                output[(y * width + x) * channels + c] = static_cast<unsigned char>(sum);
            }
        }
    }
}

bool GaussianBlur::processTile(const unsigned char* input,
                               unsigned char* output,
                               int width,
                               int height,
                               int channels,
                               const Params& params) {
    auto kernel = createKernel(params.kernelSize, params.sigma);
    applyKernel(input, output, width, height, channels, kernel);
    return true;
}
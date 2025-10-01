//
// Created by souvik on 10/1/25.
//

#ifndef CORE_BLUR_GAUSSIAN_BLUR_H
#define CORE_BLUR_GAUSSIAN_BLUR_H
#include <string>
#include <vector>

class GaussianBlur {
public:
    struct Params {
        int kernelSize;   // must be odd (e.g., 3, 5, 7)
        double sigma;     // Gaussian sigma
    };

    bool processTile(const unsigned char* input,
                     unsigned char* output,
                     int width,
                     int height,
                     int channels,
                     const Params& params);

private:
    std::vector<float> createKernel(int size, double sigma);
    static void applyKernel(const unsigned char* input,
                     unsigned char* output,
                     int width,
                     int height,
                     int channels,
                     const std::vector<float>& kernel);
};

#endif //CORE_BLUR_GAUSSIAN_BLUR_H
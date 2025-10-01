//
// Created by souvik on 10/1/25.
//
#include <iostream>
#include <chrono>

#include "gaussian_blur.h"


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input> <output> [kernelSize] [sigma]\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    int kernelSize = (argc > 3) ? std::stoi(argv[3]) : 5;
    double sigma = (argc > 4) ? std::stod(argv[4]) : 1.0;

    GaussianBlur blur;
    GaussianBlur::Params params{kernelSize, sigma};

    auto start = std::chrono::high_resolution_clock::now();
    if (!blur.processImage(inputPath, outputPath, params)) {
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Gaussian blur completed in " << elapsed.count() << " ms\n";

    return 0;
}
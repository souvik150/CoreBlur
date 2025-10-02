//
// Created by souvik on 10/1/25.
//
#include <iostream>
#include <chrono>

#include "gaussian_blur.h"
#include "image_io.h"


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input> <output> [kernelSize] [sigma]\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    int kernelSize = (argc > 3) ? std::stoi(argv[3]) : 5;
    double sigma = (argc > 4) ? std::stod(argv[4]) : 1.0;

    int width, height, channels;
    auto input = load_image_data(inputPath, width, height, channels);
    if (!input) {
        std::cerr << "Error: Could not load image " << inputPath << "\n";
        return 1;
    }

    std::vector<unsigned char> output(width * height * channels);

    GaussianBlur blur;
    GaussianBlur::Params params{kernelSize, sigma};

    auto start = std::chrono::high_resolution_clock::now();
    blur.processTile(input, output.data(), width, height, channels, params);
    auto end = std::chrono::high_resolution_clock::now();

    if (!save_image_data(outputPath, output.data(), width, height, channels)) {
        std::cerr << "Error: Could not save image " << outputPath << "\n";
        return 1;
    }

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Gaussian blur completed in " << elapsed.count() << " ms\n";

    return 0;
}

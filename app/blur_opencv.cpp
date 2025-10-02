//
// Created by souvik on 10/2/25.
//
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input> <output> [kernel_size] [sigma]\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    int kernel_size = (argc > 3) ? std::stoi(argv[3]) : 5;
    double sigma = (argc > 4) ? std::stod(argv[4]) : 1.5;

    // Kernel size must be odd
    if (kernel_size % 2 == 0) {
        std::cerr << "Kernel size must be odd, increasing by 1\n";
        kernel_size += 1;
    }

    // Load input image
    cv::Mat img = cv::imread(inputPath, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cerr << "Failed to load image: " << inputPath << "\n";
        return 1;
    }

    cv::Mat blurred;

    // Benchmark timing
    auto t0 = std::chrono::high_resolution_clock::now();
    cv::GaussianBlur(img, blurred, cv::Size(kernel_size, kernel_size), sigma);
    auto t1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = t1 - t0;
    std::cout << "Gaussian blur completed in " << elapsed.count() << " ms\n";

    if (!cv::imwrite(outputPath, blurred)) {
        std::cerr << "Failed to save image: " << outputPath << "\n";
        return 1;
    }

    std::cout << "Output saved successfully: " << outputPath << "\n";
    return 0;
}

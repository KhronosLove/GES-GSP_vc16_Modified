#include "ImageCropper.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>

namespace fs = std::filesystem;

ImageCropper::ImageCropper(const std::string& directory)
    : directoryPath(directory) {}

// Crop a portion of the image
cv::Mat ImageCropper::cropImage(const cv::Mat& img, int x, int y, int width, int height) {
    // Define the rectangle to crop
    cv::Rect cropRegion(x, y, width, height);
    return img(cropRegion);
}

// Crop images according to the provided specifications
void ImageCropper::cropImages() {
    // List of images to process
    std::map<std::string, std::pair<cv::Point, cv::Size>> cropInfo = {
        {"0.png", {cv::Point(1740, 0), cv::Size(2100, 2160)}},
        {"1.png", {cv::Point(1740, 0), cv::Size(2100, 2160)}},
        {"2.png", {cv::Point(0, 0), cv::Size(2700, 2160)}},
        {"3.png", {cv::Point(0, 0), cv::Size(2700, 2160)}}
    };

    for (const auto& entry : cropInfo) {
        std::string filePath = directoryPath + "/" + entry.first;

        // Check if the file exists
        if (!fs::exists(filePath)) {
            std::cerr << "File not found: " << filePath << std::endl;
            continue;
        }

        // Load the image
        cv::Mat img = cv::imread(filePath);
        if (img.empty()) {
            std::cerr << "Failed to load image: " << filePath << std::endl;
            continue;
        }

        // Crop the image based on the given coordinates and size
        cv::Point topLeft = entry.second.first;
        cv::Size cropSize = entry.second.second;
        cv::Mat croppedImg = cropImage(img, topLeft.x, topLeft.y, cropSize.width, cropSize.height);

        // Save the cropped image, replacing the original one
        if (!cv::imwrite(filePath, croppedImg)) {
            std::cerr << "Failed to save cropped image: " << filePath << std::endl;
        }
    }

    std::cout << "Cropping completed." << std::endl;
}

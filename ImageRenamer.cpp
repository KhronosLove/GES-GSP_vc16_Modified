#include "ImageRenamer.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>

namespace fs = std::filesystem;

ImageRenamer::ImageRenamer(const std::string& directory)
    : directoryPath(directory) {}

// Rotate the image by 180 degrees
cv::Mat ImageRenamer::rotateImage180(const cv::Mat& img) {
    cv::Mat rotated;
    cv::rotate(img, rotated, cv::ROTATE_180); // Rotate 180 degrees
    return rotated;
}

// Rename and process images as per the instructions
void ImageRenamer::renameAndProcessImages() {
    // Map of original names to new names
    std::map<std::string, std::string> renameMap = {
        {"c1.png", "1.png"},
        {"c2.png", "3.png"},
        {"c3.png", "2.png"},
        {"c4.png", "0.png"}
    };

    for (const auto& entry : renameMap) {
        std::string oldFilePath = directoryPath + "/" + entry.first;
        std::string newFilePath = directoryPath + "/" + entry.second;

        // Check if the file exists
        if (!fs::exists(oldFilePath)) {
            std::cerr << "File not found: " << oldFilePath << std::endl;
            continue;
        }

        // Load the image
        cv::Mat img = cv::imread(oldFilePath);
        if (img.empty()) {
            std::cerr << "Failed to load image: " << oldFilePath << std::endl;
            continue;
        }

        // If it's c3.png or c4.png, rotate the image by 180 degrees
        if (entry.first == "c3.png" || entry.first == "c4.png") {
            img = rotateImage180(img);
        }

        // Save the image with the new name
        if (!cv::imwrite(newFilePath, img)) {
            std::cerr << "Failed to save image: " << newFilePath << std::endl;
        }

        // Remove the old file
        fs::remove(oldFilePath);
    }

    std::cout << "Renaming and processing completed." << std::endl;
}

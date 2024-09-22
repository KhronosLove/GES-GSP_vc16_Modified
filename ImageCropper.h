#ifndef IMAGECROPPER_H
#define IMAGECROPPER_H

#include <string>
#include <opencv2/opencv.hpp>

class ImageCropper {
public:
    // Constructor that sets the directory path
    ImageCropper(const std::string& directory);

    // Method to crop the images
    void cropImages();

private:
    std::string directoryPath;

    // Method to crop an image given a specific region
    cv::Mat cropImage(const cv::Mat& img, int x, int y, int width, int height);
};

#endif // IMAGECROPPER_H

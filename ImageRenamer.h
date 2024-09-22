#ifndef IMAGERENAMER_H
#define IMAGERENAMER_H

#include <string>
#include <opencv2/opencv.hpp>

class ImageRenamer {
public:
    // Constructor that sets the directory path
    ImageRenamer(const std::string& directory);

    // Method to rename and process the images
    void renameAndProcessImages();

private:
    std::string directoryPath;

    // Method to rotate an image by 180 degrees
    cv::Mat rotateImage180(const cv::Mat& img);
};

#endif // IMAGERENAMER_H

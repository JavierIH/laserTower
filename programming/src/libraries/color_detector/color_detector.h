#ifndef RGB_DETECTOR_H
#define RGB_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat extractChannel(cv::Mat,int);
cv::Mat detectColor(std::string, cv::Mat);


#endif // RGB_DETECTOR_H

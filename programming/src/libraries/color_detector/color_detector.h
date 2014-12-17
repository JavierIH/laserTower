#ifndef RGB_DETECTOR_H
#define RGB_DETECTOR_H

#include <opencv2/opencv.hpp>

//-- Can color definitions
static const int COLOR_RED = 0;
static const int COLOR_BLUE = 1;
static const int COLOR_GREEN = 2;
static const int COLOR_CYAN = 3;
static const int COLOR_MAGENTA = 4;
static const int COLOR_YELLOW = 5;

cv::Mat extractChannel(cv::Mat,int);
cv::Mat detectColorRGB(std::string, cv::Mat);
cv::Mat detectColorRGB(int, cv::Mat);
cv::Mat1b dilation(cv::Mat1b,int);
cv::Mat detectColorHSV(std::string, cv::Mat);
cv::Mat detectColorHSV(int color, cv::Mat);

std::vector<cv::Point> getTarget(std::string, cv::Mat);
std::vector<cv::Point> getTarget(int, cv::Mat);
std::vector<cv::Point> getTargetHSV(std::string , cv::Mat);
std::vector<cv::Point> getTargetHSV(int, cv::Mat);




#endif // RGB_DETECTOR_H

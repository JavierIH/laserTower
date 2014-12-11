#ifndef WEBCAM_H
#define WEBCAM_H

#include <opencv2/opencv.hpp>

class Webcam{
private:
     cv::VideoCapture capture;
     int id;
public:
     Webcam(int);
     bool destroy();
     cv::Mat getFrame();
     void cleanBuffer();
};



#endif // WEBCAM_H

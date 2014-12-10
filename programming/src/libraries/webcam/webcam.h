#ifndef WEBCAM_H
#define WEBCAM_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Webcam{
private:
     cv::VideoCapture capture;
     int id;
public:
     Webcam(int);
     cv::Mat getFrame();
     void cleanBuffer();
};



#endif // WEBCAM_H

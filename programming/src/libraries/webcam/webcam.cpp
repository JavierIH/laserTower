#include "webcam.h"
#include "../debug/debug.h"

Webcam::Webcam(int device=-1){
    cv::VideoCapture cap(device);
    if(!cap.isOpened()) report(ERROR,"Camera failed at opening");
    else{
        capture=cap;
        report(OK,"Camera initialized correctly");
    }
}

cv::Mat Webcam::getFrame(){
    cleanBuffer();
    cv::Mat frame;
    capture.read(frame);
    return frame;
}

void Webcam::cleanBuffer(){
    for(int i=0; i<5; i++) capture.grab();
}

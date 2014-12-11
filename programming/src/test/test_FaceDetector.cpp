#include "../libraries/webcam/webcam.h"
#include "../libraries/FaceDetector/facedetector.hpp"
#include "../libraries/debug/debug.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <unistd.h>



int main()
{
    cv::VideoCapture capture(-1);

    if(!capture.isOpened())
    {
        report(ERROR,"Camera failed at opening");
    }

    FaceDetector faceDetector;

    while( true)
    {
        cv::Mat frame;
        capture.read(frame);

        std::vector<cv::Rect> faces = faceDetector.detect(frame);

        for (int i = 0; i < faces.size(); i++)
            cv::rectangle(frame, faces[i], cv::Scalar(0, 0, 255));

        try {
            cv::imshow("Faces", frame);
        } catch (cv::Exception e) {
            report(ERROR, e.what());
        }

        char key = cv::waitKey(30);
        if (  (char) key == 'q')
            break;
    }

}

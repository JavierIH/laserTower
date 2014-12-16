#include "../libraries/Turret/turret.hpp"
#include "../libraries/FaceDetector/facedetector.hpp"
#include "../libraries/debug/debug.h"

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include <yarp/os/all.h>
#include <opencv2/opencv.hpp>

int main(int argc, char ** argv)
{
    //-- Start yarp server
//    yarp::os::Network::init();
//    yarp::os::Network::runNameServer(argc, argv);

    //-- Setup Turret
    Turret myTurret("/dev/ttyACM0");

    if (!myTurret.start())
    {
        std::cerr << "[Test] Not able to connect!" << std::endl;
        return 1;
    }

    printf("[Test] Turret Connected\n");


    //-- Create face detector
    FaceDetector faceDetector;


    //-- Open webcam
    cv::VideoCapture capture(1);

    if(!capture.isOpened())
    {
        report(ERROR,"Camera failed at opening");
    }

    //-- P controller params
    float kpx = 0.02;
    float kpy = 0.02;

    //-- Control loop goes here
    do
    {
        //-- Get image from webcam
        cv::Mat frame;
        capture.read(frame);

        //-- Extract targets
        std::vector<cv::Rect> faces = faceDetector.detect(frame);

        //-- If target found
        if (faces.size() > 0)
        {
            //-- calculate error
            int center_x = faces[0].x + faces[0].width / 2;
            int center_y = faces[0].y + faces[0].height / 2;

            int error_x = frame.cols / 2 - center_x;
            int error_y = frame.rows / 2 - center_y;

            std::cout << "----------------------------------------------" << std::endl;
            std::cout << "Error x: " << error_x << std::endl;
            std::cout << "Error y: " << error_y << std::endl;

            //-- P controller
            int move_x = - error_x * kpx;
            int move_y = - error_y * kpy;

            //-- Command motors
            myTurret.movePanInc(move_x);
            myTurret.moveTiltInc(move_y);


            //-- Plotting
            cv::rectangle(frame, faces[0], cv::Scalar(0, 0, 255));
            cv::circle(frame, cv::Point(center_x, center_y ), 2, cv::Scalar(0, 0, 255), 2);

        }

        cv::circle(frame, cv::Point(frame.cols / 2, frame.rows / 2 ), 2, cv::Scalar(255, 0, 0), 2);

        cv::imshow("out", frame);
        char key = cv::waitKey(30);
        if ( key == 27 || key == 'q' )
            break;

    } while (true);


    myTurret.destroy();
    yarp::os::Network::fini();
}

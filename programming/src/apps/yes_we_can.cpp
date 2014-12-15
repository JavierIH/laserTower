#include "../libraries/Turret/turret.hpp"
#include "../libraries/FaceDetector/facedetector.hpp"
#include "../libraries/debug/debug.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

#include <yarp/os/all.h>
#include <opencv2/opencv.hpp>

void printUsage()
{
    std::cout << std::endl <<  "Usage: yes_we_can [can ids]" << std::endl << std::endl;
    std::cout << "Where: [can ids] - Space-separated can ids, for example: 1 2 0" << std::endl << std::endl;
}

int main(int argc, char ** argv)
{

    /**********************************************************************
     * Setup
     * ********************************************************************
     * This block sets up everything needed for the program
     *
     * *******************************************************************/

    //-- Extract can ids from the command line
    //----------------------------------------------------------------------
    std::vector<int> can_ids;

    if (argc == 1)
    {
        printUsage();
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        can_ids.push_back(atoi(argv[i]));
        std::cout << "New target added: " << argv[i] << std::endl;
    }


    //-- Setup YARP
    //-----------------------------------------------------------------------
    //-- Start yarp server
//    yarp::os::Network::init();
//    yarp::os::Network::runNameServer(argc, argv);


    //-- Setup Turret
    //-----------------------------------------------------------------------
    Turret myTurret("/dev/ttyACM0"); //-- (Change port as desired)

    if (!myTurret.start())
    {
        std::cerr << "[Test] Not able to connect!" << std::endl;
        return 1;
    }

    std::cout << "Turret is now connected" << std::endl;


    //-- Create face detector
    //-----------------------------------------------------------------------
    FaceDetector faceDetector;
    std::cout << "Face detector ready. Safety measures enabled!" << std::endl;


    //-- Setup webcam
    //-----------------------------------------------------------------------
    cv::VideoCapture capture(1);

    if(!capture.isOpened())
    {
        report(ERROR,"Camera failed at opening");
        return 1;
    }


    //-- Setup P controller params
    //-----------------------------------------------------------------------
    float kpx = 0.01;
    float kpy = 0.01;



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
            int move_x = error_x * kpx;
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

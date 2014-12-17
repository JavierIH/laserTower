#include "../libraries/Turret/turret.hpp"
#include "../libraries/FaceDetector/facedetector.hpp"
#include "../libraries/debug/debug.h"
#include "../libraries/color_detector/color_detector.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>

#include <yarp/os/all.h>
#include <opencv2/opencv.hpp>





//-- Min error to stop tracking:
static const int THRESH_X = 20;
static const int THRESH_Y = 20;

void printUsage()
{
    std::cout << std::endl <<  "Usage: yes_we_can [options]" << std::endl << std::endl;
    std::cout << "Options: " << std::endl;
    std::cout << "\t--help (shows usage)" << std::endl;
    std::cout << "\t--port [serial port] (serial port to be used)" << std::endl;
    std::cout << "\t--camera [integer id] (camera to be used)" << std::endl;
    std::cout << "\t--cans [can id list] (sequence of cans to be shoot {as string}})" << std::endl << std::endl;

}

bool isInsideRect(cv::Point point, cv::Rect rectangle)
{
    if (point.x >= rectangle.x &&
        point.x <= rectangle.x + rectangle.width &&
        point.y >= rectangle.y &&
        point.y <= rectangle.y + rectangle.height)
        return true;
    else
        return false;
}

cv::Rect scaleRect(cv::Rect rectangle, double scale_factor)
{
    int center_x = rectangle.x + rectangle.width / 2;
    int center_y = rectangle.y + rectangle.height /2;

    return cv::Rect(center_x - scale_factor * rectangle.width / 2,
                    center_y - scale_factor * rectangle.height / 2,
                    scale_factor * rectangle.width,
                    scale_factor * rectangle.height);
}

int main(int argc, char ** argv)
{

    /**********************************************************************
     * Setup
     * ********************************************************************
     * This block sets up everything needed for the program
     *
     * *******************************************************************/

    //-- YARP Resource finder (used to extract command line arguments)
    //---------------------------------------------------------------------
    yarp::os::ResourceFinder resourceFinder;
    resourceFinder.setVerbose(false);
    resourceFinder.configure(argc, argv);

    //-- Show usage if requested:
    //----------------------------------------------------------------------
    if (resourceFinder.check("help"))
    {
        printUsage();
        return 1;
    }

    //-- Extract can ids from the command line
    //----------------------------------------------------------------------
    std::vector<int> can_ids;

    if (!resourceFinder.check("cans"))
    {
        std::cerr << "ERROR: no can sequence was specified!" << std::endl;
        printUsage();
        return 1;
    }

    std::string can_list =  resourceFinder.find("cans").asString();
    std::stringstream can_list_ss(can_list);

    while ( !can_list_ss.eof())
    {
        int temp;
        can_list_ss >> temp;
        can_ids.push_back(temp);
        std::cout << "New target added: " << temp << std::endl;
    }


    //-- Setup YARP
    //-----------------------------------------------------------------------
    //-- Start yarp server
//    yarp::os::Network::init();
//    yarp::os::Network::runNameServer(argc, argv);


    //-- Setup Turret
    //-----------------------------------------------------------------------
    //-- Read port from command line
    std::string serial_port_name = resourceFinder.find("port").asString();
    if (serial_port_name == "")
        serial_port_name = "/dev/ttyACM0";
    std::cout << "Connecting to serial port: " << serial_port_name << std::endl;

    //-- Start turret
    Turret myTurret(serial_port_name);

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
    int camera_id = -1;
    if (resourceFinder.check("camera"))
        camera_id = resourceFinder.find("camera").asInt();
    std::cout << "Opening camera: " << camera_id << std::endl;

    cv::VideoCapture capture(camera_id);

    if(!capture.isOpened())
    {
        report(ERROR,"Camera failed at opening");
        return 1;
    }


    //-- Setup P controller params
    //-----------------------------------------------------------------------
    float kpx = 0.01;
    float kpy = 0.01;


    /**********************************************************************
     * Program itself
     * ********************************************************************
     * This block is the program
     *
     * *******************************************************************/

    //-- For each target
    for (int i = 0; i < can_ids.size(); i++)
    {
        //-- Loop to track targets
        //---------------------------------------------------------------------------------------
        int error_x = 600, error_y = 600;
        bool target_found;
        do
        {
            target_found = false;

            //-- Get image from webcam
            cv::Mat frame;
            capture.read(frame);


            /*
             * ***** HERE IS WHERE TARGET IS EXTRACTED
             */
            std::vector<cv::Rect> target_bboxes = getTarget(can_ids[i], frame);

            if (target_bboxes.size() > 0)
            {
                target_found = true;

                //-- Get center:
                cv::Rect target_box = target_bboxes[0];
                cv::Rect scaled_target_box = scaleRect(target_box, 0.80);

                cv::Point target;
                target.x=target_box.x + target_box.width/2;
                target.y=target_box.y + target_box.height/2;


                //-- Calculate error
                error_x = frame.cols / 2 - target.x;
                error_y = frame.rows / 2 - target.y;

                std::cout << "--------------DEBUG--------------------------------" << std::endl;
                std::cout << "Error x: " << error_x << std::endl;
                std::cout << "Error y: " << error_y << std::endl;

                //-- P controller
                int move_x = - error_x * kpx;
                int move_y = - error_y * kpy;

                //-- Command motors
                myTurret.movePanInc(move_x);
                myTurret.moveTiltInc(move_y);

                //-- Plotting target to have feedback
                cv::circle(frame, target, 3, cv::Scalar(0, 255, 0), 2);
                cv::rectangle(frame, scaled_target_box, cv::Scalar(0, 0, 255));

                //-- Finish if we get to the target
                if ( isInsideRect(target, scaled_target_box))
                    break;

            }
            //-- This is the scope (substitute it by another thing if needed)
            cv::circle(frame, cv::Point(frame.cols / 2, frame.rows / 2 ), 2, cv::Scalar(255, 0, 0), 2);

            //-- Show on screen things
            cv::imshow("out", frame);
            char key = cv::waitKey(30);
            if ( key == 27 || key == 'q' )
                return 0;

        } while ( abs(error_x) > THRESH_X && abs(error_y) > THRESH_Y );


        //-- Safety loop: (Extract faces)
        //-------------------------------------------------------------------------------------------
        std::vector<cv::Rect> faces;
        do
        {
            //-- Get image from webcam
            cv::Mat frame;
            capture.read(frame);

            faces = faceDetector.detect(frame);

            if (faces.size() == 0)
                break;

            std::cout << "Face detected!! Waiting to shoot..." << std::endl;

            //-- calculate center of bounding box
            int center_x = faces[0].x + faces[0].width / 2;
            int center_y = faces[0].y + faces[0].height / 2;


            //-- Plotting
            cv::rectangle(frame, faces[0], cv::Scalar(0, 0, 255));
            cv::circle(frame, cv::Point(center_x, center_y ), 2, cv::Scalar(0, 0, 255), 2);


            cv::imshow("out", frame);
            char key = cv::waitKey(30);
            if ( key == 27 || key == 'q' )
                return 0;

        } while (faces.size() > 0 );


        //-- All is clear: shoot!!
        //---------------------------------------------------------------------------------------------
        myTurret.shoot();
        std::cout << "Target \"" << can_ids.at(i) << "\" was shot down!" << std::endl;

    }

    yarp::os::Time::delay(3);
    myTurret.destroy();
    yarp::os::Network::fini();

    return 0;
}

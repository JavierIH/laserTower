#include "facedetector.hpp"

const std::string FaceDetector::haarcascade_frontal_file = "../../share/haarcascade_frontalface_default.xml";
const double FaceDetector::RESIZE_FACTOR_X = 1.25;
const double FaceDetector::RESIZE_FACTOR_Y = 1.25;

FaceDetector::FaceDetector()
{
   initCascadeClassifier();
}

std::vector<cv::Rect> FaceDetector::detect(cv::Mat src)
{
    //-- Create variables to store faces
     std::vector< cv::Rect > faces;
     std::vector< cv::Rect > resized_faces;

     //-- Convert the source image to a greyscale image:
     cv::Mat srcGrey;
     cv::cvtColor( src, srcGrey, CV_BGR2GRAY );

     cv::Mat srcGreySmall;
     cv::resize( srcGrey, srcGreySmall, cv::Size(0, 0), 0.25, 0.25 );

     //-- Detect face:
     faceDetector.detectMultiScale( srcGreySmall, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size( 30, 30) );

     //-- Show detected faces:
     if ( ! faces.empty() )
     {
         for (int i = 0; i < faces.size(); i++)
         {
             cv::Rect resizedRect;

             if ( RESIZE_FACTOR_X == 1 && RESIZE_FACTOR_Y == 1 )
             {
                 //-- Do not resize:
                 resizedRect = cv::Rect( faces[i].x * 4,
                             faces[i].y * 4,
                             faces[i].width  * 4,
                             faces[i].height * 4 );
             }
             else
             {
                 //-- Resize:
                 //-- Calculate original center:
                 double cx = faces[i].x * 4 + faces[i].width * 2;
                 double cy = faces[i].y * 4 + faces[i].height * 2;

                 //-- Calculate new size:
                 double newW = faces[i].width * 4 * RESIZE_FACTOR_X;
                 double newH = faces[i].width * 4 * RESIZE_FACTOR_Y;

                 //-- Calculate new center:
                 double newX = cx - newW / 2;
                 double newY = cy - newH / 2;

                 resizedRect = cv::Rect( newX, newY, newW, newH );
             }

             //-- Save rectancle
             resized_faces.push_back( resizedRect );
         }
     }

     return resized_faces;
}

void FaceDetector::initCascadeClassifier()
{
    //-- Load file with the classifier features:
    if ( ! faceDetector.load( haarcascade_frontal_file ) )
    {
        std::cerr << "[Error] Could not load cascade classifier features file." << std::endl;
    }

}

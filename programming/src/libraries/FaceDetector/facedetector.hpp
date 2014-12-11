#ifndef FACEDETECTOR_HPP
#define FACEDETECTOR_HPP

#include <string>
#include <vector>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"

class FaceDetector
{
    public:
        FaceDetector();

        std::vector<cv::Rect> detect(cv::Mat src);

    private:
        static const std::string haarcascade_frontal_file;

        //! -- \brief Cascade classifier to detect faces:
        cv::CascadeClassifier faceDetector;

        //! -- \brief Position of the last faces found:
        std::vector< cv::Rect > lastFacesPos;

        //! -- \brief Resize the rectangles.
        static const double RESIZE_FACTOR_X, RESIZE_FACTOR_Y;

        //! -- \brief Initializes the face detector
        void initCascadeClassifier();
};

#endif // FACEDETECTOR_HPP

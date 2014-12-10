#include "../libraries/debug/debug.h"
#include "../libraries/webcam/webcam.h"
#include "../libraries/color_detector/color_detector.h"

using namespace cv;

int main()
{

    Webcam camera(-1);

    while(1){
        Mat image=camera.getFrame();
        imshow("Original",image);

        Mat detection=detectColorRGB("blue",image);
        imshow("Color azul",detection);

        Mat thresh;
        threshold(detection,thresh,30,255,THRESH_BINARY);
        imshow("Threshold",thresh);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours( thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );

        for( int i = 0; i < contours.size(); i++ )
           { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
             boundRect[i] = boundingRect( Mat(contours_poly[i]) );
             minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
           }


        RNG rng(12345);

        /// Draw polygonal contour + bonding rects + circles
        Mat drawing = Mat::zeros( thresh.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
           {
             Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
             drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
             rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
             circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
           }

        imshow( "Contours", drawing );

        if (waitKey(30) == 27) break;
    }
}

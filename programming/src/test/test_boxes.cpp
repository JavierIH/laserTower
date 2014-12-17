
#include "../libraries/debug/debug.h"
#include "../libraries/webcam/webcam.h"
#include "../libraries/color_detector/color_detector.h"


using namespace cv;

void old_test(){
    Webcam camera(1);

    while(1){
        Mat image=camera.getFrame();
        imshow("Original",image);


        Mat detection=detectColorRGB("red",image);
        imshow("Color rojo",detection*2);

        Mat1b thresh;
        threshold(detection,thresh,100,255,THRESH_BINARY);

        thresh=dilation(thresh,-20);
        thresh=dilation(thresh,20);

        imshow("Threshold",thresh);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours( thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );

        for( int i = 0; i < contours.size(); i++ ){
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        }

        Mat drawing = Mat::zeros( thresh.size(), CV_8UC3 );

        int min_area=10000;
        int max_area=60000;

        //Threshold de areas
        if(contours.size()){
            for( int i = 0; i< contours.size(); i++ ){
                int area=boundRect[i].height*boundRect[i].width;
                if(area<min_area||area>max_area){
                    boundRect.erase(boundRect.begin()+i);
                }
            }
            vector<Point> target;

            for( int i = 0; i< boundRect.size(); i++ ){
                Scalar color = Scalar(255, 0, 0);
                rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
                Point center(boundRect[i].x+boundRect[i].width/2,boundRect[i].y+boundRect[i].height/2);
                target.push_back(center);
            }
            report("\n\n\n\n\n\n\n\n\n");
            report(OK,"Numero de contornos: "+to_string(contours.size()));
            for( int i = 0; i< target.size(); i++ ) report(INFO,"Centro de contornos "+to_string(target[i]));

            imshow( "Contornos", drawing );
        }
        char key = waitKey(30);
        if ( key == 27 || key == 'q') break;
    }
}

int main()
{

    Webcam camera(1);

    while(1){
        Mat image=camera.getFrame();
        imshow("Original",image);
        vector<Rect> rects;
        rects=getTarget("red",image);
        report(INFO,"Objetivos: "+to_string(rects));
        if (waitKey(30) == 27) break;
    }
}



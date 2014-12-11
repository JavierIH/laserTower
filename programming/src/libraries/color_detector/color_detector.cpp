#include "color_detector.h"
#include "../debug/debug.h"

using namespace cv;

Mat extractChannel(Mat image, int channel){
    Mat out(image.size(),image.depth());
    int ch[] = { channel, 0 };
    mixChannels( &image, 1, &out, 1, ch, 1 );
    return out;
}

Mat detectColorRGB(string color,Mat image){
    Mat blue=extractChannel(image,0);
    Mat green=extractChannel(image,1);
    Mat red=extractChannel(image,2);

    if(color=="red") return((red-blue)+(red-green));
    else if(color=="green") return((green-red)+(green-blue));
    else if(color=="blue") return((blue-red)+(blue-green));
    else if(color=="yellow") return((red-blue)+(green-blue));
    else if(color=="cyan") return((green-red)+(blue-red));
    else if(color=="magenta") return((blue-green)+(red-green));
    else report(ERROR, "Bad color detection");

    return image;
}

cv::Mat1b dilation(Mat1b input, int level){
    Mat1b output;
    if(level>0)dilate(input, output, Mat(),Point(-1,-1),level);
    if(level<0)erode(input, output, Mat(),Point(-1,-1),-level);

    return output;
}

vector<Point> getTarget(Mat image, string color){
    report(ERROR,"ESTO AUN NO RULA!");

    Mat detection=detectColorRGB(color,image);

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
    vector<Point> target;
    if(!contours.empty()){
        for( int i = 0; i< contours.size(); i++ ){
            int area=boundRect[i].height*boundRect[i].width;
            if(area<min_area||area>max_area){
                boundRect.erase(boundRect.begin()+i);
            }
        }
        if(!boundRect.empty()){
            report(OK,"Se han encontrado latas asesinas!!!");
            for( int i = 0; i< boundRect.size(); i++ ){
                Scalar color = Scalar(255, 0, 0);
                rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
                Point center(boundRect[i].x+boundRect[i].width/2,boundRect[i].y+boundRect[i].height/2);
                target.push_back(center);
            }
        }
        report(OK,"Numero de contornos: "+to_string(contours.size()));
        for( int i = 0; i< target.size(); i++ ) report(INFO,"Centro de contornos "+to_string(target[i]));
    }
    else report(INFO,"NO se han encontrado objetivos");

    return target;
}

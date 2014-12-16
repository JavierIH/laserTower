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

vector<Point> getTarget(std::string color, Mat image){

    Mat detection=detectColorRGB(color,image);

    Mat1b thresh;
    threshold(detection,thresh,150,255,THRESH_BINARY);

    thresh=dilation(thresh,-20);
    thresh=dilation(thresh,20);

    //imshow("Threshold",thresh);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );

    for( int i = 0; i < contours.size(); i++ ){
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    }

    //Mat drawing = Mat::zeros( thresh.size(), CV_8UC3 );

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
                Point center(boundRect[i].x+boundRect[i].width/2,boundRect[i].y+boundRect[i].height/2);
                target.push_back(center);
            }
        }
    }
    else report(INFO,"NO se han encontrado objetivos");

    return target;
}


vector<Point> getTarget(int color, Mat image){

    Mat detection=detectColorRGB(color,image);

    Mat1b thresh;
    threshold(detection,thresh,150,255,THRESH_BINARY);

    thresh=dilation(thresh,-20);
    thresh=dilation(thresh,20);

    //imshow("Threshold",thresh);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );

    for( int i = 0; i < contours.size(); i++ ){
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    }

    //Mat drawing = Mat::zeros( thresh.size(), CV_8UC3 );

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
                Point center(boundRect[i].x+boundRect[i].width/2,boundRect[i].y+boundRect[i].height/2);
                target.push_back(center);
            }
        }
    }
    else report(INFO,"NO se han encontrado objetivos");

    return target;
}


Mat detectColorRGB(int color, Mat image)
{
    Mat blue=extractChannel(image,0);
    Mat green=extractChannel(image,1);
    Mat red=extractChannel(image,2);

    if(color==COLOR_RED) return((red-blue)+(red-green));
    else if(color==COLOR_GREEN) return((green-red)+(green-blue));
    else if(color==COLOR_BLUE) return((blue-red)+(blue-green));
    else if(color==COLOR_YELLOW) return((red-blue)+(green-blue));
    else if(color==COLOR_CYAN) return((green-red)+(blue-red));
    else if(color==COLOR_MAGENTA) return((blue-green)+(red-green));
    else report(ERROR, "Bad color detection");

    return image;
}
Mat detectColorHSV(string color, Mat image ){


    Mat HSV;
    cvtColor(image,HSV,COLOR_BGR2HSV);
    Mat imgThresholded;
    int LowH=0;
    int HighH=0;
    const int LowS=35;
    const int LowV=35;
    const int HighS=255;
    const int HighV=255;

    if(color == "red"){
        Mat thres1;
        Mat thres2;
        inRange(HSV, Scalar(0, LowS, LowV), Scalar(15, HighS, HighV), thres1);
        inRange(HSV, Scalar(150, LowS, LowV), Scalar(179, HighS, HighV),thres2); //Threshold the image
        imgThresholded=thres1+thres2;
    }else{
        if(color=="green") {LowH=30; HighH=60;}
        else if(color=="blue") {LowH=90; HighH=120;}
        else if(color=="yellow") {LowH=25; HighH=45;}
        else report(ERROR, "Bad color detection");

        inRange(HSV, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), imgThresholded); //Threshold the image
    }
    return imgThresholded;

}

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
    else if(color=="magente") return((blue-green)+(red-green));
    else report(ERROR, "Bad color detection");

    return image;
}

cv::Mat1b dilation(Mat1b input, int level){
    Mat1b output;
    if(level>0)dilate(input, output, Mat(),Point(-1,-1),level);
    if(level<0)erode(input, output, Mat(),Point(-1,-1),-level);

    return output;
}

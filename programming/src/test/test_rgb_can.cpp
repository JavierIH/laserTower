#include "../libraries/debug/debug.h"
#include "../libraries/webcam/webcam.h"
#include "../libraries/color_detector/color_detector.h"
#include <cv.h>
#include <highgui.h>
#include <math.h>

using namespace cv;

int main()
{

    Webcam camera(-1);

    while(1){

        Mat image=camera.getFrame();

        imshow("Original",image);
        imshow("Color azul",detectColor("blue",image));
        if (waitKey(30) == 27) break;
    }
}

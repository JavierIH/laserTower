#include "../libraries/debug/debug.h"
#include "../libraries/webcam/webcam.h"
#include <cv.h>
#include <highgui.h>
#include <math.h>

using namespace cv;

int main()
{
    report(OK,"Testing the webcam");
    Webcam camera(0); // means /dev/video0

    Mat image=camera.getFrame();

    if(image.empty()){
        report(ERROR,"Something bad has happened");
        return 0;
    }

    imshow("Your image",image);
    waitKey();

    report(OK,"Bye!");
    return 1;
}

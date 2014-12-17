#include "../libraries/debug/debug.h"
#include "../libraries/webcam/webcam.h"
#include "../libraries/color_detector/color_detector.h"

using namespace cv;

int main()
{

   Webcam camera(-1);

   /*Mat image = imread("/home/roberto/laserTower/programming/src/test/fotos/arcoiris.png");
    if(! image.data )                              // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return 1;
        }
    */
    while(1){
        Mat image=camera.getFrame();

        imshow("Original",image);
        imshow("Color azul HSV",detectColorHSV("blue",image));
        imshow("Color azul RGB",detectColorRGB("blue",image));

        if (waitKey(30) == 27) break;
    }
}

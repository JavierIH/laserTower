
#include "../libraries/Turret/turret.hpp"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>


int main()
{
    Turret myTurret("/dev/ttyACM0");

    if (!myTurret.start())
    {
        std::cerr << "Not able to connect!" << std::endl;
        return 1;
    }

    printf("Connected\n");
    fflush(0);
    usleep(1e6);

    myTurret.testLED();
    usleep(1e6);
    //myTurret.movePanAbs(25);
    myTurret.shoot();
    printf("Toggle LED\n");
    fflush(0);
    usleep(1e6);

    printf("Homing turret\n");
    fflush(0);
    myTurret.movePanAbs(45);
    usleep(1e6);
}


#include "../libraries/Turret/turret.hpp"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

//int main()
//{
//    Turret myTurret("/dev/ttyACM0");

//    if (!myTurret.start())
//    {
//        std::cerr << "Not able to connect!" << std::endl;
//        return 1;
//    }

//    system("sleep 10");
//    printf("Connected\n");

//    printf("Turning on LED\n");
//    myTurret.LED();
//    usleep(1000000);

//    printf("Homing\n");
//    myTurret.movePanAbs(45);
//    myTurret.moveTiltAbs(45);
//    usleep(1e6);
//    myTurret.movePanAbs(90);
//    myTurret.moveTiltAbs(90);
//    usleep(1e6);

//    for (int i = 0; i < 180; i++)
//    {
//        myTurret.movePanAbs(90-i);
//        myTurret.moveTiltAbs(90-i);
//        usleep(5e5);
//    }
//    myTurret.destroy();
//}

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

    myTurret.LED();
    usleep(1e6);
    //myTurret.movePanAbs(25);
    myTurret.shoot();
    printf("Toggle LED\n");
    fflush(0);
    usleep(1e6);

    printf("Homing turret\n");
    fflush(0);
    usleep(1e6);
}

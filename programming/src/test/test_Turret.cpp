
#include "../libraries/Turret/turret.hpp"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include <yarp/os/all.h>

int main(int argc, char ** argv)
{
    //-- Start yarp server
//    yarp::os::Network::init();
//    yarp::os::Network::runNameServer(argc, argv);

    Turret myTurret("/dev/ttyACM0");

    if (!myTurret.start())
    {
        std::cerr << "[Test Turret] Not able to connect!" << std::endl;
        return 1;
    }

    printf("[Test Turret] Connected\n");
    fflush(0);
    yarp::os::Time::delay(2);

    myTurret.testLED();
    yarp::os::Time::delay(2);
    myTurret.movePanAbs(25);
    myTurret.moveTiltAbs(25);
    myTurret.shoot();
    printf("[Test Turret] Toggle LED\n");
    fflush(0);
    yarp::os::Time::delay(2);

    printf("[Test Turret] Homing turret\n");
    fflush(0);
    myTurret.movePanAbs(45);
    yarp::os::Time::delay(2);

    myTurret.destroy();
    yarp::os::Network::fini();
}

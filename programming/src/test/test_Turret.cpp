
#include "../libraries/Turret/turret.hpp"
#include <iostream>
#include <unistd.h>

int main()
{
    Turret myTurret("/dev/ttyACM0");

    if (!myTurret.start())
    {
        std::cerr << "Not able to connect!" << std::endl;
        return 1;
    }

    usleep(1e6);
    std::cout << "Connected" << std::endl;

    std::cout << "Turning on LED" << std::endl;
    myTurret.LED();
    usleep(1e6);

    std::cout << "Homing" << std::endl;
    myTurret.movePanAbs(45);
    myTurret.moveTiltAbs(45);
    usleep(1e6);
    myTurret.movePanAbs(90);
    myTurret.moveTiltAbs(90);
    usleep(1e6);

    for (int i = 0; i < 180; i++)
    {
        myTurret.movePanAbs(90-i);
        myTurret.moveTiltAbs(90-i);
        usleep(5e5);
    }
    myTurret.destroy();
}

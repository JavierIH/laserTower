#ifndef TURRET_HPP
#define TURRET_HPP

#include <vector>
#include <iostream>
#include <string>

#include <RdAudioManager.hpp>

class Turret
{
    public:
        Turret(const std::string serial_port);

        void moveTiltAbs(float angle_absolute);
        void movePanAbs(float angle_absolute);

        void moveTiltInc(float angle_increment);
        void movePanInc(float angle_increment);

        bool shoot();
        bool testLED();

        bool start();
        bool stop();
        bool destroy();
        void reset();


   private:
       float angle_tilt, angle_pan;
       bool connected;

       static const int LIMIT_TILT_MAX;
       static const int LIMIT_TILT_MIN;
       static const int LIMIT_PAN_MAX;
       static const int LIMIT_PAN_MIN;


       std::string serial_port_name;


       //-- Audio-related functions
       rd::RdAudioManager * audioManager;

};

#endif // TURRET_HPP

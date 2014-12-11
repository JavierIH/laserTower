#ifndef TURRET_HPP
#define TURRET_HPP

#include <vector>
#include <iostream>
#include <string>
#include <SerialStream.h>

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
        bool LED();

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

       //-- Serial port related functions:
       //--------------------------------------------------------------------------------------------------
       //! \brief Configures and opens the serial port
       bool initSerialPort();
       //! \brief Checks that the modular robot is connected by checking the initial message from the robot
       bool checkConnection();
       //! \brief Sends the commands required for toggling the user LED on the robot board
       bool toggleLED();
       //! \brief Sends the commands required for setting the joint position values on the modular robots
       bool sendJointValuesSerial(std::vector<float> joint_values);


       SerialPort* serialPort;
       std::string serial_port_name;


       //-- Audio-related functions
       rd::RdAudioManager * audioManager;

};

#endif // TURRET_HPP

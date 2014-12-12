#include "turret.hpp"

const int Turret::LIMIT_TILT_MAX = 90;
const int Turret::LIMIT_TILT_MIN = -90;
const int Turret::LIMIT_PAN_MAX = 90;
const int Turret::LIMIT_PAN_MIN = -90;

Turret::Turret(const std::string serial_port)
{
    angle_tilt = 0;
    angle_pan = 0;
    connected = false;
    serial_port_name = serial_port;

    //-- Configure AudioManager
    audioManager = rd::RdAudioManager::getAudioManager();
    audioManager->load("../../share/01_milshot.wav", "shoot", rd::RdAudioManager::FX);
}

void Turret::moveTiltAbs(float angle_absolute)
{
    if (connected)
    {
        if (angle_absolute <= LIMIT_TILT_MAX && angle_absolute >= LIMIT_TILT_MIN )
        {
            angle_tilt = angle_absolute;
            std::vector<float> joints;
            joints.push_back(angle_tilt);
            joints.push_back(angle_pan);
            //-- Send joint values
        }
        else
        {
            std::cerr << "[Turret] Angle " << angle_absolute << " is out of limits!" << std::endl;
        }
    }
    else
    {
        std::cerr << "[Turret] Not connected yet!" << std::endl;
    }

}

void Turret::movePanAbs(float angle_absolute)
{
    if (connected)
    {
        if (angle_absolute <= LIMIT_PAN_MAX && angle_absolute >= LIMIT_PAN_MIN )
        {
            angle_pan = angle_absolute;
            std::vector<float> joints;
            joints.push_back(angle_tilt);
            joints.push_back(angle_pan);
            //-- Send joint values
        }
        else
        {
            std::cerr << "[Turret] Angle " << angle_absolute << " is out of limits!" << std::endl;
        }
    }
    else
    {
        std::cerr << "[Turret] Not connected yet!" << std::endl;
    }

}

void Turret::moveTiltInc(float angle_increment)
{
    if (connected)
    {
        float new_angle = angle_tilt + angle_increment;
        if (new_angle <= LIMIT_TILT_MAX && new_angle >= LIMIT_TILT_MIN )
        {
            angle_tilt = new_angle;
            std::vector<float> joints;
            joints.push_back(angle_tilt);
            joints.push_back(angle_pan);
            //-- Send joint values
        }
        else
        {
            std::cerr << "[Turret] Angle " << new_angle << " is out of limits!" << std::endl;
        }
    }
    else
    {
        std::cerr << "[Turret] Not connected yet!" << std::endl;
    }
}

void Turret::movePanInc(float angle_increment)
{
    if (connected)
    {
        float new_angle = angle_pan + angle_increment;
        if (new_angle <= LIMIT_PAN_MAX && new_angle >= LIMIT_PAN_MIN )
        {
            angle_pan = new_angle;
            std::vector<float> joints;
            joints.push_back(angle_tilt);
            joints.push_back(angle_pan);
            //-- Send joint values
        }
        else
        {
            std::cerr << "[Turret] Angle " << new_angle << " is out of limits!" << std::endl;
        }
    }
    else
    {
        std::cerr << "[Turret] Not connected yet!" << std::endl;
    }
}

bool Turret::shoot()
{
    audioManager->playSound("shoot", 0);
    return true;
}

bool Turret::testLED()
{
    std::cerr << "[Turret] Not connected yet!" << std::endl;
    return false;
}

bool Turret::start()
{
    //-- Connect

    if (connected)
    {
        std::vector<float> joints;
        joints.push_back(angle_tilt);
        joints.push_back(angle_pan);
        //-- Send joint values
    }

    return connected;
}

bool Turret::stop()
{
    //-- Close serial port

    return true;
}

bool Turret::destroy()
{

    return true;
}


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
            double joints[] = {angle_tilt, angle_pan};
            position_controller->positionMove(joints);
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
            double joints[] = {angle_tilt, angle_pan};
            position_controller->positionMove(joints);
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
            double joints[] = {angle_tilt, angle_pan};
            position_controller->positionMove(joints);
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
            double joints[] = {angle_tilt, angle_pan};
            position_controller->positionMove(joints);
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
    if (!connected)
    {
        //-- Register custom plugin
        YARP_REGISTER_PLUGINS(RdYarp);

        //-- Start yarp network
        yarp::os::Network::init();

        //-- Check yarp server
        if (!yarp::os::Network::checkNetwork())
        {
            std::cerr << "Please start a yarp name server first" << std::endl;
            return false;
        }

        //-- Configure board driver
        yarp::os::Property options;
        options.put("device","RdSerialServoBoard");
        options.put("remote","/turret");
        options.put("local","/local");
        yarp::dev::PolyDriver dd(options);
        if(!dd.isValid())
        {
            std::cerr << "RdSerialServoBoard device not available" << std::endl;
            dd.close();
            yarp::os::Network::fini();
            return false;
        }

        //-- Extract Position Control

        bool ok = dd.view(position_controller);
        if (!ok)
        {
            std::cerr << "[warning] Problems acquiring robot interface" << std::endl;
            return false;
        }
        else
        {
            std::cerr << "[success] testAsibot acquired robot interface" << std::endl;
        }
        position_controller->setPositionMode();

        //-- Now it is connected
        connected = true;
    }

    if (connected)
    {
        double joints[] = {angle_tilt, angle_pan};
        position_controller->positionMove(joints);
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


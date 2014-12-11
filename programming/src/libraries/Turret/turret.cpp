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
    serialPort = NULL;

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
            sendJointValuesSerial(joints);
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
            sendJointValuesSerial(joints);
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
            sendJointValuesSerial(joints);
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
            sendJointValuesSerial(joints);
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

bool Turret::LED()
{
    if (connected)
        return toggleLED();
}

bool Turret::start()
{
    connected = initSerialPort();

    if (connected)
    {
        std::vector<float> joints;
        joints.push_back(angle_tilt);
        joints.push_back(angle_pan);
        sendJointValuesSerial(joints);
    }

    return connected;
}

bool Turret::stop()
{
    //-- Close serial port
    if ( serialPort && serialPort->IsOpen() )
    {
        serialPort->Close();
        connected = false;
    }

    return true;
}

bool Turret::destroy()
{
    //-- Close serial port
    if ( serialPort && serialPort->IsOpen() )
    {
        serialPort->Close();
        connected = false;
    }

    delete serialPort;
    serialPort = NULL;

    return true;
}

bool Turret::initSerialPort()
{
    serialPort = new SerialPort( serial_port_name );

    try
    {
        serialPort->Open( SerialPort::BAUD_57600, SerialPort::CHAR_SIZE_8,
                          SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1,
                          SerialPort::FLOW_CONTROL_NONE );
    }
    catch ( SerialPort::OpenFailed e )
    {
        std::cerr << "[Turret] Error opening the serial port" << serial_port_name << std::endl;
        return false;
    }

//    if ( ! checkConnection() )
//    {
//        std::cerr << "Error communicating with the robot. Exiting...\n" << serial_port_name << std::endl;
//        return false;
//    }

    return true;
}

bool Turret::checkConnection()
{
    //-- Read welcome message to check if connected to the robot
    SerialPort::DataBuffer buffer;

    try
    {
        serialPort->Read( buffer, 13, 1500);
    }
    catch ( SerialPort::ReadTimeout e)
    {
        std::cout << "Timeout! Exiting..." << std::endl;
        return false;
    }

    //-- Check if connected
    std::string welcomeMessage = "[Debug] Ok!\r\n";
    bool diffFlag = false;
    for (int i = 0; i < (int) buffer.size(); i++)
    {
        if ( welcomeMessage[i] != buffer[i] )
            diffFlag = true;
    }

    return !diffFlag;
}

bool Turret::toggleLED()
{
    if ( serialPort && serialPort->IsOpen() )
    {
        SerialPort::DataBuffer outputBuff;
        outputBuff.push_back(0x5F); //-- 0x5F -> Toggle LED
        serialPort->Write( outputBuff );

        return true;
    }
    else
    {
        std::cerr << "[Turret] Error: LED could not be toggled (no robot connected)"
                  << std::endl;
        return false;
    }
}

bool Turret::sendJointValuesSerial(std::vector<float> joint_values)
{
    if ( serialPort && serialPort->IsOpen() )
    {
        if ( joint_values.size() <= 8)
        {
            //-- Convert joint position to servo values [0-180]
            for (int i = 0; i < joint_values.size(); i++)
            {
                joint_values[i]+=90;
                if (joint_values[i] < 0) joint_values[i] = 0;
                if (joint_values[i] > 180) joint_values[i] = 180;
            }


            SerialPort::DataBuffer outputBuff;
            outputBuff.push_back(0x50); //-- 0x50 -> Set pos to all joints

            for (int i = 0; i < joint_values.size(); i++)
                outputBuff.push_back( (char)( (int) (joint_values[i])));

            try
            {
                serialPort->Write( outputBuff );
            }
            catch( std::runtime_error e)
            {
                std::cerr << "Exception ocurred: " << e.what() << std::endl;
                return false;
            }

            return true;
        }
        else
        {
            //-- Temporal solution to more than 8 modules configurations
            std::cerr << "[Turret] Error: more than 8 joints are not supported by "
                      << "the current architecture. Sending just 8 values..." << std::endl;

            SerialPort::DataBuffer outputBuff;
            outputBuff.push_back(0x50); //-- 0x50 -> Set pos to all joints

            for (int i = 0; i < 8; i++)
                outputBuff.push_back( (char)( (int) (joint_values[i])));

            serialPort->Write( outputBuff );

            return false;
        }
    }
    else
    {
        std::cerr << "Robot could not send joints (because it is not connected)"
                  << std::endl;
        return false;
    }
}

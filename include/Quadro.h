/*
 * BBBQuadro.h
 *
 *  Created on: 6 Dec 2015
 *      Author: michaelbrookes
 */

#ifndef SRC_H_BBBQUADRO_H_
#define SRC_H_BBBQUADRO_H_

#include "BBBAnalogDevice.h"
#include "BBBLSM303DLHC.h"
#include "BBBPWMDevice.h"
#include "BBBL3GD20H.h"
#include "BBBConfiguration.h"
#include <cstdlib>
#include <cmath>
#include <pthread.h>
#include <iomanip>

class Quadro {

public:
    Quadro( );

    int AddAnalogDevices( );
    int AddI2CDevices( );
    int AddMotors( );

    void SetAllMotorsSpeed( int motor_speed );
    void SetAllMotorsPower( BBBPWMDevice::PWM_RunValues val );

    void SetMotorSpeed( int motor_index, int motor_speed );
    void SetMotorPower( int motor_index, BBBPWMDevice::PWM_RunValues val );

    void CheckSensorsForSense( );

    void SetDefaultTargetValuesBasedOnStaticAverages( );

    int AnalyseData( int DataArray[ MAX_DATASTORE ], int TargetValue, int AllowedErrorValue);

    BBBPWMDevice PropellorMotor[ PROPELLOR_COUNT ];
    BBBAnalogDevice AnalogSensor[ ANALOG_SENSOR_COUNT ];
    BBBLSM303DLHC Accelerometer;
    BBBLSM303DLHC Magnetometer;
    BBBL3GD20H Gyroscope;
    BBBConfiguration Config;

protected:
    void *status;

    int ConvertAnalogReadingToAltitude( int AReading);
};

#endif /* SRC_H_BBBQUADRO_H_ */
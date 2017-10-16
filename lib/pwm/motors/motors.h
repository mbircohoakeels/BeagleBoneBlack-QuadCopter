/*
Copyright (C) 2017 Michael Brookes

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BEAGLEBONE_MOTOR_MOTORS_H
#define BEAGLEBONE_MOTOR_MOTORS_H

#include "../pwmDevice.h"
#include <string>

namespace quadro {

    namespace pwm {

        /**
         * Base class for adding functionality to the program for pwm motors
         */
        class motors : public pwmDevice {

        public:
            /**
             * default constructor
             */
            motors() = default;

            /**
             * Constructor for initialising the motor with it's associated pinBlock and PWMPin
             *
             * @param pinBlocks _block
             * @param PWMPins _pin
             */
            motors( pinBlocks _block, PWMPins _pin );

            /**
             * Set the speed that you want the motor to achieve.
             *
             * @param _targetSpeed
             */
            void setTargetSpeed( long _targetSpeed );

            /**
             * Reverse the motors polarity using this method
             */
            void reversePolarity();

            /**
             * If the motor thread isn't running, this method will start the thread running which continuously attempts to maintain the
             * current motor speed as closely as possible to the target speed that has been set.
             *
             * @throws pwmSetupException
             */
            void start();

            /**
             * Run this method if you want to stop your motor from maintaining it's targets.
             * Please note, this will make the motor stop completely - if in mid air...well...catastrophe!
             */
            void stop();

            /**
             * Set the motors duty value
             *
             * @param _duty
             */
            void setDuty( long _duty );

            /**
             * Set the motors Period value here
             *
             * @param _period
             */
            void setPeriod( long _period );

            /**
             * Set the motors Run value here
             *
             * @param _run
             */
            void setRun( int _run );

            /**
             * Set the motors Polarity here.
             *
             * @param _polarity
             */
            void setPolarity( int _polarity );

            long currentDuty = 0; //!< Storage for the current Duty value
            long currentPeriod = 0; //!< Storage for the current Period value
            int currentRun = 0; //!< Storage for the current Run value
            int currentPolarity = 0; //!< Storage for the current Polarity value

        protected:


            /**
             * Set your motors max speed here, it will probably be the lowest of the numbers as lower means faster.
             *
             * @param _maxSpeed
             */
            void setMaxSpeed( long _maxSpeed );

            /**
             * Set your motors min speed here, it will probably be the highest of the numbers as higher means slower.
             *
             * @param _minSpeed
             */
            void setMinSpeed( long _minSpeed );

        private:

            /**
             * The available motor status settings
             */
            enum status {
                On = 1, //!< Status On
                Off = 0, //!< Status Off
                Error = 3 //!< Status Error
            };

            /**
             * Polarity settings
             */
            enum polarity {
                Negative = 0, //!< Polarity is set to negative
                Positive = 1 //!< Polarity is set to positive
            };

            /**
             * Set the motor status as per the available status settings
             *
             * @param _status
             */
            void setStatus( status _status );

            /**
             * Increase the motor speed, if no speed step is specified this method will use the default value.
             *
             * @param _SpeedMod (defaults to 250)
             */
            void increaseSpeed( unsigned short _SpeedMod = 250 );

            /**
             * Decrease the motor speed, if no speed step is specified this method will use the default value.
             *
             * @param _SpeedMod (defaults to 250)
             */
            void decreaseSpeed( unsigned short _SpeedMod = 250 );

            /**
             * Main thread for each motor, its goal is to maintain the motors current speed in line with the set target speed.
             *
             * @return 0
             */
            static void* maintainTargetSpeed( void* );

            pthread_t threadHandle = 0; //!< Thread Handle
            int threadRet = 0; //!< Thread return value

            long targetSpeed = 0; //!< Storage for this motors target speed.
            long maxSpeed = 0; //!< Storage for this motors max speed value.
            long minSpeed = 0; //!< Storage for this motors min speed value.

            status Status = status::Off; //!< Storage for this motors status.
        };

    }

}

#endif //BEAGLEBONE_MOTOR_MOTORS_H

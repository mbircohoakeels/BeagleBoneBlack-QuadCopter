/*!
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

#ifndef IDEVICE_H
#define IDEVICE_H

#include <cstdio>

namespace quadro {
    /*!
     * Interface class for all devices
     */
    class IDevice {
    public:
        virtual ~IDevice() = default;

        /*!
        * pure virtual openDevice - All devices implementing this interface will require this functionality.
        */
        virtual void initDevice() = 0;

        /*!
        * pure virtual openDevice - All devices implementing this interface will require this functionality.
        */
        virtual int openDevice() = 0;

        /*!
        * pure virtual writeToDevice - All devices implementing this interface will require this functionality.
        */
        virtual int writeToDevice( size_t bufferSize_ ) = 0;

        /*!
        * pure virtual readDevice - All devices implementing this interface will require this functionality.
        */
        virtual short readDevice( size_t bufferSize_ ) = 0;

        /*!
        * pure virtual connectToDevice - All devices implementing this interface will require this functionality.
        */
        virtual int connectToDevice() = 0;

    protected:
    };
}
#endif //IDEVICE_H
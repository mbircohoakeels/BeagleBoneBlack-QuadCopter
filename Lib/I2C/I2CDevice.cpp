/*
Copyright (C) 2016 Michael Brookes

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

#include "./I2CDevice.h"

namespace abI2C {

    I2CDevice::I2CDevice( ) {
        this->DeviceAddress = 0x00;
        this->BusId = 0;
        this->DeviceInitialised = false;
    }

    I2CDevice::~I2CDevice( ) { close( this->FileHandle ); }

    void I2CDevice::InitDevice( ) throw( I2CSetupException& ) {
        if(!this->DeviceAddress) throw I2CSetupException( "I2C Device Not Configured ( try : 'obj->SetDeviceAddress([hex address])' )" );
        if(!this->BusId) throw I2CSetupException( "I2C Device Not Configured ( try : 'obj->SetBusId([bus number])' )" );
        /*
         * ** ## -- Setup Stage -- ## ** *
         * SetBusPaths : Saves the file paths to the available buses for ease of access.
         */
        this->SetBusPaths( );

        /*
         * ** ## -- Assignment Stage ( based on args ) -- ## ** *
         * ValidateBusId : Make sure we have a valid bus ID before proceeding.
         * SelectABusPath : Used to specify which bus your I2C device is on.
         * SetDeviceAddress: Hex value for your specific I2C Device.
         */
        this->ValidateBusId( );
        this->SelectABusPath( );

        /*
         * ** ## -- Init Stage -- ## ** *
         * OpenDevice : Creates a file handle for the device, should it be closed? Probably... :)
         * ConnectToDevice : Assigns the device as an I2C Slave and checks availability using IOCTL
         *
         * More info on IOCTL : http://man7.org/linux/man-pages/man2/ioctl.2.html
         */
        this->OpenDevice( );
        this->ConnectToDevice( );

        this->DeviceInitialised = true;
    }

    void I2CDevice::SetBusPaths( ) {
        this->_Bus[ 1 ].BusPath = this->ValidateBusPath( (char *)I2C_1 );
    }

    void I2CDevice::SelectABusPath( ) { this->DeviceBusPath = _Bus[ this->BusId ].BusPath; }

    void I2CDevice::SetRegisterAddress( unsigned char _RegisterAddress ) { this->RegisterAddress = _RegisterAddress; }

    void I2CDevice::SetRegisterValue( unsigned char _RegisterValue ){ this->RegisterValue = _RegisterValue; }

    const char * I2CDevice::GetFilePath( ) { return this->DeviceBusPath; }

    int I2CDevice::GetDeviceFileHandle( ) { return this->FileHandle; }

    int I2CDevice::ValidateBusId( ) throw( I2CSetupException& ) {
        if( this->BusId > I2C_BUS_COUNT || this->BusId < 1 ) {
            snprintf( this->ErrMessage, sizeof( this->ErrMessage ), "Bus ID : %d  is not a valid BUS for this device.", this->BusId );
            throw( I2CSetupException( this->ErrMessage ) );
        }
        else
            return EXIT_SUCCESS;
    }

    char * I2CDevice::ValidateBusPath( char * _I2CBusProposedPath ) throw( I2CSetupException& ) {
        if( stat ( _I2CBusProposedPath, &buffer) == 0 )
            return _I2CBusProposedPath;
        else{
            snprintf( this->ErrMessage, sizeof( this->ErrMessage ), "Fatal I2C Error - Unable to locate the I2C Bus file : %s", _I2CBusProposedPath );
            throw I2CSetupException( this->ErrMessage );
        }
    }

    short I2CDevice::GetValueFromRegister( unsigned char _RegisterAddress ) {
        if(!this->DeviceInitialised) throw I2CSetupException( "I2C Device Not Initialised ( try : 'obj->InitDevice()' )" );
        this->SetRegisterAddress( _RegisterAddress );
        this->WriteBufferOnly[ 0 ] = this->RegisterAddress;
        if( write( this->GetDeviceFileHandle( ), this->WriteBufferOnly, 1 ) == 1 ) {
            return this->ReadDevice( ONE_BYTE );
        }
        else {
            snprintf( this->ErrMessage, sizeof( this->ErrMessage ), "Fatal I2C Error - Unable to write to file : %s", this->GetFilePath( ));
            throw I2CSetupException( this->ErrMessage );
        }
    }

    short I2CDevice::ReadDevice( size_t _BufferSize ) throw( I2CSetupException& ) {
        if(!this->DeviceInitialised) throw I2CSetupException( "I2C Device Not Initialised ( try : 'obj->InitDevice()' )" );
        unsigned char buff[ _BufferSize ];
        if( read( this->GetDeviceFileHandle( ), buff, _BufferSize ) != _BufferSize ) {
            snprintf( this->ErrMessage, sizeof( this->ErrMessage ), "Fatal I2C Error - Unable to read from file : %s", this->GetFilePath( ) );
            throw I2CSetupException( this->ErrMessage );
        }
        else
            return buff[ 0 ];
    }

    int I2CDevice::OpenDevice( ) throw( I2CSetupException& ) {
        this->FileHandle = open( this->GetFilePath( ), O_RDWR );
        if( this->FileHandle == 0 ) {
            snprintf( this->ErrMessage, sizeof( this->ErrMessage ), "Fatal I2C Error - Unable to open file : %s", this->GetFilePath( ) );
            throw I2CSetupException( this->ErrMessage );
        }
        return this->FileHandle;
    }

    int I2CDevice::WriteToDevice( size_t _BufferSize  ) throw( I2CSetupException& ) {
        if(!this->DeviceInitialised) throw I2CSetupException( "I2C Device Not Initialised ( try : 'obj->InitDevice()' )" );
        try {
            if( _BufferSize > ONE_BYTE ) {
                this->ReadAndWriteBuffer[ 0 ] = this->RegisterAddress;
                this->ReadAndWriteBuffer[ 1 ] = this->RegisterValue;
                write( this->GetDeviceFileHandle( ), this->ReadAndWriteBuffer, _BufferSize );
            }
            else {
                this->WriteBufferOnly[ 0 ] = this->RegisterAddress;
                write( this->GetDeviceFileHandle( ), this->WriteBufferOnly, _BufferSize );
            }
        }
        catch( exception& e ) {
            snprintf( this->ErrMessage, sizeof( this->ErrMessage ), "Fatal I2C Error - Unable to write to file : %s", this->GetFilePath( ) );
            throw I2CSetupException( this->ErrMessage );
        }

        return EXIT_SUCCESS;
    }

}
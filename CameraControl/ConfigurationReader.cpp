/*
ConfigurationReader
File meant for: 
->Storing the Configuration of a device
->Loading Devices and Streams from a Configuration File
Reason this file exists:
->Redundancy
-->Insures ultiple paths of device connection
->More Informative Connection process
-->Allows for control of device/stream parameters
*/

#include <PvSampleUtils.h>
#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvStreamU3V.h>
#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>


PV_INIT_SIGNAL_HANDLER();

//FileName of the Config File
#define FILE_NAME ( "CameraConfig.pvxml" )

#define DEVICE_CONFIGURATION_TAG ( "DeviceConfiguration" )
#define STREAM_CONFIGURAITON_TAG ( "StreamConfiguration" )



//
//  Store device and stream configuration.
//  Does so with some sort of ConnectionID
//  ->IP
//	->MAV
//  ->etc.
//  Returns a boolean to indicate Success/Failure

bool StoreConfiguration( const PvString &aConnectionID )
{
    PvResult lResult;
    PvConfigurationWriter lWriter;

    // Connect to the GigE Vision or USB3 Vision device
    cout << "Connecting to device" << endl;
    PvDevice *lDevice = PvDevice::CreateAndConnect( aConnectionID, &lResult );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to connect to device" << endl;
        PvDevice::Free( lDevice );
        return false;
    }

    // Store  with a PvDevice.
    cout << "Store device configuration" << endl;
    lWriter.Store( lDevice, DEVICE_CONFIGURATION_TAG );

    // Create and open PvStream
    cout << "Store stream configuration" << endl;
    PvStream *lStream = PvStream::CreateAndOpen( aConnectionID, &lResult );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to open stream object from device" << endl;
        lDevice->Disconnect();
        PvDevice::Free( lDevice );
        return false;
    }

    // Store with a PvStream
    lWriter.Store( lStream, STREAM_CONFIGURAITON_TAG );

    // Store with a simple string.
    cout << "Store string information" << endl;
    lWriter.Store( TEST_STRING, STRING_INFORMATION_TAG );

    // Insert the path of your file here.
    cout << "Store string information" << endl;
    lWriter.Save( FILE_NAME );

    PvStream::Free( lStream );
    PvDevice::Free( lDevice );

    return true;
}

/*
Uses the Congiration file defined above to:
->Restore a connection to a device
->Return a pointer to that device
->Verify if the connection is succesful
->Returns NULL pointer in case of failure
*/

PvDevice *RestoreDevice()
{
    PvConfigurationReader lReader;
    
    // Load all the information into a reader.
    cout << "Load information and configuration" << endl;
    lReader.Load( FILE_NAME );

    PvDeviceGEV lDeviceGEV;
    PvDevice *lDevice = NULL;

    cout << "Restore configuration for a device with the configuration name" << endl;

    // Attempt restoring as a GEV device
    PvResult lResult = lReader.Restore( DEVICE_CONFIGURATION_TAG, &lDeviceGEV );
    if ( lResult.IsOK() )
    {
        // Success, keep PvDevice pointer to GEV device
        lDevice = &lDeviceGEV;
    }
    else
	{
		cout << "Could not restore camera from configuration" << endl;
		return NULL;		
	}

    cout << "Verify operation success" << endl;
    if ( !lDevice->IsConnected() )
    {
        return NULL;
    }

    return lDevice;
}


/*
Uses the Congiration file defined above to:
->Restore a stream
->Return a pointer to that stream
->Verify if opening the stream is succesful
->Returns NULL pointer in case of failure
*/

PvSstream *RestoreStream()
{
    PvConfigurationReader lReader;
    
    // Load all the information into a reader.
    cout << "Load information and configuration" << endl;
    lReader.Load( FILE_NAME );
    
    PvStreamGEV lStreamGEV;
    PvStream *lStream = NULL;

    cout << "Restore configuration for a stream with the configuration name" << endl;

    // Attempt restoring as a GEV Stream
    PvResult lResult = lReader.Restore( STREAM_CONFIGURAITON_TAG, &lStreamGEV );
    if ( lResult.IsOK() )
    {
        // Success, keep PvStream pointer to GEV Stream
        lStream = &lStreamGEV;
    }
    else
    {
    	cout << "Could not resotre stream from configuration" << endl;
	}

    cout << "Verify operation success" << endl;
    if ( !lStream->IsOpen() )
    {
        return NULL;
    }

    return lStream;
}




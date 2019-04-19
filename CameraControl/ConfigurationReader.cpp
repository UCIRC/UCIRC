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


//PV_INIT_SIGNAL_HANDLER();

//FileName of the Config File
#define FILE_1 ( "lib/CameraConfig1.pvxml" )
#define FILE_2 ( "lib/CameraConfig2.pvxml" )

#define DEVICE_CONFIGURATION_TAG ( "DeviceConfiguration" )
#define STREAM_CONFIGURAITON_TAG ( "StreamConfiguration" )
#define STRING_INFORMATION_TAG ( "StringInformation" )

//  Does so with some sort of ConnectionID
//  ->IP
//	->MAV
//  ->etc.
//  Returns a boolean to indicate Success/Failure

//bool camera controls which config file to save the device information to
//True is for the first camera, false for the second
void StoreConfiguration( PvDevice *aDevice, PvStream *aStream, bool camera )
{
    PvConfigurationWriter lWriter;

	//Store with a PvDevice
	lWriter.Store( lDevice, DEVICE_CONFIGURATION_TAG );
    // Store with a PvStream
    lWriter.Store( lStream, STREAM_CONFIGURAITON_TAG );
    cout << "Store string information" << endl;
    if( camera ){
		lWriter.Save( FILE_1 );
	}
	else {
		lWriter.Save( FILE_2 );
	}
}

/*
Uses the Congiration file defined above to:
->Restore a connection to a device
->Return a pointer to that device
->Verify if the connection is succesful
->Returns NULL pointer in case of failure
*/

PvDevice *RestoreDevice( bool camera )
{
    PvConfigurationReader lReader;
    
    // Load all the information into a reader.
    cout << "Load information and configuration" << endl;
  	if( camera ){
		lReader.Load( FILE_1 );
	}
	else {
		lReader.Load( FILE_2 ); 
	}

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

PvStream *RestoreStream( bool camera )
{
    PvConfigurationReader lReader;
    
    // Load all the information into a reader.
    cout << "Load information and configuration" << endl;
    
  	if( camera ){
		lReader.Load( FILE_1 );
	}
	else {
		lReader.Load( FILE_2 ); 
	}
    
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




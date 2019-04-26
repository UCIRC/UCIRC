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
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>
#include <PvResult.h>
#include <PvPropertyList.h>
#include <cstring>

//PV_INIT_SIGNAL_HANDLER();

//FileName of the Config File
#define CONFIG_FILE ( "lib/Configuration.pvxml" )
#define CAMERA1_CONFIGURATION_TAG ( "Camera1" )
#define STREAM1_CONFIGURAITON_TAG ( "Stream1" )
#define CAMERA2_CONFIGURATION_TAG ( "Camera2" )
#define STREAM2_CONFIGURATION_TAG ( "Stream2" )
#define GENERAL_CONFIGURATION_TAG ( "GeneralParams" )

//  Does so with some sort of ConnectionID
//  ->IP
//	->MAV
//  ->etc.
//  Returns a boolean to indicate Success/Failure

//bool camera controls which config file to save the device information to
//True is for the first camera, false for the second
//Returns 1 if succesful, 0 if fails to store config
int StoreConfiguration( PvDevice *aDevice, PvStream *aStream, bool camera )
{
    PvConfigurationWriter lWriter;
	PvResult lResult;
	PvString target_device;
	PvString target_stream;
	if ( camera ){
		string device_string = CAMERA1_CONFIGURATION_TAG;
		//For some reason it says the macro is outta scope
		//********BUG****************
		string stream_string = "Stream1";
		target_device = PvString( device_string.c_str() );
		target_stream = PvString( stream_string.c_str() );
		cout << "Saving Device and Stream to Camera1 and Stream1 properties" << endl;
	}

	else {
		string device_string = CAMERA2_CONFIGURATION_TAG;
		string stream_string = STREAM2_CONFIGURATION_TAG;
		target_device = PvString( device_string.c_str() );
		target_stream = PvString( stream_string.c_str() );
		cout << "Saving Device and Stream to Camera2 and Stream2 properties" << endl;
	}

	//Store with a PvDevice
	lResult = lWriter.Store( aDevice, target_device );
	if ( !lResult.IsOK() ){
		cout << "Failed to store Device configuration" << endl;
//		return 0;
	}
    // Store with a PvStream
    lResult = lWriter.Store( aStream, target_stream );
	if ( !lResult.IsOK() ){
		cout << "Failed to store Stream configuration" << endl;
		return 0;
	}
	
	lResult = lWriter.Save( CONFIG_FILE );
	if ( !lResult.IsOK () ){
		cout << "Failed to save configuration information to Configuration File" << endl;
		return 0;
	}

	return 1;
	
}

/*
Uses the Congiration file defined above to:
->Restore a connection to a device
->Verify if the connection is succesful
->Returns 1 on success, 0 on failure
*/

int RestoreConfiguration( PvDevice *aDevice, PvStream *aStream, bool camera )
{
	PvResult lResult;
    PvConfigurationReader lReader;
 	PvString target_device;
	PvString target_stream;
	if ( camera ){
		string device_string = CAMERA1_CONFIGURATION_TAG;
		string stream_string = "Stream1";
		target_device = PvString( device_string.c_str() );
		target_stream = PvString( stream_string.c_str() );
		cout << "Restoring Device from Camera1 and Stream1 properties" << endl;
	}

	else {
		string device_string = CAMERA2_CONFIGURATION_TAG;
		string stream_string = STREAM2_CONFIGURATION_TAG;
		const PvString target_device = PvString( device_string.c_str() );
		const PvString target_stream = PvString( stream_string.c_str() );
		cout << "Restoring Device from Camera2 and Stream2 properties" << endl;
	}
    
    cout << "Loading information and configuration" << endl;
	lResult = lReader.Load( CONFIG_FILE );
	if ( !lResult.IsOK() ){
		cout << "Could not open Config File" << endl;
		return 0;		
	}

	cout << "Restoring Device..." << endl;
    // Attempt restoring as a GEV device
    lResult = lReader.Restore( target_device, aDevice );
    if ( !lResult.IsOK() )
    {
        cout << "Failed to restore device" << endl; 
		return 0;
    }

    cout << "Verify operation success..." << endl;
    if ( !aDevice->IsConnected() )
    {
		cout << "Failed to connect to Device" << endl;
        return 0;
    }

	cout << "Connected to Device!" << endl; 

	cout << "Restoring Stream..." << endl;
	
    lResult = lReader.Restore( target_stream , aStream );
    if ( !lResult.IsOK() )
    {
        cout << "Failed to Restore Stream" << endl;
		return 0;
    }

    cout << "Verify operation success" << endl;
    if ( !aStream->IsOpen() )
    {
    	cout << "Failed to open Stream" << endl;
	    return 0;
    }
	
	cout << "Restored Stream!" << endl;

    return 1;
}

int RestoreGeneralParams( PvPropertyList *aList ){
    string param_string = GENERAL_CONFIGURATION_TAG;
	cout << "Looking for General Parameters at " << param_string.c_str() << endl;
    const PvString ParamList = PvString( param_string.c_str() );
	cout << "PvString: " << ParamList.GetAscii() << endl;
    PvConfigurationReader lReader;
	PvResult lResult;
	PvPropertyList lList;

	lResult = lReader.Load( CONFIG_FILE );
	if ( !lResult.IsOK() ){
		cout << "Could not open Configuration file" << endl;
		return -1;
	}
	cout << "Config FIle Loaded" << endl;
	
	lResult = lReader.Restore( ParamList, &lList );
    //If It cannot find the property it creates a new one and writes the defualts
    if ( !lResult.IsOK() ) // lResult.GetCodeString() == "NOT_FOUND" )
    {
		cout << "Could not find property list" << endl;
		return -1;

	}
	cout << "Config list restored" << endl;

	return 0;
	
}

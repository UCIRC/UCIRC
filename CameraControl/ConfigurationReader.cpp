/*
ConfigurationReader
File meant for: 
->Storing the Configuration of a device
->Loading Devices and Streams from a Configuration File
->Loading General Parameters
Reason this file exists:
->Redundancy
-->Insures ultiple paths of device connection
->More Informative Connection process
-->Allows for control of device/stream parameters
*/

#include "lib/ConfigurationReader.h"


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
		return -1;
	}
    // Store with a PvStream
    lResult = lWriter.Store( aStream, target_stream );
	if ( !lResult.IsOK() ){
		cout << "Failed to store Stream configuration" << endl;
		return -1;
	}
	
	lResult = lWriter.Save( CONFIG_FILE );
	if ( !lResult.IsOK () ){
		cout << "Failed to save configuration information to Configuration File" << endl;
		return -1;
	}

	return 0;
	
}

/*
Uses the Congiration file defined above to:
->Restore a connection to a device
->Verify if the connection is succesful
->Returns 1 on success, 0 on failure
*/

PvDevice *RestoreDevice( bool camera )
{
	PvResult lResult;
    PvConfigurationReader lReader;
 	PvString target_device;
	PvDeviceGEV aDeviceGEV;
	PvDevice *aDevice = NULL;
	
	if ( camera ){
		string device_string = CAMERA1_CONFIGURATION_TAG;
		target_device = PvString( device_string.c_str() );
		cout << "Restoring Device from Camera1 properties" << endl;
	}

	else {
		string device_string = CAMERA2_CONFIGURATION_TAG;
		target_device = PvString( device_string.c_str() );
		cout << "Restoring Device from Camera2 properties" << endl;
	}
    
    cout << "Loading information and configuration" << endl;
	lResult = lReader.Load( CONFIG_FILE );
	if ( !lResult.IsOK() ){
		cout << "Could not open Config File" << endl;
		return NULL;		
	}

	cout << "Restoring Device..." << endl;
    // Attempt restoring as a GEV device
    lResult = lReader.Restore( target_device, &aDeviceGEV );
    if ( !lResult.IsOK() )
    {
        cout << "Failed to restore device" << endl; 
		return NULL;
    }
	
	aDevice = &aDeviceGEV;

    cout << "Verify operation success..." << endl;
    if ( !aDevice->IsConnected() )
    {
		cout << "Failed to connect to Device" << endl;
        return NULL;
    }

	cout << "Connected to Device!" << endl; 

    return aDevice;
}


PvStream  *RestoreStream( bool camera )
{
	PvResult lResult;
	PvStream *aStream;
    PvConfigurationReader lReader;
	PvString target_stream;
	if ( camera ){
		//FIX THIS BUG
		string stream_string = "Stream1";
		target_stream = PvString( stream_string.c_str() );
		cout << "Restoring Stream Stream1 properties" << endl;
	}

	else {
		string stream_string = STREAM2_CONFIGURATION_TAG;
		target_stream = PvString( stream_string.c_str() );
		cout << "Restoring Device from Camera2 and Stream2 properties" << endl;
	}
    
    cout << "Loading information and configuration" << endl;
	lResult = lReader.Load( CONFIG_FILE );
	if ( !lResult.IsOK() ){
		cout << "Could not open Config File" << endl;
		return NULL;		
	}

	cout << "Restoring Stream..." << endl;
	
    lResult = lReader.Restore( target_stream , aStream );
    if ( !lResult.IsOK() )
    {
        cout << "Failed to Restore Stream" << endl;
		return NULL;
    }

    cout << "Verify operation success" << endl;
    if ( !aStream->IsOpen() )
    {
    	cout << "Failed to open Stream" << endl;
	    return NULL;
    }
	
	cout << "Restored Stream!" << endl;

    return aStream;
}

PvPropertyList *RestoreGeneralParams(){
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
		return NULL;
	}
	
	else if ( lResult.GetCodeString() == "CANNOT_OPEN_FILE" )
	{
		cout << "Could not open Configuration file" << endl;
		return NULL;
	}

	cout << "Config File Loaded" << endl;
	
	uint32_t index = lReader.GetPropertyListCount();
	
	lResult = lReader.Restore( ParamList  , &lList );

	cout << "PropertyList Restored" << endl;
    //If It cannot find the property it creates a new one and writes the defualts
    if ( lResult.GetCodeString().GetAscii()  == "NOT_FOUND" )
    {
		cout << "Could not find property list" << endl;
		return NULL;

	}

	else if ( !lResult.IsOK() )
	{
		cout << "Could not load property list" << endl;
		return NULL;
	}

	cout << "Config list restored" << endl;

	return &lList;
	
}

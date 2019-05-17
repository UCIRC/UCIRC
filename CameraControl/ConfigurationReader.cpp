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
	string target_file;
	PvString target_stream = STREAM_CONFIGURATION_TAG;
	PvString target_device = DEVICE_CONFIGURATION_TAG;

	if ( camera ){
		target_file = CAMERA_1_FILE;
	}

	else {
		target_file = CAMERA_2_FILE;
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

	
	int counter = 0;
    bool removed = false;
    cout << "Remove operation attempt " << counter << endl;
    while ( remove( target_file.c_str() ) != 0 )
    {
        if ( counter > 5 )
        {
            cout << "Failed to remove file" << endl;
            cout << "Saving operation Failed" << endl;
            return false;
        }
        counter ++;
        cout << "Remove operation attempt " << counter << endl;
    }

    cout << "Remove Operation Successful" << endl;
	
	lResult = lWriter.Save( PvString( target_file.c_str() ) );
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
 	PvString target_device = DEVICE_CONFIGURATION_TAG;
	PvDeviceGEV *aDeviceGEV = new PvDeviceGEV;
	PvDevice *aDevice = NULL;
	string target_file;
	
	if ( camera ){
		target_file = CAMERA_1_FILE;
	}

	else {
		target_file = CAMERA_2_FILE;
	}
    
    cout << "Loading information and configuration" << endl;
	lResult = lReader.Load( PvString( target_file.c_str() ) );
	if ( !lResult.IsOK() ){
		cout << "Could not open Config File" << endl;
		return NULL;		
	}

	cout << "Restoring Device..." << endl;
    // Attempt restoring as a GEV device
    lResult = lReader.Restore( target_device, aDeviceGEV );
    if ( !lResult.IsOK() )
    {
        cout << "Failed to restore device" << endl; 
		return NULL;
    }
	
	aDevice = aDeviceGEV;

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
	PvStreamGEV *aStreamGEV = new PvStreamGEV;
	PvStream *aStream;
    PvConfigurationReader lReader;
	string target_file;
	PvString target_stream = STREAM_CONFIGURATION_TAG;
	if ( camera ){
		target_file = CAMERA_1_FILE;	
	}

	else {
		target_file = CAMERA_2_FILE;	
	}
    
    cout << "Loading information and configuration" << endl;
	lResult = lReader.Load( PvString( target_file.c_str() ) );
	if ( !lResult.IsOK() ){
		cout << "Could not open Config File" << endl;
		return NULL;		
	}

	cout << "Restoring Stream..." << endl;
	
    lResult = lReader.Restore( target_stream , aStreamGEV );
    if ( !lResult.IsOK() )
    {
        cout << "Failed to Restore Stream" << endl;
		return NULL;
    }

	aStream = aStreamGEV;

    cout << "Verify operation success" << endl;
    if ( !aStream->IsOpen() )
    {
    	cout << "Failed to open Stream" << endl;
	    return NULL;
    }
	
	cout << "Restored Stream!" << endl;

    return aStream;
}

PvPropertyList  *RestoreGeneralParams(){
    string param_string = GENERAL_CONFIGURATION_TAG;
	cout << "Looking for General Parameters at " << param_string.c_str() << endl;
    const PvString ParamList = PvString( param_string.c_str() );
	//Test for fidelity
	cout << "PvString:                         " << ParamList.GetAscii() << endl;
    

	//Forward Declarations
	PvConfigurationReader lReader;
	PvResult lResult;
	PvPropertyList *lList = new PvPropertyList;

	//Check if file can be loaded
	lResult = lReader.Load( CONFIG_FILE );
	if ( lResult.GetCodeString() == "CANNOT_OPEN_FILE"  ){
		cout << "Could not open Configuration file" << endl;
		cout << lResult.GetCodeString().GetAscii() << endl << endl;
		return NULL;
	}
	
	else if ( !lResult.IsOK() )
	{
		cout << "Could not open Configuration file" << endl;
		cout << "SOME OTHER REASON" << endl << endl;
		return NULL;
	}

	cout << "Config File Loaded" << endl;
	
	uint32_t index = lReader.GetPropertyListCount();

	cout << index << " Property List(s)" << endl;
	
	lResult = lReader.Restore( ParamList  , lList );

    //If It cannot find the property it creates a new one and writes the defualts
    if ( lResult.GetCodeString().GetAscii()  == "NOT_FOUND" )
    {
		cout << "Could not find property list" << endl;
		cout << lResult.GetCodeString().GetAscii() << endl << endl;
		return NULL;
	}

	else if ( !lResult.IsOK() )
	{
		cout << "Could not load property list" << endl;
		cout << lResult.GetCodeString().GetAscii()  << endl << endl;
		return NULL;
	}

	cout << "Param list restored!" << endl << endl;
	return lList;
}

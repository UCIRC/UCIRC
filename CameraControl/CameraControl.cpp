#include "lib/CameraControl.h"

void PrintHelp();

int main(int aCount, const char **aArgs){
	
	if( ParseOptionFlag( aCount, aArgs, "--help" ) )
	{
		PrintHelp();
		exit( 0 );
	}


	PvResult lResult;
	PvPropertyList *GeneralParams;
	PvString lName_1;
	PvString lName_2;
	PvString Filename;
	string Directory;
	int64_t lImageCount;
	int64_t lBufferCount;

	//Retrive General Parameters
	cout << "Restoring General Parameters..." << endl;
	
	GeneralParams = RestoreGeneralParams();
	
	if( GeneralParams != NULL )
	{
		cout << "General Params Loaded" << endl;
		PvProperty *lProperty;
		lProperty = GeneralParams->GetProperty( PvString( "BufferCount" ) ); 
		lProperty->GetValue( lBufferCount );
		lProperty = GeneralParams->GetProperty( PvString( "ImageCount" ) );
		lProperty->GetValue( lBufferCount );
		lProperty = GeneralParams->GetProperty( PvString( "Name_1" ) );
		lName_1 = lProperty->GetValue();
		lProperty = GeneralParams->GetProperty( PvString( "Name_2" ) );
		lName_2 = lProperty->GetValue();
		lProperty = GeneralParams->GetProperty( PvString( "ImagePath" ) );
		Directory = lProperty->GetValue().GetAscii();
		cout << "Buffer Count: " << lBufferCount << endl;
		cout << "Image Count: " << lImageCount << endl;
		cout << "Name_1: " << lName_1.GetAscii() << endl;
		cout << "Name_2: " << lName_1.GetAscii() << endl;
	}

	else
	{
		cout << "Failed to read General Params" << endl;
		cout << "Using Hardcoded Values" << endl;
		lBufferCount = DEFAULT_BUFFER_COUNT;
		lName_1 = DEFAULT_NAME_1;
		lName_2 = DEFAULT_NAME_2;
		Directory = DEFAULT_IMAGE_PATH; 
		cout << "Buffer Count: " << lBufferCount << endl;
		cout << "Name_1: " << lName_1.GetAscii() << endl;
		cout << "Name_2: " << lName_1.GetAscii() << endl;
	}

	bool calibrating = false;

	Camera *Camera_1 = NewCamera( lName_1 );
	cout << "Camera_1 Created" << endl;
	Camera *Camera_2 = NewCamera( lName_2 );
	cout << "Camera_2 Created" << endl;

	if( SetupCamera( Camera_1, true, lBufferCount ) && SetupCamera( Camera_2, false, lBufferCount ) )
	{
		cout << "Cameras Set up!" << endl;
	}

	else
	{
		cout << "Set up failure" << endl;
		cout << "Cleaning up..." << endl;
		FreeCameraInfo(Camera_1);
		FreeCameraInfo(Camera_2);
		delete Camera_1;
		delete Camera_2;
		delete GeneralParams;
		cout << "Terminating program" << endl;	

		return 0;
	}

	string File;
	
	if( ParseOptionFlag( aCount, aArgs, "--cal1" ) )
	{
		calibrating = true;
		Directory = "Calibrations/";
		File = Directory;
		File += Camera_1->Name.GetAscii();
		
		AcquireImages( Camera_1, File, 1 );
	}

	if( ParseOptionFlag( aCount, aArgs, "--cal2" ) )
	{
		calibrating = true;
		Directory = "Calibrations/";
		File = Directory;
		File += Camera_2->Name.GetAscii();

		AcquireImages( Camera_2, File, 1 );  		
	}

/*
	char time_buf[21];
    time_t now;
    time(&now);
    strftime(time_buf, 21, "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    string file = Directory;
    std::ostringstream o;
    o << time_buf;
    file += aCamera->Name.GetAscii();
    file += o.str();
*/

	
	if( !calibrating )
	{
	
		char c;
		cout << "Press Ctrl + D to terminate" << endl;
		while(cin>>c)
		{	
			if(Camera_1->Device->IsConnected()) cout << "YES1" << endl;
			else cout << "NO1" << endl;
			if(Camera_2->Device->IsConnected()) cout << "YES2" << endl;
			else cout << "NO2" << endl;
		}
	//	AcquireImages( Camera_1, GeneralParams );
	}
	cout << "Cleaning up..." << endl;
	FreeCameraInfo(Camera_1);
	FreeCameraInfo(Camera_2);
	delete Camera_1;
	delete Camera_2;
	delete GeneralParams;
	cout << "Terminating program" << endl;	

	return 0;

}

void PrintHelp()
{
    cout << "This porgram serves two purposes." << endl;
	cout << "	1) The first is to collect images automatically" << endl;
	cout << "	   It does this by collecting images at set time intervals" << endl;
	cout << "	   No Tags required to use" << endl;
	cout << "	   ->time interval currently unspecified/image collection not set up" << endl;
	cout << "	   ->Current functionality is connecting until terminated" << endl;
	cout << "	2) The second is to collect calibration images" << endl;
	cout << " 	   Add the tag '--cal1' to collect from the first camera" << endl;
	cout << "	   Add the tag '--cal2' to collect from the second camera" << endl;
	cout << "	   Calibration photos are saved to the 'Calibrations' directory" << endl;
}


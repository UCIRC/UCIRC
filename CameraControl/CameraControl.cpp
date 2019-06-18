#include "lib/CameraControl.h"

int main(void){
	
	PvResult lResult;
	PvPropertyList *GeneralParams;
	PvString lName_1;
	PvString lName_2;
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
		lProperty = GeneralParams->GetProperty( PvString( "Name_1" ) );
		lName_1 = lProperty->GetValue();
		lProperty = GeneralParams->GetProperty( PvString( "Name_2" ) );
		lName_2 = lProperty->GetValue();
		cout << "Buffer Count: " << lBufferCount << endl;
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
		cout << "Buffer Count: " << lBufferCount << endl;
		cout << "Name_1: " << lName_1.GetAscii() << endl;
		cout << "Name_2: " << lName_1.GetAscii() << endl;
	}

	Camera *Camera_1 = NewCamera( lName_1 );
	cout << "Camera Created" << endl;

	
	if( SetupCamera( Camera_1, true, lBufferCount ) )
	{
	
		char c;
		cout << "Press Ctrl + D to terminate" << endl;
		while(cin>>c)
		{	
			if(Camera_1->Device->IsConnected()) cout << "YES" << endl;
			else cout << "NO" << endl;
		}
	//	AcquireImages( Camera_1, GeneralParams );
	}
	cout << "Cleaning up..." << endl;
	FreeCameraInfo(Camera_1);
	delete Camera_1;
	delete GeneralParams;
	cout << "Terminating program" << endl;	

	return 0;

}


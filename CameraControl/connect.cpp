#include "lib/connect.h"

//Returns pointer to PvDevice Object
PvDevice *connect( const PvString connection_point ){
	PvResult lResult;
	//Find Device via connection_point
	cout << "Connecting to Device at: " << connection_point.GetAscii()  << endl;
    PvDevice *lCamera;
	cout << "Creating Device" << endl;
    //Creat Device Object For camera, Connect to it
	lCamera = PvDevice::CreateAndConnect( connection_point, &lResult );
 	cout << "Created Device" << endl;
	//Check if successful
	if( !lResult.IsOK() || lCamera==NULL || !lCamera->IsConnected() )
    {
        cout << "Unable to connect to device" << endl;
        return NULL;
    }
    cout << "Connected to device" << endl;
    
	return lCamera;
}


//Returns -1 if Failed, 0 if Success
//DOES NOT FREE DEVICE
int disconnect( PvDevice *lCamera ){
    PvResult lResult;
	cout << "Disconnecting from device..." << endl;
    //Disconnect From Camera
	lResult = lCamera->Disconnect();
	//Check if Succesful
	if( !lResult.IsOK() )
	{
		cout << "Could Not Disconnect from device" << endl;
		return -1;
	}
	cout << "Disconnected From Device" << endl;
	return 0;
}

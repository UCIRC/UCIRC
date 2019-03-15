#include <PvSampleUtils.h>
#include <PvDevice.h>
#include <PvSystem.h>
#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>
#include <PvDeviceInfo.h>
#include <PvDeviceInfoGEV.h>
#include <PvDevice.h>
#include <PvResult.h>
#include <stdlib.h>


PvDevice *connect( const PvString MAC, const PvDeviceInfo *lCameraInfo ){
    cout << "Connecting To Device..." << endl;
	PvResult lResult;
	PvSystem lSystem;
	//Set TimeOut Limit on System
	//Good Practice, should not really be neccesarry 
    cout << "Setting Timeout" << endl;
	lSystem.SetDetectionTimeout( 200 );
	//Find Device via MAC
	cout << "Finding Device by MAC" << endl;
	cout << MAC.GetAscii() << endl;
    lResult = lSystem.FindDevice( MAC, &lCameraInfo );
    //Check if Found
	cout << "Checking Result" << endl;
	if( !lResult.IsOK() )
    {
        cout << "Unable to find device" << endl;
        return NULL;
    }
    cout << "Found Device" << endl;
    PvDevice *lCamera;
	cout << "Creating Device" << endl;
    //Creat Device Object For camera, Connect to it
	lCamera = PvDevice::CreateAndConnect( lCameraInfo, &lResult);
 	cout << "Created Device" << endl;
	//Check if successful
	cout << "Checking if Successful" << endl;
	if( !lResult.IsOK() || lCamera==NULL || !lCamera->IsConnected() )
    {
        cout << "Unable to connect to device" << endl;
        return NULL;
    }
    cout << "Connected to device" << endl;
    
	//This Bit might be unnecessary, Just checking Camera Info against the MAC we expect
//	const PvDeviceInfoGEV* lCameraInfoGEV = dynamic_cast<const PvDeviceInfoGEV*>( lCameraInfo );
 //   cout << "Device MAC: " << lCameraInfoGEV->GetMACAddress().GetAscii() << endl;
	//Free this 
//	free(lCameraInfoGEV);
	//Return pointer to the device
	return lCamera;
}

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

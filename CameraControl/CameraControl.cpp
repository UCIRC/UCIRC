#include "lib/connect.h"
#include "lib/stream.h"
#include <stdlib.h>
#include <PvSystem.h>
#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvStreamU3V.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvSystemEnums.h>
#include <PvSampleUtils.h>

int main(){
	PvSystem lSystem;
//	if(lSystem == NULL){ cout << "Malloc Failure" << endl; return -1; }
	const PvDeviceInfo *lCameraInfo = (const PvDeviceInfo *) malloc(sizeof(PvDeviceInfo));
	if(lCameraInfo == NULL){ cout << "Malloc Failure" << endl; return -1; }
	const PvString MAC = "00:11:1c:03:09:12";
	PvResult lResult;
	
	//Check if Device can be connected to
	lResult = lSystem.FindDevice( MAC, &lCameraInfo );
	if( !lResult.IsOK() ){ cout << "Could not find device at " << MAC.GetAscii() << endl; return -1; }

	
	//Connect to Camera
	PvDevice *lCamera;
	lCamera = connect( MAC );

	PvStream *lStream;
	lStream = open_stream( lCameraInfo );
	
	int status;
	if(lStream != NULL)
	{
		//Close the stream
		status = close_stream( lStream );
		PvStream::Free( lStream );
	}	

	if(lCamera != NULL)
	{
		//Disconnect from Camera
		status = disconnect( lCamera );
		//VERY IMPORTANT: Free Camera Object
		PvDevice::Free( lCamera );	
	}

	else status = -1;
	
//	free(&lSystem);

	return status;


}

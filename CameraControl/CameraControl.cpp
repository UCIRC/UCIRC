#include "lib/connect.h"
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
	PvSystem *lSystem = (PvSystem *) malloc(sizeof(PvSystem));
	if(lSystem == NULL){ cout << "Malloc Failure" << endl; return -1; }
	const PvString MAC = "00:11:1c:03:09:12";
	const PvDeviceInfo *lCameraInfo = (const PvDeviceInfo *) malloc(sizeof(const PvDeviceInfo));
	if(lCameraInfo == NULL) { cout << "Malloc Failure" << endl; return -1; }

	
	//Connect to Camera
	PvDevice *lCamera;
	lCamera = connect( MAC, lCameraInfo );

	int status = 0;
	if(lCamera != NULL)
	{
		//Disconnect from Camera
		status = disconnect( lCamera );
		//VERY IMPORTANT: Free Camera Object
		PvDevice::Free( lCamera );	
	}

	else status = -1;
	
	free(lSystem); free( (PvDeviceInfo *) lCameraInfo );

	return status;


}

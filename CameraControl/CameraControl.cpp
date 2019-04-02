#include "lib/connect.h"
#include "lib/stream.h"
#include "lib/ImageAcquisition.h"
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

void clean_up( const PvDeviceInfo *lCameraInfo,
			   PvDevice *lCamera,
			   PvStream *lStream )
{
	cout << "Freeing Camera Info" << endl;
	delete lCameraInfo;
	
	if( lCamera != NULL )
	{
		while( disconnect( lCamera ) != -1 );	
		cout << "Freeing Camera" << endl;
 		PvDevice::Free( lCamera ); 
	}
	
	if( lStream != NULL )
	{
		while( close_stream( lStream ) != -1 );
		cout << "Freeing Stream" << endl;
		PvStream::Free( lStream );
	}

	return; 
}

int main(){
	PvSystem lSystem;
	const PvDeviceInfo *lCameraInfo;// = new(nothrow) (PvDeviceInfo);
//	if(lCameraInfo == NULL){ cout << "Allocation Failure" << endl; return -1; }
	const PvString MAC = "00:11:1c:03:09:12";
	PvResult lResult;
	PvDevice *lCamera = NULL;
	PvStream *lStream = NULL;
	PvPipeline *lPipeline = NULL;

	//Check if Device can be connected to
	lResult = lSystem.FindDevice( MAC, &lCameraInfo );
	if( !lResult.IsOK() ){ cout << "Could not find device at " << MAC.GetAscii() << endl; return -1; }

	
	//Connect to Camera
	lCamera = connect( MAC );
	if( lCamera == NULL )
	{
		cout << "Cleaning up..." << endl;
		clean_up(lCameraInfo, lCamera, lStream); 
		cout << "Terminating program" << endl;
		return -1; 
	}

	lStream = open_stream( lCameraInfo );
	if( lStream == NULL )
	{ 
		cout << "Cleaning up..." << endl;
		clean_up(lCameraInfo, lCamera, lStream);
		cout << "Terminating program" << endl;	
		return -1;
	}	

	ConfigureStream( lCamera, lStream );
	lPipeline = CreatePipeline( lCamera, lStream );
	if( lPipeline )
	{
		AcquireImages( lCamera, lStream, lPipeline );
		delete lPipeline;
	}
	
	cout << "Cleaning up..." << endl;
	clean_up(lCameraInfo, lCamera, lStream);
	cout << "Terminating program" << endl;	

	return 0;
}

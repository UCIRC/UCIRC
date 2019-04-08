#include "lib/connect.h"
#include "lib/stream.h"
#include "lib/ImageAcquisition.h"
#include "lib/ConfigurationReader.cpp"
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

#define BUFFER_COUNT ( 6 )





void clean_up( const PvDeviceInfo *lCameraInfo,
			   PvDevice *lCamera,
			   PvStream *lStream,
			   PvPipeline *lPipeline );
PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream );


int main(){
	PvSystem lSystem;
	const PvDeviceInfo *lCameraInfo;
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
	if( lCamera != NULL )
	{
		lStream = open_stream( lCameraInfo );
		if( lStream != NULL )
		{	 
			ConfigureStream( lCamera, lStream );
			lPipeline = CreatePipeline( lCamera, lStream );
			if( lPipeline != NULL )
			{
//				AcquireImages( lCamera, lStream, lPipeline );
				delete lPipeline;
			}
		}
	}
	cout << "Cleaning up..." << endl;
	clean_up(lCameraInfo, lCamera, lStream, lPipeline);
	cout << "Terminating program" << endl;	

	return 0;
}



void clean_up( const PvDeviceInfo *lCameraInfo,
			   PvDevice *lCamera,
			   PvStream *lStream,
			   PvPipeline *lPipeline )
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
	
	cout << "Freeing Pipeline" << endl;
	delete lPipeline;

	return; 
}

PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream )
{
    // Create the PvPipeline object
    PvPipeline* lPipeline = new PvPipeline( aStream );

    if ( lPipeline != NULL )
    {
        // Reading payload size from device
        uint32_t lSize = aDevice->GetPayloadSize();

        // Set the Buffer count and the Buffer size
        lPipeline->SetBufferCount( BUFFER_COUNT );
        lPipeline->SetBufferSize( lSize );
    }

    return lPipeline;
}


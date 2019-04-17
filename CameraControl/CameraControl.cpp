#include "lib/connect.h"
#include "lib/stream.h"
#include "lib/ImageAcquisition.h"
#include "lib/ConfigurationReader.h"
#include "lib/Configuration.h"
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

#define BUFFER_COUNT( 10 )

void clean_up( const PvDeviceInfo *lCameraInfo,
			   PvDevice *lCamera,
			   PvStream *lStream);
PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream );


int main( int aCount, const char **aArgs ){
	
	//Creates a configuration Instance
	Configuration lConfig;
	//Checks Command line for none default arguments
	lConfig.ParseCommandLine( aCount, aArgs );

	PvSystem lSystem;

	//PvDeviceInfo object for tracking and sotring device qualities
	const PvDeviceInfo *lCameraInfo;

	//PvString That stores the MAC address to be used
	const PvString MAC = PvString( lConfig.GetMAC().c_str() );
	//PvString That stores the IP address to be used
	const PvString IP  = PvString( lConfig.GetIP().c_str() );

	//Inits pointers of objects to be used later
	PvResult lResult;
	PvDevice *lCamera  = NULL;
	PvStream *lStream = NULL;
	PvPipeline *lPipeline = NULL;
	bool useMAC = true; 
	
	//Check if Device can be connected to via MAC
	lResult = lSystem.FindDevice( MAC, &lCameraInfo );
	if( !lResult.IsOK() )
	{ 
		cout << "Could not find device at " << MAC.GetAscii() << endl
		//specify that MAC is not useable as a connection point
		useMAC = false;
		//Check is IP is useable
		cout << "Attempting to find by IP " <<endl;
		lResult = lSystem.FindDevice( IP, &lCameraInfo );
		if( !lResult.IsOK() )
		{
			cout << "Could not find device at " << IP.GetAscii() << endl;
			//There is no connection point. Terminate the porgram
			cout << "Terminating Program" << endl; 
			clean_up(lCameraInfo, lCamera, lStream);
			return -1;
		}
	}

	//Connect to Camera
	if( useMAC ) { lCamera = connect( MAC ); }
	else { lCamera = connect( IP ); }
	if( lCamera != NULL )
	{
		lStream = open_stream( lCameraInfo );
		if( lStream != NULL )
		{	 
			ConfigureStream( lCamera, lStream );
			lPipeline = CreatePipeline( lCamera, lStream );
			if( lPipeline != NULL )
			{
				AcquireImages( lCamera, lStream, lPipeline );
				delete lPipeline;
			}
		}
	}
	cout << "Cleaning up..." << endl;
	clean_up(lCameraInfo, lCamera, lStream);
	cout << "Terminating program" << endl;	

	return 0;
}



void clean_up( const PvDeviceInfo *lCameraInfo,
			   PvDevice *lCamera,
			   PvStream *lStream)
{
	cout << "Freeing Camera Info" << endl;
	delete lCameraInfo;
	
	if( lCamera != NULL )
	{
		while( disconnect( lCamera ) != 0 );	
		cout << "Freeing Camera" << endl;
 		PvDevice::Free( lCamera ); 
	}
	
	if( lStream != NULL )
	{
		while( close_stream( lStream ) != 0 );
		cout << "Freeing Stream" << endl;
		PvStream::Free( lStream );
	}
	

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


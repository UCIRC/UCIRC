#include "lib/connect.h"
#include "lib/stream.h"
#include "lib/ImageAcquisition.h"
#include "lib/ConfigurationReader.h"
//#include "lib/Configuration.h"
#include <stdlib.h>
#include <PvSystem.h>
#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvSystemEnums.h>
#include <PvSampleUtils.h>


#define CONFIG_FILE ( "lib/Configuration.pvxml" )

void clean_up( PvDevice *lCamera, PvStream *lStream);
PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream, int64_t aBufferCount );
PvDevice *ConnectToDevice( const PvString &aConnectionID );
PvStream *OpenStream( const PvString &aConnectionID ); 
int BackupConnection ( PvDevice *aDevice, PvStream *aStream );  

int main(void){
	
//	PvSystem lSystem;
	PvResult lResult;
	PvDevice *lCamera = NULL;
	PvStream *lStream = NULL;
	PvPipeline *lPipeline;
	
	if ( RestoreConfiguration( lCamera, lStream ) != 1 )
	{
		

		PvString lConnectionID;
    	if ( PvSelectDevice( &lConnectionID ) )
    	{
        	lCamera = ConnectToDevice( lConnectionID );
        	if ( lCamera != NULL )
        	{
            	lStream = OpenStream( lConnectionID );
            	if ( lStream != NULL )
				{
					cout << "Seccusful Connection" << endl;
				}
			
				else 
				{
					cout << "Crit Connection Failure" << endl;
					clean_up( lCamera, lStream );
					return -1;	
				}	
			}
			else
			{
				cout << "Crit Connection Failure" << endl;
				clean_up( lCamera, lStream );
				return -1;	
			}
		}
//			return -1;
		//Stand in bit of code
//		if ( BackupConnection( lCamera, lStream ) != 1 )
//	{
//			cout << "Critical Connection Failure" << endl;
//			clean_up( lCamera, lStream );
//			return -1;
//		}
/*		
		else 
		{
			StoreConfiguration( lCamera, lStream );
			cout << "New Configuration Stored" << endl; 	
		}
*/	}

	if ( lCamera == NULL ) { cout << "Camera is Null " << endl; clean_up( lCamera, lStream ); return -1; }
	if ( lStream == NULL ) { cout << "Stream is NUll " << endl; clean_up( lCamera, lStream ); return -1; }
	if ( !lCamera->IsConnected() ) { cout << "Failed to Connect to device" << endl; clean_up( lCamera, lStream); return -1; }  
	if ( !lStream->IsOpen() ) { cout << "Failed to open stream" << endl; clean_up( lCamera, lStream); return -1; }  
	
	PvPropertyList *GeneralParams = NULL;
	cout << "Param List Declared" << endl;
	if( RestoreGeneralParams( GeneralParams ) != 0)
	{
		cout << "General Params Loaded" << endl;
		PvProperty *pBufferCount;
		pBufferCount = GeneralParams->GetProperty( "BufferCount" ); 
		int64_t lBufferCount;
		pBufferCount->GetValue( lBufferCount );
		cout << "Buffer Count: " << lBufferCount << endl;
		cout << "Configuring Stream" << endl;
		ConfigureStream( lCamera, lStream );
		cout << "Stream Configured" << endl;
		cout << "Creating Pipeline ... " << endl;
		lPipeline = CreatePipeline( lCamera, lStream, lBufferCount );
		cout << "Pipeline Created" << endl;
		if( lPipeline != NULL )
		{
			AcquireImages( lCamera, lStream, lPipeline, GeneralParams );
			delete lPipeline;
		}	

	}
	cout << "Cleaning up..." << endl;
	clean_up( lCamera, lStream);
	cout << "Terminating program" << endl;	

	return 0;

}

void clean_up( PvDevice *lCamera, PvStream *lStream )
{
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

PvDevice *ConnectToDevice( const PvString &aConnectionID )
{
    PvDevice *lDevice;
    PvResult lResult;

    // Connect to the GigE Vision or USB3 Vision device
    cout << "Connecting to device." << endl;
    lDevice = PvDevice::CreateAndConnect( aConnectionID, &lResult );
    if ( lDevice == NULL )
    {
        cout << "Unable to connect to device." << endl;
    }

    return lDevice;
}

PvStream *OpenStream( const PvString &aConnectionID )
{
    PvStream *lStream;
    PvResult lResult;

    // Open stream to the GigE Vision or USB3 Vision device
    cout << "Opening stream from device." << endl;
    lStream = PvStream::CreateAndOpen( aConnectionID, &lResult );
    if ( lStream == NULL )
    {
        cout << "Unable to stream from device." << endl;
    }

    return lStream;
}


PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream, int64_t aBufferCount )
{
    // Create the PvPipeline object
    PvPipeline* lPipeline = new PvPipeline( aStream );

    if ( lPipeline != NULL )
    {
        // Reading payload size from device
        uint32_t lSize = aDevice->GetPayloadSize();

        // Set the Buffer count and the Buffer size
        lPipeline->SetBufferCount( aBufferCount );
        lPipeline->SetBufferSize( lSize );
    }

    return lPipeline;
}

int BackupConnection( PvDevice *aDevice, PvStream *aStream ){
	
	PvString lConnectionID;
    if ( PvSelectDevice( &lConnectionID ) )
    {
        aDevice = ConnectToDevice( lConnectionID );
        if ( aDevice != NULL )
        {
            aStream = OpenStream( lConnectionID );
            if ( aStream != NULL )
			{
				return 1;
			}
		}
		
		return -1;
	}

	return -1;
}

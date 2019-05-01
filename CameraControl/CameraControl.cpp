#include "lib/CameraControl.h"

int main(void){
	
	PvResult lResult;
	PvDevice *lCamera = NULL;
	PvStream *lStream = NULL;
	PvPipeline *lPipeline;
	bool Connected = false;

	//Test Case we should Check: Attempting to restore when no device is connected
	//Restore Device from .pvxml
	lCamera = RestoreDevice();	
	if ( lCamera != NULL )
	{
		lStream = RestoreStream();
	
		if ( lStream != NULL )
		{
			cout << "Device and Stream Restored from Configuration File" << endl;
			//If Connection succesful, set Connected to true
			Connected = true;
		}
	}

	//If Not connected, intiate backup protocol
	if ( !Connected )
	{
		cout << "Configuration Restoration Failure, attempting Backup protocol" << endl;
		PvString lConnectionID;
    	if ( BackupConnection( &lConnectionID ) == 0 )
    	{
        	lCamera = ConnectToDevice( lConnectionID );
        	if ( lCamera != NULL )
        	{
            	lStream = OpenStream( lConnectionID );
            	if ( lStream != NULL )
				{
					//If Backup Succesful, Set Connected to true and Store the New Configuration
					cout << "Seccusful Connection" << endl;
					Connected = true;
					if ( StoreConfiguration( lCamera, lStream ) == 0 )
					{
						cout << "Stored New Configuration" << endl;
					}
					else
					{
						cout << "Issue Storing New Connection" << endl; 
					}
				}
			}
		}
	}

	if ( !Connected )
	{
		cout << "Critical Connection Failure" << endl;
		clean_up( lCamera, lStream );
		return -1;
	}

	if ( lCamera == NULL ) { cout << "Camera is Null " << endl; clean_up( lCamera, lStream ); return -1; }
	if ( lStream == NULL ) { cout << "Stream is NUll " << endl; clean_up( lCamera, lStream ); return -1; }
	if ( !lCamera->IsConnected() ) { cout << "Failed to Connect to device" << endl; clean_up( lCamera, lStream); return -1; }  
	if ( !lStream->IsOpen() ) { cout << "Failed to open stream" << endl; clean_up( lCamera, lStream); return -1; }  
	
	PvPropertyList *GeneralParams = NULL;
	cout << "Param List Declared" << endl;
	GeneralParams = RestoreGeneralParams();
	if( GeneralParams != NULL)
	{
		cout << "General Params Loaded" << endl;
		PvProperty *pBufferCount;
		pBufferCount = GeneralParams->GetProperty( PvString( "BufferCount" ) ); 
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
	
		else
		{
			cout << "Failed to Create Pipeline" << endl;
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

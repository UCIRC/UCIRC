#include "lib/CameraControl.h"

int main(void){
	
	PvResult lResult;
	PvDevice *lCamera_1 = NULL;
	PvStream *lStream_1 = NULL;
	PvDevice *lCamera_2 = NULL;
	PvStream *lStream_2 = NULL;
	PvPipeline *lPipeline_1 = NULL;
	PvPipeline *lPipeline_2 = NULL;
	bool Connected = false;

	//Test Case we should Check: Attempting to restore when no device is connected
	//Restore Device from .pvxml
	lCamera_1 = RestoreDevice(true);
	lCamera_2 = RestoreDevice(false);	
	if ( lCamera_1 != NULL && lCamera_2 != NULL )
	{
		lStream_1 = RestoreStream(true);
		lStream_2 = RestoreStream(false);
	
		if ( lStream_1 != NULL && lStream_2 != NULL )
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
		PvString lConnectionID_1;
		PvString lConnectionID_2;
    	if ( BackupConnection( &lConnectionID_1, NULL, true ) == 0 &&
			 BackupConnection( &lConnectionID_2, NULL, false) == 0   )
    	{
        	lCamera_1 = ConnectToDevice( lConnectionID_1 );
			lCamera_2 = ConnectToDevice( lConnectionID_2 );
        	if ( lCamera_1 != NULL && lCamera_2 != NULL )
        	{
            	lStream_1 = OpenStream( lConnectionID_1 );
				lStream_2 = OpenStream( lConnectionID_2 );
            	if ( lStream_1 != NULL && lStream_2 != NULL )
				{
					//If Backup Succesful, Set Connected to true and Store the New Configuration
					cout << "Seccusful Connection" << endl;
					Connected = true;
					if ( StoreConfiguration( lCamera_1, lStream_1 ) == 0 )
					{
						cout << "Stored First Configuration" << endl;
					}
					else
					{
						cout << "Issue Storing New First Connection" << endl; 
					}
					if ( StoreConfiguration( lCamera_2, lStream_2 ) == 0 )
					{
						cout << "Stored Second Configuration" << endl;
					}
					else
					{
						cout << "Issue Storing New Second Configuration" << endl;
					}
				}
			}
		}
	}

	if ( !Connected )
	{
		cout << "Critical Connection Failure" << endl;
		clean_up( lCamera_1, lStream_1 );
		clean_up( lCamera_2, lStream_2 );
		return -1;
	}

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
		ConfigureStream( lCamera_1, lStream_1 );
		ConfigureStream( lCamera_2, lStream_2 );
		cout << "Stream Configured" << endl;
		cout << "Creating Pipeline ... " << endl;
		lPipeline_1 = CreatePipeline( lCamera_1, lStream_1, lBufferCount );
		lPipeline_2 = CreatePipeline( lCamera_2, lStream_2, lBufferCount );
		cout << "Pipeline Created" << endl;
		if( lPipeline_1 != NULL && lPipeline_2 != NULL )
		{
			AcquireImages( lCamera_1, lStream_1, lPipeline_1, GeneralParams );
			delete lPipeline_1;
		}
		
		else { cout << "Failed to create Pipeline" << endl; }

		if( lPipeline_2 != NULL )
		{
			AcquireImages( lCamera_2, lStream_2, lPipeline_2, GeneralParams );
			delete lPipeline_2;
		}

		else { cout << "Failed to Create Pipeline" << endl; }	

	}
	cout << "Cleaning up..." << endl;
	clean_up( lCamera_1, lStream_1 );
	clean_up( lCamera_2, lStream_2 );
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
		cout << lResult.GetCodeString().GetAscii() << endl;
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


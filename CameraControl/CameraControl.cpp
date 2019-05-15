#include "lib/CameraControl.h"

int main(void){
	
	PvResult lResult;
	PvPropertyList *GeneralParams;
	PvDevice *lCamera_1;
	PvStream *lStream_1;
	PvDevice *lCamera_2;
	PvStream *lStream_2;
	const PvDeviceInfo *lDeviceInfo_1;
	const PvDeviceInfo *lDeviceInfo_2;
	PvString lConnectionID_1;
	PvString lConnectionID_2;
	PvPipeline *lPipeline_1 = NULL;
	PvPipeline *lPipeline_2 = NULL;
	bool Connected = false;
	int64_t lBufferCount;

	//Retrive General Parameters
	cout << "Restoring General Parameters..." << endl;
	
	GeneralParams = RestoreGeneralParams();
	
	if( GeneralParams != NULL )
	{
		cout << "General Params Loaded" << endl;
		PvProperty *lProperty;
		lProperty = GeneralParams->GetProperty( PvString( "BufferCount" ) ); 
		lProperty->GetValue( lBufferCount );
		cout << "Buffer Count: " << lBufferCount << endl;
	}

	else
	{
		cout << "Failed to read General Params" << endl;
		cout << "Using Hardcoded Values" << endl;
		lBufferCount = DEFAULT_BUFFER_COUNT;
		cout << "Buffer Count: " << lBufferCount << endl;
	}


/*
	//Test Case we should Check: Attempting to restore when no device is connected
	//Restore Device from .pvxml
	lCamera_1 = RestoreDevice(true);
	if(lCamera_1 != NULL) cout << "Camera One Created" << endl << endl;
	lCamera_2 = RestoreDevice(false);
	if(lCamera_2 != NULL) cout << "Camera Two Created" << endl << endl;
	if ( lCamera_1 != NULL && lCamera_2 != NULL )
	{
		lStream_1 = RestoreStream(true);
		if(lStream_1 != NULL) cout << "Stream One created" << endl;
		lStream_2 = RestoreStream(false);
		if(lStream_2 != NULL) cout << "Stream Two Created" << endl;
	
		if ( lStream_1 != NULL && lStream_2 != NULL )
		{
			cout << "Device and Stream Restored from Configuration File" << endl;
			//If Connection succesful, set Connected to true
			Connected = true;
		}
	}
*/
	//If Not connected, intiate backup protocol
	if ( !Connected )
	{
		cout << "Configuration Restoration Failure, attempting Backup protocol" << endl;
    	if ( BackupConnection( &lDeviceInfo_1, &lDeviceInfo_2 ) == 0 )
    	{
			if( lDeviceInfo_1 != NULL && lDeviceInfo_2 != NULL )
			{
				lConnectionID_1 = lDeviceInfo_1->GetConnectionID();
				cout << "First Device Connection ID: " << lConnectionID_1.GetAscii() << endl;
				lConnectionID_2 = lDeviceInfo_2->GetConnectionID();
				cout << "Second Device Connection ID: " << lConnectionID_2.GetAscii() << endl;
        	
				lCamera_1 = ConnectToDevice( lConnectionID_1 );
				lCamera_2 = ConnectToDevice( lConnectionID_2 );
			}

			else
			{
				cout << "Failed to Retrieve Device info" << endl;
			}		        	

			if ( lCamera_1 != NULL && lCamera_2 != NULL )
        	{
            	lStream_1 = OpenStream( lConnectionID_1 );
				lStream_2 = OpenStream( lConnectionID_2 );
            	if ( lStream_1 != NULL && lStream_2 != NULL )
				{
					//If Backup Succesful, Set Connected to true and Store the New Configuration
					cout << "Seccusful Connection" << endl;
					Connected = true;
					if ( StoreConfiguration( lCamera_1, lStream_1, true ) == 0 )
					{
						cout << "Stored First Configuration" << endl;
					}
					else
					{
						cout << "Issue Storing New First Connection" << endl; 
					}
					if ( StoreConfiguration( lCamera_2, lStream_2, false ) == 0 )
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
		delete GeneralParams;
		return -1;
	}



	cout << "Configuring Stream 1..." << endl;
	ConfigureStream( lCamera_1, lStream_1 );
	cout << "Configuring Stream 2..." << endl;
	ConfigureStream( lCamera_2, lStream_2 );
	cout << "Streams Configured" << endl;
	
	cout << "Creating Pipeline 1... " << endl;
	lPipeline_1 = CreatePipeline( lCamera_1, lStream_1, lBufferCount );
	if( lPipeline_1 != NULL) cout << "Pipeline 1 Created" << endl;
	else cout << "Failed to creat Pipeline 1" << endl;
	cout << "creating Pipeline 2..." << endl;
	lPipeline_2 = CreatePipeline( lCamera_2, lStream_2, lBufferCount );
	if( lPipeline_2 != NULL) cout << "Pipeline 2 Created" << endl;
	else cout << "Failed to creat Pipeline 2" << endl;

	if( lPipeline_1 != NULL )
	{
		cout << "Acquiring Images from Pipeline 1..." << endl;
//		AcquireImages( lCamera_1, lStream_1, lPipeline_1, GeneralParams );
		//We're done with this pipeline so we delete it from memory
		delete lPipeline_1;
	}

	if( lPipeline_2 != NULL )
	{
		cout << "Acquiring Images from Pipeline 2..." << endl;
//		AcquireImages( lCamera_2, lStream_2, lPipeline_2, GeneralParams );
		//We're done with this pipeline so we delete it from memory
		delete lPipeline_2;
	}

	cout << "Cleaning up..." << endl;
	clean_up( lCamera_1, lStream_1 );
	clean_up( lCamera_2, lStream_2 );
	delete GeneralParams;
	cout << "Terminating program" << endl;	

	return 0;

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
		cout << lResult.GetCodeString().GetAscii() << endl;
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

int close_stream ( PvStream *lStream ){
    PvResult lResult;
    cout << "Closing Stream..." << endl;
    lResult = lStream->Close();
    if( !lResult.IsOK() ){
        cout << "Failed to close the stream" << endl;
        return -1;
    }
    cout << "Closed the stream" << endl;
    return 0;
}

void ConfigureStream( PvDevice *aDevice, PvStream *aStream )
{
    // If this is a GigE Vision device, configure GigE Vision specific streaming parameters
    PvDeviceGEV* lDeviceGEV = dynamic_cast<PvDeviceGEV *>( aDevice );
    if ( lDeviceGEV != NULL )
    {
        PvStreamGEV *lStreamGEV = static_cast<PvStreamGEV *>( aStream );

        // Negotiate packet size
        lDeviceGEV->NegotiatePacketSize();

        // Configure device streaming destination
        lDeviceGEV->SetStreamDestination( lStreamGEV->GetLocalIPAddress(), lStreamGEV->GetLocalPort() );
    }
}


#include "lib/Camera.h"

PvDevice *ConnectToDevice( const PvString &aConnectionID );
PvStream *OpenStream( const PvString &aConnectionID );
PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream, int64_t aBufferCount );
void clean_up( PvDevice *lCamera, PvStream *lStream );
int disconnect( PvDevice *lCamera );
int close_stream ( PvStream *lStream );
void ConfigureStream( PvDevice *aDevice, PvStream *aStream );

Camera *NewCamera( PvString aName )
{
	Camera *aCamera = new Camera;
	aCamera->Device = NULL;
	aCamera->Stream = NULL;
	aCamera->DeviceInfo = NULL;
	aCamera->Pipeline = NULL;
	aCamera->Name = aName;
}

void FreeCameraInfo( Camera *aCamera )
{
	clean_up( aCamera->Device, aCamera->Stream);
	aCamera->Device = NULL;
	aCamera->Stream = NULL;
	if ( aCamera->DeviceInfo != NULL )
	{
		delete aCamera->DeviceInfo;
		aCamera->DeviceInfo = NULL;
	}
	if ( aCamera->Pipeline != NULL )
	{
		delete aCamera->Pipeline;
		aCamera->Pipeline = NULL;
	}
	return;
}

bool SetupCamera( Camera *aCamera, bool select, int64_t aBufferCount )
{
	PvSystem lSystem;
	PvResult lResult;
	PvDevice *lDevice;
	PvStream *lStream;
	const PvDeviceInfo *lDeviceInfo;
	PvString lConnectionID;
	PvPipeline *lPipeline;
	bool Connected = false;

	//Attempt To restore Connection
	cout << "Attempting to Restore from pvxml file" << endl;
	lDevice = RestoreDevice( select );
	if( lDevice != NULL )
	{
		aCamera->Device = lDevice;
		cout << "Restored Device" << endl;
		lStream = RestoreStream( select );
		if ( lStream != NULL )
		{
			cout << "Restored Stream" << endl;
			aCamera->Stream = lStream;
			Connected = true;
		}
	}
		
	//If Restoring from pvxml dosesnt work then Connect using name
	if ( !Connected )
	{
		cout << "Attempting to Restore via Device Name..." << endl;
		FreeCameraInfo( aCamera );
		lResult = lSystem.FindDevice( aCamera->Name, &lDeviceInfo );
		if ( lResult.IsOK() )
		{
			aCamera->DeviceInfo = lDeviceInfo;
			lConnectionID = lDeviceInfo->GetConnectionID();
			lDevice = ConnectToDevice( lConnectionID );
			if ( lDevice != NULL )
			{
				cout << "Connected to device" << endl;
				aCamera->Device = lDevice;
				lStream = OpenStream( lConnectionID );
				if ( lStream != NULL )
				{
					cout << "Opened Stream" << endl;
					aCamera->Stream = lStream;
					Connected = true;
					cout << "Storing New Configuration" << endl;
					StoreConfiguration( lDevice, lStream, select );
				}
			}
		}
		else { cout << lResult.GetCodeString().GetAscii() << endl; }
	}

	if ( !Connected )
	{
		cout << "Attempting to connect via Backup..." << endl;
		FreeCameraInfo( aCamera );	
		if ( BackupConnection( &lDeviceInfo ) )
		{
			aCamera->DeviceInfo = lDeviceInfo;
			lConnectionID = lDeviceInfo->GetConnectionID();
			lDevice = ConnectToDevice( lConnectionID );
			if ( lDevice != NULL )
			{
				cout << "Connected to Device" << endl;
				aCamera->Device = lDevice;
				lStream = OpenStream( lConnectionID );
				if ( lStream != NULL )
				{
					cout << "Opened Stream" << endl;
					aCamera->Stream = lStream;
					Connected = true;
					cout << "Storing New Configuration" << endl;
					StoreConfiguration( lDevice, lStream , select );
				}
			}
		}
	}

	if ( !Connected )
	{
		cout << "Critical Connection Failure" << endl;
		FreeCameraInfo( aCamera );
		return false;
	}

	cout << "Configuring Stream..." << endl;
	ConfigureStream( lDevice, lStream );
	cout << "Stream Configured" << endl; 	

	cout << "Creating Pipeline..." << endl;
	lPipeline = CreatePipeline( lDevice, lStream, aBufferCount );
	if ( lPipeline == NULL )
	{
		cout << "Failed to creat Pipeline" << endl;
		FreeCameraInfo( aCamera );
		return false;
	}
	cout << "Created Pipeline" << endl;
	
	aCamera->Pipeline = lPipeline;

	return true;
}


/*Will Only Return a non NULL value if 
  Device is Created and Connected */
PvDevice *ConnectToDevice( const PvString &aConnectionID )
{
    PvDevice *lDevice;
    PvResult lResult;

    // Connect to the GigE Vision or USB3 Vision device
    cout << "Connecting to device." << endl;
    lDevice = PvDevice::CreateAndConnect( aConnectionID, &lResult );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to connect to device." << endl;
		cout << lResult.GetCodeString().GetAscii() << endl;
		return NULL;
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
    if ( !lResult.IsOK() )
    {
        cout << "Unable to stream from device." << endl;
		cout << lResult.GetCodeString().GetAscii() << endl;
		return NULL;
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


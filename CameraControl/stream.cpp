#include "lib/stream.h"
#include <PvStream.h>
#include <PvSampleUtils.h>
#include <PvResult.h>
#include <PvDeviceGEV.h>
#include <PvStreamGEV.h>

PvStream *open_stream ( const PvDeviceInfo *lCameraInfo ){
	cout << "Opening Stream to Device" << endl;

	PvResult lResult;
	PvStream *lStream;
	
	lStream = PvStream::CreateAndOpen( lCameraInfo, &lResult );
	if( !lResult.IsOK() ){
		cout << "Could not open strem" << endl;
		return NULL;
	}
	
	cout << "Stream opened" << endl;
	
	return lStream;

}

int close_stream ( PvStream *lStream ){
	PvResult lResult;
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


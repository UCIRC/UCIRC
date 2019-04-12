#include <PvSampleUtils.h>
#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvStreamU3V.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvBufferWriter.h>
#include <stdlib.h>

PV_INIT_SIGNAL_HANDLER();

#define BUFFER_COUNT ( 1 )
#define IMAGE_COUNT  ( 1 )


void AcquireImages( PvDevice *aDevice, PvStream *aStream, PvPipeline *aPipeline )
{
    // Get device parameters need to control streaming
    PvGenParameterArray *lDeviceParams = aDevice->GetParameters();

    // Map the GenICam AcquisitionStart and AcquisitionStop commands
    PvGenCommand *lStart = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStart" ) );
    PvGenCommand *lStop = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStop" ) );

    // Note: the pipeline must be initialized before we start acquisition
    cout << "Starting pipeline" << endl;
    aPipeline->Start();

    // Get stream parameters
    PvGenParameterArray *lStreamParams = aStream->GetParameters();

    // Map a few GenICam stream stats counters
//    PvGenFloat *lFrameRate = dynamic_cast<PvGenFloat *>( lStreamParams->Get( "AcquisitionRate" ) );
    PvGenFloat *lBandwidth = dynamic_cast<PvGenFloat *>( lStreamParams->Get( "Bandwidth" ) );

    // Enable streaming and send the AcquisitionStart command
    cout << "Enabling streaming and sending AcquisitionStart command." << endl;
    aDevice->StreamEnable();
    cout << "Stream Enabled" << endl;
	lStart->Execute();
	cout << "Start Executed" << endl;

    double lBandwidthVal = 0.0;

    for(int i=0; i<IMAGE_COUNT; i++)
    {
        PvBuffer *lBuffer = NULL;
        PvResult lOperationResult;

        // Retrieve next buffer
		cout << "Retrieving Next Buffer" << endl;
        PvResult lResult = aPipeline->RetrieveNextBuffer( &lBuffer, 10000, &lOperationResult );
        if ( lResult.IsOK() )
        {
            if ( lOperationResult.IsOK() )
            {
                PvPayloadType lType;

                lBandwidth->GetValue( lBandwidthVal );

                lType = lBuffer->GetPayloadType();
				//Check if Payload Type is Image
                if ( lType == PvPayloadTypeImage )
                {
                    // Get image specific buffer interface.
                    PvImage *lImage = lBuffer->GetImage();
					PvResult lResult;
					const char *file = "image";
					const PvString Filename =  PvString(file);
					uint32_t *BytesWritten = NULL;
					PvBufferFormatType Format = PvBufferFormatBMP;
					PvBufferWriter lWriter;
					lResult = lWriter.Store(lBuffer, Filename);
					if( !lResult.IsOK() )
					{
						cout << "Failed to Save Image" << endl;
					}
                }
                else {
                    cout << " (buffer does not contain image)";
                }
            }
            else
            {
				cout << "Operational Result Failure" << endl;
                // Non OK operational result
                cout  << lOperationResult.GetCodeString().GetAscii() << endl;
            }
			cout << "Realeasing Buffer" << endl;
            // Release the buffer back to the pipeline
            aPipeline->ReleaseBuffer( lBuffer );
        }
        else
        {
			cout << "Failed to retrieve buffer" << endl;
            // Retrieve buffer failure
            cout << " " << lResult.GetCodeString().GetAscii() << endl;
			continue;
        }
    }

    PvGetChar(); // Flush key buffer for next stop.
    cout << endl << endl;

    // Tell the device to stop sending images.
    cout << "Sending AcquisitionStop command to the device" << endl;
    lStop->Execute();

    // Disable streaming on the device
    cout << "Disable streaming on the controller." << endl;
    aDevice->StreamDisable();

    // Stop the pipeline
    cout << "Stop pipeline" << endl;
    aPipeline->Stop();
}

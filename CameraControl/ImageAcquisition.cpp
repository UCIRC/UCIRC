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

#define BUFFER_COUNT ( 6 )



void AcquireImages( PvDevice *aDevice, PvStream *aStream, PvPipeline *aPipeline, int Count )
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
    lStart->Execute();

//    char lDoodle[] = "|\\-|-/";
//    int lDoodleIndex = 0;
//    double lFrameRateVal = 0.0;
    double lBandwidthVal = 0.0;

    // Acquire images until the user instructs us to stop.
//    cout << endl << "<press a key to stop streaming>" << endl;
    for(int i=0; i<Count; i++)
    {
        PvBuffer *lBuffer = NULL;
        PvResult lOperationResult;

        // Retrieve next buffer
        PvResult lResult = aPipeline->RetrieveNextBuffer( &lBuffer, 1000, &lOperationResult );
        if ( lResult.IsOK() )
        {
            if ( lOperationResult.IsOK() )
            {
                PvPayloadType lType;

                //
                // We now have a valid buffer. This is where you would typically process the buffer.
                // -----------------------------------------------------------------------------------------
                // ...

//                lFrameRate->GetValue( lFrameRateVal );
                lBandwidth->GetValue( lBandwidthVal );

                // If the buffer contains an image, display width and height.
//                uint32_t lWidth = 0, lHeight = 0;
                lType = lBuffer->GetPayloadType();

//                cout << fixed << setprecision( 1 );
//                cout << lDoodle[ lDoodleIndex ];
//                cout << " BlockID: " << uppercase << hex << setfill( '0' ) << setw( 16 ) << lBuffer->GetBlockID();
                if ( lType == PvPayloadTypeImage )
                {
                    // Get image specific buffer interface.
//                    PvImage *lImage = lBuffer->GetImage();
					PvResult lResult;
					const char *file = "image";
					const PvString Filename =  PvString(file);
					uint32_t *BytesWritten = NULL;
					PvBufferFormatType Format = PvBufferFormatBMP;
					PvBufferWriter Writer;
					lResult = Writer.Store(lBuffer, Filename);
					if( !lResult.IsOK() )
					{
						cout << "Failed to Save Image" << endl;
					}
                    // Read width, height.
  //                  lWidth = lImage->GetWidth();
    //                lHeight = lImage->GetHeight();
      //              cout << "  W: " << dec << lWidth << " H: " << lHeight;
                }
                else {
                    cout << " (buffer does not contain image)";
                }
        //        cout << "  " << lFrameRateVal << " FPS  " << ( lBandwidthVal / 1000000.0 ) << " Mb/s   \r";
            }
            else
            {
                // Non OK operational result
                cout  << lOperationResult.GetCodeString().GetAscii() << "\r";
            }

            // Release the buffer back to the pipeline
            aPipeline->ReleaseBuffer( lBuffer );
        }
        else
        {
            // Retrieve buffer failure
            cout << " " << lResult.GetCodeString().GetAscii() << "\r";
        }

//        ++lDoodleIndex %= 6;
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

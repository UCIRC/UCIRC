#include "lib/ImageAcquisition.h"

//PV_INIT_SIGNAL_HANDLER();


bool AcquireImages( Camera *aCamera, string File, int64_t ImageCount )
{
	bool result = false;
	//Raw Image Parameters
	uint32_t *BytesWritten = NULL;
	PvBufferFormatType Format = PvBufferFormatRaw;
	PvBufferWriter lWriter;
	PvPipeline *aPipeline = aCamera->Pipeline;
	PvDevice *aDevice = aCamera->Device;


	// Get device parameters need to control streaming
	PvGenParameterArray *lDeviceParams = aDevice->GetParameters();

	// Map the GenICam AcquisitionStart and AcquisitionStop commands
	PvGenCommand *lStart = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStart" ) );
	PvGenCommand *lStop = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStop" ) );

	// Note: the pipeline must be initialized before we start acquisition
	cout << "Starting pipeline" << endl;
	aPipeline->Start();

    // Enable streaming and send the AcquisitionStart command
    cout << "Enabling streaming and sending AcquisitionStart command." << endl;
    aDevice->StreamEnable();
    cout << "Stream Enabled" << endl;
	lStart->Execute();
	cout << "Start Executed" << endl;
	

    for(int64_t i=0; i<ImageCount; i++)
    {
        PvBuffer *lBuffer = NULL;
		PvResult lResult;
        PvResult lOperationResult;

        // Retrieve next buffer
		cout << "Retrieving Next Buffer" << endl;
        lResult = aPipeline->RetrieveNextBuffer( &lBuffer, 10000, &lOperationResult );
        if ( lResult.IsOK() )
        {
            if ( lOperationResult.IsOK() )
            {
                PvPayloadType lType;
                lType = lBuffer->GetPayloadType();

				//Check if Payload Type is RawData
                if ( lType == PvPayloadTypeRawData )
				{
					//If So, get the data
					cout << "Image is RawData" << endl;
				}

				else if ( lType == PvPayloadTypeImage )
				{
					//If it's an image convert it to RawData
					cout << "Resetting Buffer Format" << endl;
					lResult = lBuffer->Reset(PvPayloadTypeRawData);
					if( !lResult.IsOK() )
					{
						cout << "Failed to Reset Buffer" << endl;
						continue;
					}
				}

				else
				{
					cout << "Data is neither an Image or Raw Data" << endl;
					continue;
				}

				File += "(" + to_string(i) + ")";
				File += ".raw";
	
				const PvString Filename = PvString( File.c_str() );
				
				lResult = lWriter.Store(lBuffer, Filename, Format, BytesWritten );
				
				if( !lResult.IsOK() )
				{
					cout << "Failed to Save Image" << endl;
				}
				else
				{
					cout << "Saved Image" << endl;
					result = result || true;
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
            lResult = aPipeline->ReleaseBuffer( lBuffer );
			cout << lResult.GetCodeString().GetAscii() << endl;
			cout << "Buffer Released" << endl;
        }
        else
        {
			cout << "Failed to retrieve buffer" << endl;
            // Retrieve buffer failure
            cout << " " << lResult.GetCodeString().GetAscii() << endl;
        }
    }

    cout << endl;

    // Tell the device to stop sending images.
    cout << "Sending AcquisitionStop command to the device" << endl;
    lStop->Execute();

    // Disable streaming on the device
    cout << "Disable streaming on the controller." << endl;
    aDevice->StreamDisable();

    // Stop the pipeline
    cout << "Stop pipeline" << endl;
    aPipeline->Stop();

	if(!result) cout << "COMPLETE IMAGE ACQUISITION FAILURE" << endl;
	return result;
}

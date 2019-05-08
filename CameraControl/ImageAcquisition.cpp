#include "lib/ImageAcquisition.h"

PV_INIT_SIGNAL_HANDLER();


void AcquireImages( PvDevice *aDevice, PvStream *aStream, PvPipeline *aPipeline, PvPropertyList *GeneralParams )
{
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
	
	//p for "property"
	PvProperty *pImageCount;
	pImageCount = GeneralParams->GetProperty( "ImageCount" );
	int64_t ImageCount;
	pImageCount->GetValue( ImageCount );

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
	//			PvRawData *lRawData;

				//Check if Payload Type is RawData
                if ( lType == PvPayloadTypeRawData )
				{
					//If So, get the data
					cout << "Image is RawData" << endl;
					PvRawData *lRawData = lBuffer->GetRawData();
				}

				else if ( lType == PvPayloadTypeImage )
				{
					//If it's an image convert it to RawData
					cout << "Resetting Buffer Format" << endl;
				//	PvBuffer *lRawBuffer;
				//	PvBufferConverter lConverter;
				//	lResult = lConverter.Convert( lBuffer, lRawBuffer );
					lResult = lBuffer->Reset(PvPayloadTypeRawData);
					if( !lResult.IsOK() )
					{
						cout << "Failed to Reset Buffer" << endl;
						//Potential Issue: Continuing through the loop might cause unwanted affects(?)
						continue;
					}
					PvRawData *lRawData = lBuffer->GetRawData();

				}
				else
				{
					cout << "Data is neither an Image or Raw Data" << endl;
					continue;
				}

				//Get The Timestamp of the Data
			//	uint64_t time = lBuffer->GetTimestamp();
				uint64_t time = i;
				PvProperty *pImagePath;
				pImagePath = GeneralParams->GetProperty( "ImagePath" );
				string dir = pImagePath->GetValue().GetAscii();	
				string file = dir;
				std::ostringstream o;
				o << time;
				file += o.str();
				const PvString Filename =  PvString(file.c_str());
				cout << "Image FileName: " << Filename.GetAscii() << endl; 
				uint32_t *BytesWritten = NULL;
				PvBufferFormatType Format = PvBufferFormatRaw;
				PvBufferWriter lWriter;
				lResult = lWriter.Store(lBuffer, Filename, Format, BytesWritten );
				if( !lResult.IsOK() )
				{
					cout << "Failed to Save Image" << endl;
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
			cout << "Buffer Released" << endl;
        }
        else
        {
			cout << "Failed to retrieve buffer" << endl;
            // Retrieve buffer failure
            cout << " " << lResult.GetCodeString().GetAscii() << endl;
        }
    }

//    PvGetChar(); // Flush key buffer for next stop.
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

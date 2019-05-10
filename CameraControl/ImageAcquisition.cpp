#include "lib/ImageAcquisition.h"

PV_INIT_SIGNAL_HANDLER();


bool AcquireImages( PvDevice *aDevice, PvStream *aStream, PvPipeline *aPipeline, PvPropertyList *GeneralParams )
{
	int64_t ImageCount;
	string Directory;
	bool result = false;

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
	
	if( GeneralParams != NULL )
	{	
		//p for "property"
		PvProperty *pImageCount;
		pImageCount = GeneralParams->GetProperty( "ImageCount" );
		pImageCount->GetValue( ImageCount );
	}

	else
	{
		ImageCount = 6;
	}

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
				PvRawData *lRawData;

				//Check if Payload Type is RawData
                if ( lType == PvPayloadTypeRawData )
				{
					//If So, get the data
					cout << "Image is RawData" << endl;
					lRawData = lBuffer->GetRawData();
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
					PvRawData *lRawData = lBuffer->GetRawData();

				}
				else
				{
					cout << "Data is neither an Image or Raw Data" << endl;
					continue;
				}

				//Get The Timestamp of the Data

				char time_buf[21];
				time_t now;
				time(&now);
				strftime(time_buf, 21, "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
				
				if( GeneralParams != NULL)
				{	
					PvProperty *pImagePath;
					pImagePath = GeneralParams->GetProperty( "ImagePath" );
					Directory = pImagePath->GetValue().GetAscii();	
				}

				else
				{
					Directory = "Data/"; 
				}

				string file = Directory;
				std::ostringstream o;
				o << time_buf;
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

	if(!result) cout << "Complete Image Acquisition Failure" << endl;
	return result;
}

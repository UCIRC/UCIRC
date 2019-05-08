#include "lib/BackupConnection.h"

int BackupConnection( PvString *aConnectionID, PvDeviceInfoType *aType, bool camera )
{
    PvResult lResult;
    const PvDeviceInfo *lSelectedDI = NULL;
    PvSystem lSystem;

	lSystem.Find();
    // Detect, select device.
    vector<const PvDeviceInfo *> lDIVector;
    for ( uint32_t i = 0; i < lSystem.GetInterfaceCount(); i++ )
    {
        const PvInterface *lInterface = dynamic_cast<const PvInterface *>( lSystem.GetInterface( i ) );
        if ( lInterface != NULL )
        {
            cout << "   " << lInterface->GetDisplayID().GetAscii() << endl;
            for ( uint32_t j = 0; j < lInterface->GetDeviceCount(); j++ )
            {
                const PvDeviceInfo *lDI = dynamic_cast<const PvDeviceInfo *>( lInterface->GetDeviceInfo( j ) );
                if ( lDI != NULL )
                {
                    lDIVector.push_back( lDI );
                    cout << "[" << ( lDIVector.size() - 1 ) << "]" << "\t" << lDI->GetDisplayID().GetAscii() << endl;
                }					
            }
        }
    }

    if ( lDIVector.size() == 0)
    {
        cout << "No device found!" << endl;
		return -1;
    }


    // Read device selection, optional new IP address.
    uint32_t lIndex = 0;
	
	//Controlling which Camera Gets connected to
	if ( camera )
	{
		lIndex = 0;
		cout << "Getting information for first device..." << endl;
	}

	else
	{
		lIndex = 1;
		cout << "Getting information for second device..." << endl;
	}
    
    if ( lIndex < lDIVector.size() )
    {
        // The device is selected
        lSelectedDI = lDIVector[ lIndex ];
    }

	else
	{
		cout << "Failed to detect Requested Device" << endl;
		return -1;
	}


    // If the IP Address valid?
    if ( lSelectedDI->IsConfigurationValid() )
    {
        *aConnectionID = lSelectedDI->GetConnectionID();
        if ( aType != NULL )
        {
            *aType = lSelectedDI->GetType();
        }

        return 0;
    }

	else
	{
		cout << "Device Configuratio is Invalid" << endl;
		return -1;
	}

}

#include "lib/BackupConnection.h"

bool BackupConnection( const PvDeviceInfo **aDeviceInfo )
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
		return false;
    }
	
    uint32_t lIndex = 0;
	
    
    lSelectedDI = lDIVector[ lIndex ];


    // If the IP  Address of Device 1 valid?
    if ( lSelectedDI->IsConfigurationValid() )
    {
        *aDeviceInfo = lSelectedDI->Copy();
		cout << "Device Configuration Valid" << endl;
		return true;
    }

	cout << "Device Configuration is Invalid" << endl;
	return false;
}

#include "lib/BackupConnection.h"

int BackupConnection( const PvDeviceInfo **aDeviceInfo_1, const PvDeviceInfo **aDeviceInfo_2 )
{
    PvResult lResult;
	const PvDeviceInfo *lSelectedDI_1 = NULL;
	const PvDeviceInfo *lSelectedDI_2 = NULL;
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
	
	else if ( lDIVector.size() < 2 )
	{
		cout << "Less than two devices found!" << endl;
		return -1;
	}


    uint32_t lIndex_1 = 0;
	uint32_t lIndex_2 = 1;
	
    
    lSelectedDI_1 = lDIVector[ lIndex_1 ];
	lSelectedDI_2 = lDIVector[ lIndex_2 ];


    // If the IP  Address of Device 1 valid?
    if ( lSelectedDI_1->IsConfigurationValid() )
    {
        *aDeviceInfo_1 = lSelectedDI_1->Copy();
		if ( lSelectedDI_2->IsConfigurationValid() )
		{
			*aDeviceInfo_2 = lSelectedDI_2->Copy();
			cout << "Device Configurations Valid" << endl;
			return 0;
		}
    }

	cout << "Device Configurations are Invalid" << endl;
	return -1;
}

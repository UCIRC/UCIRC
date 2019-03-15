#include <PvSystem.h>

//connect to device via MAC address
//Args: A MAC Address in the form of a PvString
//a pointer to a PvDeviceInfo object; empty
PvDevice *connect( const PvString MAC );
//Disconnect from Device
int disconnect( PvDevice *lCamera );

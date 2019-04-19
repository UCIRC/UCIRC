#include <PvString.h>
#include <PvDevice.h>
#include <PvStream.h>


bool StoreConfiguartion( PvDevice *aDevice, PvStream *aStream, bool camera = true );

PvDevice *RestoreDevice( bool camera = true );

PvStream *RestoreStream( bool camera = true );



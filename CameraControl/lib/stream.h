#include <PvStream.h>
#include <PvDevice.h>

PvStream *open_stream( const PvDeviceInfo *lCameraInfo );

int close_stream( PvStream *lStream );

void ConfigureStream( PvDevice *aDevice, PvStream *aStream );

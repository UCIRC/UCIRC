#include <PvStream.h>
#include <PvDevice.h>

PvStream *open_stream( const PvString *lCameraInfo );

int close_stream( PvStream *lStream );

void ConfigureStream( PvDevice *aDevice, PvStream *aStream );

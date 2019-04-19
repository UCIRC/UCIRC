#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include "Configuration.h"

PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream );

void AcquireImages( PvDevice *lCamera, PvStream *lStream, PvPipeline *lPipeline, Configuration *lConfig );

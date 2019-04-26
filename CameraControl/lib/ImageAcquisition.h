#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvPropertyList.h>

PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream );

void AcquireImages( PvDevice *lCamera, PvStream *lStream, PvPipeline *lPipeline, PvPropertyList *GeneralParams );

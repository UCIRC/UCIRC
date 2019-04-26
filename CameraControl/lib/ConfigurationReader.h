#include <PvString.h>
#include <PvDevice.h>
#include <PvStream.h>
#include <PvPropertyList.h>

int StoreConfiguartion( PvDevice *aDevice, PvStream *aStream, bool camera = true );


int RestoreConfiguration( PvDevice *aDevice, PvStream *aStream, bool camera = true );

PvPropertyList *GetGeneralParams();

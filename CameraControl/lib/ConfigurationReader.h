#include <PvString.h>
#include <PvDevice.h>
#include <PvStream.h>
#include <PvPropertyList.h>

int StoreConfiguration( PvDevice *aDevice, PvStream *aStream, bool camera = true );


int RestoreConfiguration( PvDevice *aDevice, PvStream *aStream, bool camera = true );

int RestoreGeneralParams( PvPropertyList *aList );

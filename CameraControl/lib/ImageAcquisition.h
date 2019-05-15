#ifndef INIT
#define INIT
#include <PvDeviceInfo.h>
#include <PvStreamGEV.h>
#include <PvDeviceGEV.h>
#include <stdlib.h>
#include <PvString.h>
#include <PvSystem.h>
#include <PvResult.h>
#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvBufferWriter.h>
#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>
#include <PvPropertyList.h>
#include <PvSystemEnums.h>
#include <PvSampleUtils.h>

#define DEFAULT_IMAGE_COUNT ( 6 )
#define DEFAULT_IMAGE_PATH ( "Data/" )
#define DEFAULT_BUFFER_COUNT ( 10 )

#endif

#ifndef IMAGEACQUISITION_H
#define IMAGEACQUISITION_H

#include <ctime>
#include <time.h>

static std::time_t time_now = std::time(nullptr);

PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream );

bool AcquireImages( PvDevice *lCamera, PvStream *lStream, PvPipeline *lPipeline, PvPropertyList *GeneralParams );

#endif

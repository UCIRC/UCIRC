#ifndef INIT
#define INIT
#include <PvDeviceInfo.h>
#include <stdlib.h>
#include <PvString.h>
#include <PvSystem.h>
#include <PvResult.h>
#include <PvDevice.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvDeviceGEV.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvBufferWriter.h>
#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>
#include <PvPropertyList.h>
#include <PvSystemEnums.h>
#include <PvSampleUtils.h>


#define DEFAULT_MAGE_COUNT ( 6 )
#define DEFAULT_IMAGE_PATH ( "Data/" )
#define DEFAULT_BUFFER_COUNT ( 10 )

#endif


#ifndef STREAM_H
#define STREAM_H

PvStream *open_stream( const PvString *lCameraInfo );

int close_stream( PvStream *lStream );

void ConfigureStream( PvDevice *aDevice, PvStream *aStream );

#endif

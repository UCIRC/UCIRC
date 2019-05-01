#ifndef INIT
#define INIT

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

#endif


#ifndef STREAM_H
#define STREAM_H

#include <PvStreamGEV.h>

PvStream *open_stream( const PvString *lCameraInfo );

int close_stream( PvStream *lStream );

void ConfigureStream( PvDevice *aDevice, PvStream *aStream );

#endif

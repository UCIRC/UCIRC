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

#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H
#include "connect.h"
#include "stream.h"
#include "ImageAcquisition.h"
#include "ConfigurationReader.h"
#include "BackupConnection.h"

void clean_up( PvDevice *lCamera, PvStream *lStream);
PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream, int64_t aBufferCount );
PvDevice *ConnectToDevice( const PvString &aConnectionID );
PvStream *OpenStream( const PvString &aConnectionID );

#endif

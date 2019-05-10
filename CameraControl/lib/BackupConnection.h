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


#ifndef BACKUPCONNECTION_H
#define BACKUPCONNECTION_H

#include <PvDeviceInfo.h>

int BackupConnection( const PvDeviceInfo **aDeviceInfo_1, const PvDeviceInfo **aDeviceInfo_2 );

#endif

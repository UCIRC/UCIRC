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


#ifndef CONNECT_H
#define CONNECT_H


//connect to device via MAC address
//Args: A MAC Address in the form of a PvString
//a pointer to a PvDeviceInfo object; empty
PvDevice *connect( const PvString connection_point );
//Disconnect from Device
int disconnect( PvDevice *lCamera );

#endif

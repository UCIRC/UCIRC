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


#ifndef CONFIGURATIONREADER_H
#define CONFIGURATIONREADER_H

 
//FileName of the Config File 
#define CONFIG_FILE ( "lib/Configuration.pvxml" )
#define PARAM_FILE  ( "lib/Configuration.pvcfg" ) 
#define CAMERA1_CONFIGURATION_TAG ( "Camera1" ) 
#define STREAM1_CONFIGURAITON_TAG ( "Stream1" ) 
#define CAMERA2_CONFIGURATION_TAG ( "Camera2" ) 
#define STREAM2_CONFIGURATION_TAG ( "Stream2" ) 
#define GENERAL_CONFIGURATION_TAG ( "GeneralParams" ) 



int StoreConfiguration( PvDevice *aDevice, PvStream *aStream, bool camera = true );


PvDevice *RestoreDevice( bool camera = true );

PvStream *RestoreStream( bool camera = true );

PvPropertyList *RestoreGeneralParams();

#endif


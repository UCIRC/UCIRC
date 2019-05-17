#ifndef CONFIGURATIONREADER_H
#define CONFIGURATIONREADER_H

#include "Init.h"
 
//FileName of the Config File 
#define CONFIG_FILE ( "lib/Configuration.pvxml" )
#define PARAM_FILE  ( "lib/Configuration.pvcfg" )
#define CAMERA_1_FILE ( "lib/Camera1.pvxml" )
#define CAMERA_2_FILE ( "lib/Camera2.pvxml" ) 
#define CAMERA1_CONFIGURATION_TAG ( "Camera1" ) 
#define STREAM1_CONFIGURATION_TAG ( "Stream1" ) 
#define CAMERA2_CONFIGURATION_TAG ( "Camera2" ) 
#define STREAM2_CONFIGURATION_TAG ( "Stream2" )
#define DEVICE_CONFIGURATION_TAG ( "Camera" )
#define STREAM_CONFIGURATION_TAG ( "Stream" ) 
#define GENERAL_CONFIGURATION_TAG ( "GeneralParams" ) 



int StoreConfiguration( PvDevice *aDevice, PvStream *aStream, bool camera = true );


PvDevice *RestoreDevice( bool camera = true );

PvStream *RestoreStream( bool camera = true );

PvPropertyList *RestoreGeneralParams();
#endif


#ifndef CAMERA_H
#define CAMERA_H

#include "Init.h"
#include "ConfigurationReader.h"
#include "BackupConnection.h"

struct Camera {
	PvString Name;
	PvDevice *Device;
	PvStream *Stream;
	const PvDeviceInfo *DeviceInfo;
	PvPipeline *Pipeline;
} ;

Camera *NewCamera( PvString aName );
bool SetupCamera( Camera *aCamera, bool select, int64_t aBufferCount );
void FreeCameraInfo( Camera *aCamera );

#endif

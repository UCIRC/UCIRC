#ifndef CAMERA_H
#define CAMERA_H

#include "Init.h"
#include "ConfigurationReader.h"
#include "BackupConnection.h"
#include <PvGenParameterArray.h>

PvDevice *ConnectToDevice( const PvString &aConnectionID );
PvStream *OpenStream( const PvString &aConnectionID );
PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream, int64_t aBufferCount );
void clean_up( PvDevice *lCamera, PvStream *lStream );
int disconnect( PvDevice *lCamera );
int close_stream ( PvStream *lStream );
void ConfigureStream( PvDevice *aDevice, PvStream *aStream );

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

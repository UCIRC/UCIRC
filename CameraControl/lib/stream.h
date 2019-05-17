#ifndef STREAM_H
#define STREAM_H
#include "Init.h"

PvStream *open_stream( const PvString *lCameraInfo );

int close_stream( PvStream *lStream );

void ConfigureStream( PvDevice *aDevice, PvStream *aStream );

#endif

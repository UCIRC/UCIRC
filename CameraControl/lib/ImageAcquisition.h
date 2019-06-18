#ifndef IMAGEACQUISITION_H
#define IMAGEACQUISITION_H

#include "Init.h"
#include "Camera.h"
#include <ctime>
#include <time.h>
#include <PvSampleUtils.h>

static std::time_t time_now = std::time(nullptr);

PvPipeline *CreatePipeline( PvDevice *aDevice, PvStream *aStream );

bool AcquireImages( Camera *aCamera, string Filename, int64_t ImageCount );

#endif

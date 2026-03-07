#ifndef _ZLIB_USE_H
#define _ZLIB_USE_H

#include "zlib.h"

int gzDecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);

#endif

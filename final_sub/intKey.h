#ifndef _INTKEY_H
#define _INTKEY_H

#include "mtm_map/map.h"

MapKeyElement copyKeyInt(MapKeyElement n);

void freeInt(MapKeyElement n);

int compareInts(MapKeyElement n1, MapKeyElement n2);

#endif // _INTKEY_H

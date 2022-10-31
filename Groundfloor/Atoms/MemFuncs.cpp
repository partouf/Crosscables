
#include "MemFuncs.h"
#include <cstring>

void Groundfloor::nullifyMem( void *p, unsigned int count ) {
    memset( p, 0, count );
}

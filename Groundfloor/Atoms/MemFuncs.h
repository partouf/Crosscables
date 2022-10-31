
#ifndef __GFMemFuncs_H
#define __GFMemFuncs_H

#include <cstdlib>

// To use the system defaults directly, use these defines
#define GFMalloc(a) malloc(a)
#define GFFree(a) free(a)
#define GFRealloc(a,c) realloc(a,c)


namespace Groundfloor {
	void nullifyMem(void *p, unsigned int count);
}

#endif //__GFMemFuncs_H



#include "Freeable.h"

int Groundfloor::Freeable::DebugString(char *buffer, long bufferlen) {
	if (bufferlen == 0) {
		return 128;
	}
	else if (bufferlen > 128) {
		sprintf(buffer, "%s@%016x\0", this->classname(), reinterpret_cast<unsigned int>(this));
	}
	else {
		return 0;
	}

	return strlen(buffer);
}

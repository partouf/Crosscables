
#ifndef __GFFileCommunicator_H
#define __GFFileCommunicator_H

namespace Groundfloor {
	class FileCommunicator;
}

#include "../Molecules/BaseCommunicator.h"
#include "../Atoms/Lockable.h"
#include "../Molecules/Property.h"

#include <cstdio>

#define GFFILEMODE_READ    1
#define GFFILEMODE_WRITE   2
#define GFFILEMODE_APPEND  4

namespace Groundfloor {

	/** Communicator implementation for Files
	 */
	class FileCommunicator : public BaseCommunicator {
	protected:
		FILE *fp;
		Lockable lockReadWrite;

	public:
		FileCommunicator();
		~FileCommunicator();

		/// filename is assumed to be UTF-8
		StringProperty filename;
		Property<int> mode;
		Property<bool> flushdirectly;

		/// opens file
		bool connect();
		/// closes file
		bool disconnect();

		/// writes data to file
		bool send(const String *sData, CommReturnData *errData = NULL);
		/// reads data from file (sData->getSize() is used as bufferlength)
		bool receive(String *sData, CommReturnData *errData = NULL);
	};
}

#endif // __GFFileCommunicator_H

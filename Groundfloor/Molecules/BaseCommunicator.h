
#ifndef __GFBaseCommunicator_H
#define __GFBaseCommunicator_H

#include "../Atoms/Defines.h"
#include "../Atoms/Freeable.h"
#include "../Molecules/String.h"

namespace Groundfloor {

	/** Basic error and statistics information that can be utilized when
	  * sending and receiving data.
	  */
	class CommReturnData : public Freeable {
	public:
		CommReturnData();
		~CommReturnData();

		bool eof;
		bool error;
		int errorcode;
		__int64 affected;
	};

	/** TGFBaseCommunicator is intended to be a base class for stream-oriented communication classes.
	 */
	class BaseCommunicator : public Freeable {
	protected:
		bool bConnected;

	public:
		/// initializes object, sets connection status to false
		BaseCommunicator();
		/// automatically disconnects before destruction
		~BaseCommunicator();

		/// returns and/or checks whether or not the communicator is still connected
		virtual bool isConnected();

		/// connect (open) the stream, returns false if the attempt failed
		virtual bool connect() = 0;
		/// disconnect (close) the stream
		virtual bool disconnect() = 0;

		/// sends data (as binary string), returns false if attempt failed, fills errData when object is given
		virtual bool send(const String *sData, CommReturnData *errData = NULL) = 0;
		/// receives data (as binary string), returns false if attempt failed, fills errData when object is given
		///  potentially overwrites contents of given data string (sData->getSize() is used as bufferlength)
		virtual bool receive(String *sData, CommReturnData *errData = NULL) = 0;
	};

}

#endif // __GFBaseCommunicator_H

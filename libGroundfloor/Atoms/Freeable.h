
#ifndef __GFFreeable_H_
#define __GFFreeable_H_
/***************************************************************************/

#include <cstdio>
#include <cstring>

namespace Groundfloor {

	/** TGFFreeable adds a virtual destructor, so that when
	 *  an inherited class is created, it can still be destroyed by calling
	 *  delete (TGFFreeable *)object;
	 */
	class Freeable {
	private:
		Freeable& operator=(const Freeable* s); // not allowed to make deep copies
	protected:
		virtual const char* classname() { return "Freeable"; }
	public:
		Freeable() {};
		/** a default virtual destructor with no implementation */
		virtual ~Freeable() {};

		virtual int DebugString(char *buffer, long bufferlen);
	};

}

/***************************************************************************/
#endif // __GFFreeable_H_


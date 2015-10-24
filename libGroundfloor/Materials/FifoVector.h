
#ifndef _GFFifoVector_H
#define _GFFifoVector_H

#include "../Atoms/Lockable.h"
#include "../Molecules/Vector.h"

namespace Groundfloor {

	/** TGFFifoVector is an extension to TGFVector to
	 *  be able to loop its own objects with a fixed vector size.
	 *  It uses TGFLockable objects to be thread-safe.
	 */
	class FifoVector : public Vector {
	protected:
		Lockable aReadLock;
		Lockable aWriteLock;
		bool bWaitingForSpace;

		int iCurrent;
		int iLastAdded;

		void init();
	public:
		FifoVector();
		FifoVector(unsigned int iPrealloc);
		~FifoVector();

		/// Checks whether or not there are still objects to pop-off the vector
		bool isEmpty();

		/// Checks whether or not the vector is full.
		/** If this is true, it might be wise to pop() something off of the vector instead of adding something. */
		bool isFull();

		/// Push an object onto the vector.
		/** If the vector is full, it will wait until there has been a pop() */
		void push(Freeable *obj);
		/// Pop an object off that was pushed first onto the vector.
		Freeable *pop();
		/// Retreive the object that is next to Pop off the vector, but only returns it and does not remove it from the vector.
		Freeable *trypop();
	};
}

#endif // _GFFifoVector_H

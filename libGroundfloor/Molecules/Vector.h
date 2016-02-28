//---------------------------------------------------------------------------

#ifndef GFVectorH
#define GFVectorH
//---------------------------------------------------------------------------

#include "../Atoms/Freeable.h"
#include "../Atoms/Defines.h"


namespace Groundfloor {

	/** TGFVector is a dynamic array with autoresize behaviour.
	 *
	 *  Note:
	 *  In every function with an index reference, there is a check if the index
	 *  is within bounds of the vector. If you don't want the vector to check this,
	 *  you can compile the class with the define GFVECTOR_NOINDEXCHECK.
	 */
	class Vector: public Freeable {
	protected:
	   Freeable **paVector;
	   unsigned int iAllocated;
	   unsigned int iElementCount;

	   unsigned int iPreAllocateCount;

	   unsigned int iRemovedCount;
   
	   unsigned int ptrsize;

	public:
	   /// To enable/disable the ability to insert an object on insertSomewhere() without checking if the removeElement() function was used before.
	   /** The default on creation is False. */
	   bool forceInsert;
	   /// To enable/disable the behaviour of auto-deleting all objects in the array on deletion.
	   /** The default on creation is True. */
	   bool autoClear;

	   /// Default constructor, sets Preallocation count on 1 and initializes the array with 0 objects.
	   Vector();
	   /// Constructor with preallocation count argument.
	   /** If a memory-remap is scheduled on addElement()
		* or insert functions, the size of the array is incremented by this Preallocation count.
		* This is intended as a speedup feature, use 1 to be more memory efficient.
		*/
	   Vector( unsigned int iPreAlloc );
	   /// Destructor, frees memory and optionally deletes all containing objects.
	   ~Vector();

      void setPreAlloc(unsigned int iPreAlloc);

	   /// Adds an object to the end of the array, resizing if necessary.
	   int addElement(Freeable *pElement );
	   /// Inserts the pointer to pElement at the first position that is not occupied by another element.
	   /** Resizes the vector if necessary. */
	   int insertSomewhere(Freeable *pElement );
	   /// Inserts the pointer to pElement at position iAt, shifting 0 or more elements to their index +1.
	   /** Resizes the vector if necessary. */
	   int insertAt( unsigned int iAt, Freeable *pElement );

	   /// Returns a pointer to the object that is at index i.
	   Freeable *elementAt( unsigned int i ) const;

	   /// Removes the element at index i by replacing it by a NULL-value and returns the removed object pointer.
	   /** The destructor of the element will not be called by this function. */
	   Freeable *removeElement( unsigned int i );
	   /// Replaces the element at index i by pElement and returns the object pointer that was replaced.
	   Freeable *replaceElement( unsigned int i, Freeable *pElement );

	   /// Removes (and returns) the first element in the vector, and shifts 2 to size() to the front
	   Freeable *removeFirstElementAndShiftToLeft();

	   /// Swaps the element at index i with the element at index j
	   void swapElements( unsigned int i, unsigned int j );

	   /// Removes the element obj by replacing it by a NULL-value.
	   /** The destructor of the element will not be called by this function. */
	   void removeElement( const Freeable *pElement );

	   /// Remaps the used memory for the dynamic array, truncating the array if its new size is smaller than the original size.
	   bool resizeVector( unsigned int size );
	   /// Calls the destructors of every remaining object in the array, and sets the array size to 0.
	   void clear();

	   /// Returns the index of pElement in the dynamic array. If the object was not found, -1 is returned.
	   int findElement( const Freeable *pElement ) const;

	   /// Returns the element count of the array. Removed/nilled elements are not subtracted from this count.
	   unsigned int size() const;
	   /// Returns the ammount of elements the array can contain at the moment.
	   unsigned int allocated() const;

	   /// Compresses the array by element shifting and reallocation.
	   /** Shifts out removed or nilled elements from the array and resets the element count
		* and finally remaps the array to a more enclosed allocation.
		*/
	   void compress();

	   /// Copies the used array to a newly allocated piece of memory that is assigned to the given vector.
	   /** Replaces the old mapping without calling the destructors of the old elements. */
	   bool fastCopy( Vector *into );

	};

}

//---------------------------------------------------------------------------
#endif

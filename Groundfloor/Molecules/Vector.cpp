

#include "Vector.h"

#include <cstdlib>

#ifdef __BORLANDC__
	#include <mem.h>
#else
	#include <memory.h>
#endif

#ifndef min
   #define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
	#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif


//---------------------------------------------------------------------------


Groundfloor::Vector::Vector() : Groundfloor::Freeable() {
   this->iPreAllocateCount = 1;

   this->paVector = NULL;
   this->iAllocated = 0;

   this->iElementCount = 0;

   this->autoClear = true;       // clear and Delete objects in vector at Free
   this->forceInsert = false;    // look for empty spots at insert

   this->iRemovedCount = 0;

   // bug: length of pointer was assumed to be 4
   // in i386 this returns 4 bytes, but in x86_64 it returns 8 bytes
   this->ptrsize = sizeof( void * );
}

Groundfloor::Vector::Vector( unsigned int iPreAlloc ) : Freeable() {
   this->iPreAllocateCount = iPreAlloc;

   this->paVector = NULL;
   this->iAllocated = 0;

   this->iElementCount = 0;

   this->autoClear = true;       // clear and Delete objects in vector at Free
   this->forceInsert = false;    // look for empty spots at insert

   this->iRemovedCount = 0;

   this->ptrsize = sizeof( void * );
}

Groundfloor::Vector::Vector(const Vector & AnotherVector) : Freeable() {
	this->iPreAllocateCount = AnotherVector.iPreAllocateCount;

	this->paVector = NULL;
	this->iAllocated = 0;

	this->iElementCount = AnotherVector.iElementCount;

	this->autoClear = AnotherVector.autoClear;
	this->forceInsert = AnotherVector.forceInsert;

	this->iRemovedCount = AnotherVector.iRemovedCount;

	this->ptrsize = sizeof(void *);

	this->resizeVector(AnotherVector.iAllocated);
	memcpy(this->paVector, AnotherVector.paVector, this->iAllocated);
}

void Groundfloor::Vector::setPreAlloc(unsigned int iPreAlloc) {
   this->iPreAllocateCount = iPreAlloc;
}

Groundfloor::Vector::~Vector() {
   if ( this->paVector != NULL ) {
      if ( this->autoClear ) {
         clear();
      }

      GFFree( this->paVector );
   }
}

void Groundfloor::Vector::clear() {
   for ( unsigned int i = 0; i < this->iElementCount; i++ ) {
      if ( this->paVector[i] != NULL ) {
         Freeable *obj = this->paVector[i];
         this->paVector[i] = NULL;
         delete obj;
      }
   }

   // niet vertex array zelf freeen, scheelt tijd in alloceren

   this->iElementCount = 0;
}

bool Groundfloor::Vector::resizeVector( unsigned int size ) {
   // alloceer nieuwe array
   void *pNewArr = GFMalloc( size * ptrsize );

   if ( pNewArr != NULL ) {
      Groundfloor::nullifyMem( pNewArr, size * ptrsize );

      unsigned int iOldCount = this->iElementCount;
      this->iElementCount = 0;
      void *pOldArr = this->paVector;

      if ( pOldArr != NULL ) {
         // kopieer eventuele eerder toegevoegde elementen
         iOldCount = min( size, iOldCount );
         memcpy( pNewArr, this->paVector, iOldCount * ptrsize );
      }

      // de nieuwe array
      this->paVector       = static_cast<Freeable**>(pNewArr);
      this->iElementCount  = iOldCount;
      this->iAllocated     = size;

      GFFree( pOldArr );

      return true;
   }

   // te weinig geheugen oid
   return false;
}


int Groundfloor::Vector::addElement( Freeable *pElement ) {
   if ( this->iAllocated <= this->iElementCount ) {
      // automatisch expanden indien array te klein is
      if ( !resizeVector( this->iAllocated + this->iPreAllocateCount ) ) {
         return -1;
      }
   }

   this->paVector[iElementCount] = pElement;   // adres kopieren
   this->iElementCount++;

   return iElementCount - 1;
}


int Groundfloor::Vector::insertSomewhere( Freeable *pElement ) {
   if ( (this->iRemovedCount > 0) || (this->forceInsert) ) {
      for ( unsigned int i = 0; i < this->iElementCount; i++ ) {
         if ( this->paVector[i] == NULL ) {
            this->paVector[i] = pElement;

            if ( this->iRemovedCount > 0 ) {
               this->iRemovedCount--;
            }

            return i;
         }
      }
   }

   return addElement( pElement );
}


int Groundfloor::Vector::insertAt( unsigned int iAt, Freeable *pElement ) {
   Freeable *tmpElemB = NULL;
   Freeable *tmpElemA = reinterpret_cast<Freeable *>(-1);    // is om niet NULL te zijn, thats all (negeer idiote compiler msgs)

#ifndef GFVECTOR_NOINDEXCHECK
   if ( iAt < this->iElementCount ) {
#endif

      if ( this->paVector[iAt] == NULL ) {
         this->paVector[iAt] = pElement;
      } else {

         unsigned int i = iAt;
         tmpElemB = pElement;
         while ( (tmpElemA != NULL) && (i < this->iElementCount) ) {
            tmpElemA = this->paVector[i];
            this->paVector[i] = tmpElemB;
            tmpElemB = tmpElemA;
            i++;
         }

         if ( tmpElemB != NULL ) {
            addElement( tmpElemB );
         }
      }

      if ( this->iRemovedCount > 0 ) {
         this->iRemovedCount--;
      }

      return iAt;

#ifndef GFVECTOR_NOINDEXCHECK
   } else {
      // just insert at end
      return addElement( pElement );
   }
#endif
}


Groundfloor::Freeable *Groundfloor::Vector::elementAt( unsigned int i ) const {
#ifndef GFVECTOR_NOINDEXCHECK
   if ( i < this->iElementCount ) {
#endif

      return this->paVector[i];

#ifndef GFVECTOR_NOINDEXCHECK
   }
   return NULL;
#endif
}


// zelf deleten aub indien dit gebruikt wordt! etc.
Groundfloor::Freeable *Groundfloor::Vector::removeElement( unsigned int i ) {
   Freeable *ret = NULL;

#ifndef GFVECTOR_NOINDEXCHECK
   if ( i < this->iElementCount ) {
#endif
      if ( this->paVector[i] != NULL ) {
         ret = this->paVector[i];
         this->paVector[i] = NULL;

         this->iRemovedCount++;

// hier kan niet meer vanuit worden gegaan vanwege de vele manieren
//  van verwijderen en toevoegen van elementen.
/*
         if ( this->iRemovedCount == this->iElementCount ) {
            // reset pointer to if everything's deleted
            this->iElementCount = 0;
         }
*/
      }
#ifndef GFVECTOR_NOINDEXCHECK
   }
#endif

   return ret;
}

Groundfloor::Freeable *Groundfloor::Vector::removeFirstElementAndShiftToLeft() {
   Freeable *ret = this->paVector[0];

   memcpy(this->paVector, this->paVector + 1, (this->iElementCount - 1) * ptrsize );
   this->paVector[this->iElementCount-1] = NULL;
   this->iElementCount--;

   return ret;
}

// element vervangen op positie i, met object obj - huidige element dat vervangen is wordt teruggegeven
Groundfloor::Freeable *Groundfloor::Vector::replaceElement( unsigned int i, Freeable *pElement ) {
   Freeable *ret = NULL;

#ifndef GFVECTOR_NOINDEXCHECK
   if ( i < this->iElementCount ) {
#endif
      ret = this->paVector[i];

      this->paVector[i] = pElement;
#ifndef GFVECTOR_NOINDEXCHECK
   }
#endif

   return ret;
}

void Groundfloor::Vector::swapElements( unsigned int i, unsigned int j ) {
	Groundfloor::Freeable *a;

   unsigned int c = this->iElementCount;
#ifndef GFVECTOR_NOINDEXCHECK
   if ( ( i < c ) && ( j < c ) ) {
#endif
      a = this->paVector[i];

      this->paVector[i] = this->paVector[j];
      this->paVector[j] = a;
#ifndef GFVECTOR_NOINDEXCHECK
   }
#endif
}

void Groundfloor::Vector::removeElement( const Freeable *pElement ) {
   for ( unsigned int i = 0; i < this->iElementCount; i++ ) {
      if ( this->paVector[i] == pElement ) {
         this->paVector[i] = NULL;

         this->iRemovedCount++;


// hier kan niet meer vanuit worden gegaan vanwege de vele manieren
//  van verwijderen en toevoegen van elementen.
/*
         if ( this->iRemovedCount == this->iElementCount ) {
            // reset pointer to if everything's deleted
            this->iElementCount = 0;
         }
*/
         break;
      }
   }
}

unsigned int Groundfloor::Vector::size() const {
   return iElementCount;
}

unsigned int Groundfloor::Vector::allocated() const {
   return iAllocated;
}

int Groundfloor::Vector::findElement( const Freeable *pElement ) const {
   for ( unsigned int i = 0; i < this->iElementCount; i++ ) {
      if ( this->paVector[i] == pElement ) {
         return i;
      }
   }

   return -1;
}

/* Compress()
 *  compress total vector size by removing NULL entries
 */
void Groundfloor::Vector::compress() {
   unsigned int deleted = 0;

   for ( unsigned int i = 0; i < this->iElementCount; i++ ) {
      if ( this->paVector[i] != NULL ) {
         this->paVector[i-deleted] = this->paVector[i];
      } else {
         deleted++;
      }
   }

   this->iRemovedCount = 0;
   this->iElementCount -= deleted;
   resizeVector( this->iElementCount );
}

bool Groundfloor::Vector::fastCopy(Vector *into ) {
   void *pNewArr = GFMalloc( this->iElementCount * ptrsize );

   if ( pNewArr != NULL ) {
      into->iElementCount = 0;
      void *pOldArr = into->paVector;

      // kopieer naar nieuwe vector
      if ( this->paVector != NULL ) {
         memcpy( pNewArr, this->paVector, this->iElementCount * ptrsize );
      }

      into->paVector       = static_cast<Groundfloor::Freeable**>(pNewArr);
      into->iElementCount  = this->iElementCount;
      into->iAllocated     = this->iElementCount;

      // free huidige vector
      GFFree( pOldArr );

      return true;
   }

   // te weinig geheugen oid
   return false;
}


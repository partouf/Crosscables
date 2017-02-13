
#include "StringLookup.h"


//------------------------------------------------------------------------------------

Groundfloor::SimpleStringLookupObject::SimpleStringLookupObject(const char *sName, Groundfloor::Freeable *anObject) : Groundfloor::Freeable() {
   name.setValue_ansi(sName);
   object = anObject;
}


Groundfloor::SimpleStringLookupObject::SimpleStringLookupObject(const Groundfloor::String *sName, Groundfloor::Freeable *anObject) : Groundfloor::Freeable() {
   name.setValue(sName);
   object = anObject;
}

Groundfloor::SimpleStringLookupObject::~SimpleStringLookupObject() {
   // don't delete the object, we don't own it
   object = NULL;
}

//------------------------------------------------------------------------------------

Groundfloor::SimpleStringLookup::SimpleStringLookup() : Groundfloor::Vector() {
}

Groundfloor::SimpleStringLookup::~SimpleStringLookup() {
}

void Groundfloor::SimpleStringLookup::addObjectByString(const char *sName, Groundfloor::Freeable *anObject) {
   this->addElement(new Groundfloor::SimpleStringLookupObject(sName, anObject));
}


void Groundfloor::SimpleStringLookup::insertObjectSortedByLength(const Groundfloor::String *sName, Groundfloor::Freeable *anObject, bool bAscending) {
   unsigned long i, c;
   Groundfloor::SimpleStringLookupObject *obj;
   Groundfloor::String s(sName);

   bool bInserted = false;
   Groundfloor::SimpleStringLookupObject *lobj = new Groundfloor::SimpleStringLookupObject(sName, anObject);

   c = this->iElementCount;
   for (i = 0; i < c; i++) {
      obj = static_cast<Groundfloor::SimpleStringLookupObject *>(paVector[i]);
      if (obj != NULL) {
         if (bAscending) {
            if (s.getLength() <= obj->name.getLength()) {
               this->insertAt(i, lobj);
               bInserted = true;
               break;
            }
         }
         else {
            if (s.getLength() >= obj->name.getLength()) {
               this->insertAt(i, lobj);
               bInserted = true;
               break;
            }
         }
      }
   }

   if (!bInserted) {
      this->addElement(lobj);
   }
}

void Groundfloor::SimpleStringLookup::deleteAndClear()
{
	unsigned long i, c;
	Groundfloor::SimpleStringLookupObject *obj;

	c = this->iElementCount;
	for (i = 0; i < c; i++) {
		obj = static_cast<Groundfloor::SimpleStringLookupObject *>(paVector[i]);
		if (obj != NULL) {
			paVector[i] = NULL;

			delete obj;
		}
	}

	clear();
}

void Groundfloor::SimpleStringLookup::insertObjectSortedByLength(const char *sName, Groundfloor::Freeable *anObject, bool bAscending) {
   unsigned long i, c;
   Groundfloor::SimpleStringLookupObject *obj;
   Groundfloor::String s(sName);

   bool bInserted = false;
   Groundfloor::SimpleStringLookupObject *lobj = new Groundfloor::SimpleStringLookupObject(sName, anObject);

   c = this->iElementCount;
   for (i = 0; i < c; i++) {
      obj = static_cast<Groundfloor::SimpleStringLookupObject *>(paVector[i]);
      if (obj != NULL) {
         if (bAscending) {
            if (s.getLength() <= obj->name.getLength()) {
               this->insertAt(i, lobj);
               bInserted = true;
               break;
            }
         }
         else {
            if (s.getLength() >= obj->name.getLength()) {
               this->insertAt(i, lobj);
               bInserted = true;
               break;
            }
         }
      }
   }

   if (!bInserted) {
      this->addElement(lobj);
   }
}


Groundfloor::Freeable *Groundfloor::SimpleStringLookup::getObjectByString(const Groundfloor::String *sName, bool bCaseSensitive) {
   unsigned long i, c;
   Groundfloor::SimpleStringLookupObject *obj;

   c = this->iElementCount;
   for (i = 0; i < c; i++) {
      obj = static_cast<Groundfloor::SimpleStringLookupObject *>(paVector[i]);
      if (obj != NULL) {
         if (bCaseSensitive) {
             if (sName->match(&(obj->name))) {
                 return obj->object;
             }
         }
         else {
			 if ((obj->name.getLength() == sName->getLength()) && obj->name.startsWith(sName)) {
                 return obj->object;
			 }
         }
         
      }
   }

   return NULL;
}

Groundfloor::Freeable *Groundfloor::SimpleStringLookup::getObjectByString(const char *sName, bool bCaseSensitive) {
   Groundfloor::String tmp;
   tmp.setValue_ansi(sName);

   return getObjectByString(&tmp, bCaseSensitive);
}

void Groundfloor::SimpleStringLookup::removeObjectByString(const char *sName) {
   Groundfloor::String tmp;
   tmp.setValue_ansi(sName);

   unsigned long i, c;
   Groundfloor::SimpleStringLookupObject *obj;

   c = this->iElementCount;
   for (i = 0; i < c; i++) {
      obj = static_cast<Groundfloor::SimpleStringLookupObject *>(paVector[i]);
      if (obj != NULL) {
         if (tmp.match(&(obj->name))) {
            paVector[i] = NULL;

            delete obj;
            break;
         }
      }
   }
}

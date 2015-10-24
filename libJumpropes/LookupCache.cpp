
#include "LookupCache.h"

Jumpropes::LookupBase::LookupBase() : Freeable() {
   sNonValidHostName.setValue_ansi( C_NONVALIDHOSTNAME );
}

Jumpropes::LookupBase::~LookupBase() {
}


Jumpropes::LookupObject *Jumpropes::LookupBase::lookupHost( const String *sHost ) {
   LookupObject *obj = new LookupObject();

   obj->name.setValue( sHost );
   if ( JRresolveAll( obj ) ) {
      return obj;
   }

   delete obj;
   return NULL;
}

bool Jumpropes::LookupBase::lookupHost( LookupObject *obj ) {
   return JRresolveAll( obj );
}

bool Jumpropes::LookupBase::equalsNonValidHostResponse( const LookupObject *obj ) {
   LookupObject *nonValid = lookupHost( &sNonValidHostName );
   return obj->firstAddressEqualsOneOf( nonValid, true );
}

Jumpropes::LookupObject *Jumpropes::LookupBase::getNonValidHostResponseObject() {
    return lookupHost( &sNonValidHostName );
}

//------------------------------------------------------------------------------

Jumpropes::LookupCache::LookupCache( bool bInitNonvalidhostAdress ) : LookupBase() {
   list.autoClear = true;

   if ( bInitNonvalidhostAdress ) {
      lookupHost( &sNonValidHostName );
   }
}

Jumpropes::LookupCache::~LookupCache() {
}

Jumpropes::LookupObject *Jumpropes::LookupCache::findObj( const String *sHost ) {
   lock.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   unsigned int c = list.size();
   for ( unsigned int i = 0; i < c; i++ ) {
      LookupObject *obj = static_cast<LookupObject *>( list.elementAt(i) );

      if ( sHost->match( &obj->name ) ) {
         lock.unlock();
         return obj;
      }
   }

   lock.unlock();

   return NULL;
}

Jumpropes::LookupObject *Jumpropes::LookupCache::lookupHost( const String *sHost ) {
   LookupObject *obj = findObj( sHost );

   if ( obj == NULL ) {
      obj = Jumpropes::LookupBase::lookupHost( sHost );
      if ( obj != NULL ) {
         lock.lockWhenAvailable( GFLOCK_INFINITEWAIT );
         list.addElement( obj );
         lock.unlock();
      }
   }

   return obj;
}

bool Jumpropes::LookupCache::lookupHost( LookupObject *obj ) {
   LookupObject *objCached = findObj( &obj->name );

   if ( objCached == NULL ) {
      if ( Jumpropes::LookupBase::lookupHost( obj ) ) {
         addObject( obj );
      }
   } else {
      obj->setValue( objCached );
      return true;
   }

   return false;
}


void Jumpropes::LookupCache::clear( bool bInitNonvalidhostAdress ) {
   list.clear();

   if ( bInitNonvalidhostAdress ) {
      lookupHost( &sNonValidHostName );
   }
}

void Jumpropes::LookupCache::addObject( const LookupObject *obj ) {
   LookupObject *objCached = new LookupObject();
   objCached->setValue( obj );

   lock.lockWhenAvailable( GFLOCK_INFINITEWAIT );
   list.addElement( objCached );
   lock.unlock();
}

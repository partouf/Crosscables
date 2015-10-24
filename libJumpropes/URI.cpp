
#include "URI.h"

#include <Groundfloor/Bookshelfs/BValue.h>

#ifdef GF_OS_LINUX
   #include <netdb.h>
#endif

//#include "BaseSocket.h"
#include "Functions.h"

const char C_COLON[] = ":";
const char C_AT[] = "@";
const char C_PROTSEP[] = "://";
const char C_SLASH[] = "/";
const char C_QUERY[] = "?";

#define C_STRLEN_COLON     1
#define C_STRLEN_AT        1
#define C_STRLEN_PROTSEP   3
#define C_STRLEN_SLASH     1
#define C_STRLEN_QUERY     1


Jumpropes::URI::URI( LookupBase *pLookupBase ) : Freeable() {
   this->pLookupBase = pLookupBase;
   port.set( 0 );
}

Jumpropes::URI::~URI() {
}

void Jumpropes::URI::useDefaultPort() {
   servent *serv = getservbyname( this->protocol.getValue(), NULL );
   if ( serv != NULL ) {
      this->port.set( ntohs(serv->s_port) );
   }
}

void Jumpropes::URI::parseUri( const String *sUri, bool bClearVars ) {
   String *sTmp1 = new String();
   String *sTmp2 = new String();
   String *sTmp3 = new String();

   sTmp1->setValue( sUri );
   LookupObject *objLookup;

   int pos;
   pos = sTmp1->pos( 0, C_PROTSEP, C_STRLEN_PROTSEP );
   if ( pos != -1 ) {
      this->protocol.setValue( sTmp1->getValue(), pos );

      pos = pos + C_STRLEN_PROTSEP;

      sTmp1->setValue( sTmp1->getPointer( pos ), sTmp1->getLength() - pos );
   } else {
      if ( bClearVars ) {
         this->protocol.setValue( "", 0 );
      }
   }

   pos = sTmp1->pos( 0, C_SLASH, C_STRLEN_SLASH );
   if ( pos != -1 ) {
      sTmp2->setValue( sTmp1->getValue(), pos );

      pos = pos + C_STRLEN_SLASH;
      sTmp1->setValue( sTmp1->getPointer(pos), sTmp1->getLength() - pos );
   } else {
      if ( bClearVars ) {
         this->path.setValue( "", 0 );
      }
      sTmp2->setValue( sTmp1 );
   }

   pos = sTmp2->pos( 0, C_AT, C_STRLEN_AT );
   if ( pos != -1 ) {
      sTmp3->setValue( sTmp2->getValue(), pos );

      pos = pos + C_STRLEN_AT;

      sTmp2->setValue( sTmp2->getPointer(pos), sTmp2->getLength() - pos );

      pos = sTmp3->pos( 0, C_COLON, C_STRLEN_COLON );
      if ( pos != -1 ) {
         this->user.setValue( sTmp3->getValue(), pos );

         pos = pos + C_STRLEN_COLON;
         this->pass.setValue( sTmp3->getPointer(pos), sTmp3->getLength() - pos );
      } else {
         this->user.setValue( sTmp3 );
         this->pass.setValue( "", 0 );
      }

   } else {
      if ( bClearVars ) {
         this->user.setValue( "", 0 );
         this->pass.setValue( "", 0 );
      }
   }

   pos = sTmp2->pos( 0, C_COLON, C_STRLEN_COLON );
   if ( pos != -1 ) {
      Groundfloor::BValue val;
      val.setString( sTmp2->getPointer(pos) + C_STRLEN_COLON, sTmp2->getLength() - pos );
      this->port.set( val.asInteger() );

      sTmp2->setValue( sTmp2->getValue(), pos );
      this->host.name.setValue( sTmp2 );
   } else {
      if ( bClearVars ) {
         this->host.clear();

         if ( pLookupBase != NULL ) {
            objLookup = pLookupBase->lookupHost( sTmp2 );
            if ( objLookup != NULL ) {
               if ( pLookupBase->equalsNonValidHostResponse( objLookup ) ) {
                  this->host.name.setValue( "", 0 );
               } else {
                  this->host.setValue( objLookup );
               }
            } else {
               this->host.name.setValue( "", 0 );
            }
         } else {
            if ( JRresolveIP( sTmp2, &this->host, true ) ) {
               this->host.name.setValue( sTmp2 );
            } else {
               this->host.name.setValue( "", 0 );
            }
         }

         useDefaultPort();
      } else {
         if ( pLookupBase != NULL ) {
            objLookup = pLookupBase->lookupHost( sTmp2 );
            if ( objLookup != NULL ) {
               if ( !pLookupBase->equalsNonValidHostResponse( objLookup ) ) {
                  this->host.setValue( objLookup );
               }
            }
         } else {
            if ( JRresolveIP( sTmp2, &this->host, true ) ) {
               this->host.name.setValue( sTmp2 );
            }
         }
      }
   }

   pos = sTmp1->pos( 0, C_QUERY, C_STRLEN_QUERY );
   if ( pos != -1 ) {
      int pos2 = pos;
      pos = pos + C_STRLEN_QUERY;
      this->query.setValue( sTmp1->getPointer(pos), sTmp1->getLength() - pos );

      sTmp1->setValue( sTmp1->getValue(), pos2 );
   } else {
      this->query.setValue( "", 0 );
   }

   this->path.setValue( sTmp1 );

   delete sTmp3;
   delete sTmp2;
   delete sTmp1;
}

void Jumpropes::URI::parseUri( const char *sUri, bool bClearVars ) {
   String *s = new String( sUri );
   parseUri( s, bClearVars );
   delete s;
}

String *Jumpropes::URI::getUri() {
   String *sUri = new String();

   if ( this->protocol.getLength() > 0 ) {
      sUri->append( &this->protocol );
      sUri->append( C_PROTSEP, C_STRLEN_PROTSEP );
   }

   if ( this->user.getLength() > 0 ) {
      sUri->append( &this->user );

      if ( this->pass.getLength() > 0 ) {
         sUri->append( C_COLON, C_STRLEN_COLON );
         sUri->append( &this->pass );
      }

      sUri->append( C_AT, C_STRLEN_AT );
   }

   if ( this->host.name.getLength() > 0 ) {
      sUri->append( &this->host.name );
   }

   if ( this->port.get() > 0 ) {
      sUri->append( C_COLON, C_STRLEN_COLON );

      Groundfloor::BValue val;
      val.setInteger( this->port.get() );
      sUri->append( val.asString() );
   }

   if ( this->path.getLength() > 0 ) {
      sUri->append( C_SLASH, C_STRLEN_SLASH );
      sUri->append( &this->path );
   }

   if ( this->query.getLength() > 0 ) {
      sUri->append( C_QUERY, C_STRLEN_QUERY );
      sUri->append( &this->query );
   }

   return sUri;
}

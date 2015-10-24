
#include "NetAddress.h"

#ifdef GF_OS_LINUX
   #include <sys/socket.h>
#endif

#ifdef GF_CP_MVC
   #define AF_UNSPEC 0
#endif

//------------------------------------------------------------------------------

Jumpropes::IPAddress *Jumpropes::LookupObject::getAddress( unsigned int i ) const {
   return static_cast<IPAddress *>( elementAt(i) );
}

void Jumpropes::LookupObject::setValue( const LookupObject *obj ) {
   this->name.setValue( &obj->name );

   this->clear();
   unsigned int c = obj->size();
   for ( unsigned int i = 0; i < c; i++ ) {
      IPAddress *oldAddr = obj->getAddress( i );
      IPAddress *newAddr = new IPAddress();
      newAddr->setValue( oldAddr );
      this->addElement(newAddr);
   }
}


bool Jumpropes::LookupObject::firstAddressEqualsOneOf( const LookupObject *obj, bool bBothNullIsWin ) const {
   IPAddress *first = getAddress();
   if ( obj != NULL ) {
      if ( first != NULL ) {
         unsigned int c = obj->size();
         for ( unsigned int i = 0; i < c; i++ ) {
            if ( first->equals( obj->getAddress(i) ) ) {
               return true;
            }
         }
      } else {
         if ( ( obj->size() == 0 ) && bBothNullIsWin ) {
            return true;
         }
      }
   } else {
      return ( first == NULL );
   }

   return false;
}

//------------------------------------------------------------------------------

#include <cstring>

Jumpropes::IPAddress::IPAddress() : Freeable() {
   family = AF_UNSPEC;
   ip.setValue( "", 0 );
   memset( this->addressbytes, 0, 16 );
}

void Jumpropes::IPAddress::setValue( const IPAddress *addr ) {
   this->family = addr->family;
   this->ip.setValue( &addr->ip );
   memcpy( this->addressbytes, addr->addressbytes, 16 );
}

bool Jumpropes::IPAddress::equals( const IPAddress *addr ) {
   return
      (
         ( this->family == addr->family ) &&
         ( this->ip.match( &addr->ip ) )
      );
}

in_addr *Jumpropes::IPAddress::getIPv4AddressPtr() {
   return reinterpret_cast<in_addr *>( &addressbytes );
}

in6_addr *Jumpropes::IPAddress::getIPv6AddressPtr() {
   return reinterpret_cast<in6_addr *>( &addressbytes );
}

void Jumpropes::IPAddress::setAsIPv4Address( in_addr *address ) {
   memset( this->addressbytes, 0, 16 );
   memcpy( this->addressbytes, address, 4 );
}

void Jumpropes::IPAddress::setAsIPv6Address( in6_addr *address ) {
   memcpy( this->addressbytes, address, 16 );
}

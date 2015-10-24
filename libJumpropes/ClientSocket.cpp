
#include "ClientSocket.h"

#include <Groundfloor/Atoms/Defines.h>

#include "Functions.h"
#ifdef GF_OS_WIN32
        #include <winsock2.h>
        #include <ws2tcpip.h>
#else
        #include <arpa/inet.h>
        #include <netdb.h>

		  #ifdef GF_OS_LINUX
		  	  #include <cstring>
		  #endif
#endif


#include <errno.h>


Jumpropes::ClientSocket::ClientSocket() : BaseSocket() {
}

Jumpropes::ClientSocket::~ClientSocket() {
}

bool Jumpropes::ClientSocket::connect() {
   IPAddress *address = getRemoteAddress();
	this->iAddressType = address->family;

   if ( !BaseSocket::connect() ) {
      return false;
   }

   setBlocking( true );

	int iStatus = -1;

	char *sIpAddr = address->ip.getValue();
   if ( strlen(sIpAddr) > 0 ) {
      if ( address->family == AF_INET ) {
         struct sockaddr_in aSocketAddressIPv4;

         memset( &aSocketAddressIPv4, 0, sizeof( struct sockaddr_in ) );
         aSocketAddressIPv4.sin_family    = address->family;
         aSocketAddressIPv4.sin_port      = htons( remotePort.get() );
         aSocketAddressIPv4.sin_addr      = *address->getIPv4AddressPtr();

         iStatus = ::connect( iSocketId, reinterpret_cast<sockaddr *>( &aSocketAddressIPv4 ), sizeof( struct sockaddr_in ) );
      } else if ( address->family == AF_INET6 ) {
         struct sockaddr_in6 aSocketAddressIPv6;

         memset( &aSocketAddressIPv6, 0, sizeof( struct sockaddr_in6 ) );
         aSocketAddressIPv6.sin6_family   = address->family;
         aSocketAddressIPv6.sin6_port     = htons( remotePort.get() );
         aSocketAddressIPv6.sin6_addr     = *address->getIPv6AddressPtr();

         iStatus = ::connect( iSocketId, reinterpret_cast<sockaddr *>( &aSocketAddressIPv6 ), sizeof( struct sockaddr_in6 ) );
      }
   }

   setBlocking( false );

	this->bConnected = ( iStatus == 0 );
	if ( bConnected ) {
		loadLocalAddressInfo();
		loadRemoteAddressInfo();
	}

	return this->bConnected;
}


#include "SSLClientSocket.h"
#include <Groundfloor/Atoms/Defines.h>

#ifdef GF_OS_WIN32
   #include <winsock2.h>
   #include <ws2tcpip.h>
#else
   #include <netdb.h>
   #include <unistd.h>
   #include <fcntl.h>
   #include <arpa/inet.h>
   #include <sys/types.h>

   #include <sys/select.h>

   #include <cstring>
#endif


#include <openssl/err.h>

Jumpknots::SSLClientSocket::SSLClientSocket() : SSLBaseSocket() {
   this->method = SSLv23_client_method();
   this->ctx = SSL_CTX_new( this->method );

   iVerifyResult = -1;
}

Jumpknots::SSLClientSocket::~SSLClientSocket() {
}

bool Jumpknots::SSLClientSocket::connect() {
   IPAddress *address = getRemoteAddress();
	this->iAddressType = address->family;

   if ( !SSLBaseSocket::connect() ) {
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

//printf( "IPv4 connect (%s port %d) %x\n", sIpAddr, remotePort.get(), *address->getIPv4AddressPtr() );

         iStatus = ::connect( iSocketId, reinterpret_cast<sockaddr *>( &aSocketAddressIPv4 ), sizeof( struct sockaddr_in ) );
      } else if ( address->family == AF_INET6 ) {
         struct sockaddr_in6 aSocketAddressIPv6;

//printf( "IPv6 connect\n" );
         memset( &aSocketAddressIPv6, 0, sizeof( struct sockaddr_in6 ) );
         aSocketAddressIPv6.sin6_family   = address->family;
         aSocketAddressIPv6.sin6_port     = htons( remotePort.get() );
         aSocketAddressIPv6.sin6_addr     = *address->getIPv6AddressPtr();

         iStatus = ::connect( iSocketId, reinterpret_cast<sockaddr *>( &aSocketAddressIPv6 ), sizeof( struct sockaddr_in6 ) );
      }
   }

	if ( iStatus == 0 ) {
      this->ssl = SSL_new( this->ctx );
      SSL_set_fd( this->ssl, this->iSocketId );

      iStatus = SSL_connect( this->ssl );
      if ( iStatus != 1 ) {
         iStatus = -1;
      } else {
         iStatus = 0;
      }

      iVerifyResult = SSL_get_verify_result( this->ssl );
      // printf( "ssl verify result: %d\n",  );
   }

   setBlocking( false );

	this->bConnected = ( iStatus == 0 );
	if ( bConnected ) {
		loadLocalAddressInfo();
		loadRemoteAddressInfo();
   }

	return this->bConnected;
}

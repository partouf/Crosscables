
#include "BaseSocket.h"
#include "Functions.h"


Jumpropes::BaseSocket::BaseSocket() : BaseCommunicator() {
   resetVars();
}

Jumpropes::BaseSocket::BaseSocket( int iSocketId, const BaseSocket *aServer ) : BaseCommunicator() {
   resetVars();

	this->iSocketId = iSocketId;

	if ( this->iSocketId != -1 ) {
		loadLocalAddressInfo();
		loadRemoteAddressInfo();
	}
}

void Jumpropes::BaseSocket::resetVars() {
   iConnectionType   = JR_CONNECTIONTYPE_TCP;
   iAddressType      = AF_INET;

   aRemoteAddress.family = AF_UNSPEC;
   aRemoteAddress.ip.setValue( "", 0 );
   aLocalAddress.family = AF_UNSPEC;
   aLocalAddress.ip.setValue( "", 0 );
	localPort.set( -1 );
	remotePort.set( -1 );

	autosplit_sendbuffer.set( true );
	autosplit_size.set( 1024 );

	iSocketId = -1;
   bConnected = false;
}

Jumpropes::BaseSocket::~BaseSocket() {
	disconnect();
}

void Jumpropes::BaseSocket::reinitialize( int iSocketId ) {
	disconnect();

	this->iSocketId = iSocketId;

	if ( this->iSocketId != -1 ) {
		loadLocalAddressInfo();
		loadRemoteAddressInfo();
	}
}

bool Jumpropes::BaseSocket::loadLocalAddressInfo() {
	sockaddr_storage anAddress;
	socklen_t iAddrLen = sizeof(anAddress);
	int iStatus = -1;

	iStatus = getsockname( iSocketId, reinterpret_cast<sockaddr *>( &anAddress ), &iAddrLen );
	if ( iStatus != -1 ) {
      sockaddr_in6 *tmp = reinterpret_cast<sockaddr_in6 *>( &anAddress );  // sin_port and sin6_port are at the same position in both structs
      localPort.set( ntohs( tmp->sin6_port ) );

      aLocalAddress.ip.setLength( INET6_ADDRSTRLEN );
      aLocalAddress.family = anAddress.ss_family;
      if ( aLocalAddress.family == AF_INET ) {
         aLocalAddress.setAsIPv4Address( &reinterpret_cast<sockaddr_in *>(&anAddress)->sin_addr );
         inet_ntop( AF_INET, aLocalAddress.getIPv4AddressPtr(), aLocalAddress.ip.getPointer(0), INET_ADDRSTRLEN );
      } else if ( aLocalAddress.family == AF_INET6 ) {
         aLocalAddress.setAsIPv6Address( &reinterpret_cast<sockaddr_in6 *>(&anAddress)->sin6_addr );
         inet_ntop( AF_INET6, aLocalAddress.getIPv6AddressPtr(), aLocalAddress.ip.getPointer(0), INET6_ADDRSTRLEN );
      }
      aLocalAddress.ip.setLength( strlen( aLocalAddress.ip.getValue() ) );
   }

	return (iStatus != -1);
}

bool Jumpropes::BaseSocket::loadRemoteAddressInfo() {
	struct sockaddr_storage anAddress;
	socklen_t iAddrLen = sizeof(struct sockaddr_storage);
	int iStatus = -1;

	iStatus = getpeername( iSocketId, reinterpret_cast<sockaddr *>( &anAddress ), &iAddrLen );
	if ( iStatus != -1 ) {
      sockaddr_in6 *tmp = reinterpret_cast<sockaddr_in6 *>( &anAddress );  // sin_port and sin6_port are at the same position in both structs
      remotePort.set( ntohs( tmp->sin6_port ) );

      aRemoteAddress.ip.setLength( INET6_ADDRSTRLEN );
      aRemoteAddress.family = anAddress.ss_family;
      if ( aRemoteAddress.family == AF_INET ) {
         aRemoteAddress.setAsIPv4Address( &reinterpret_cast<sockaddr_in *>(&anAddress)->sin_addr );
         inet_ntop( AF_INET, aRemoteAddress.getIPv4AddressPtr(), aRemoteAddress.ip.getPointer(0), INET_ADDRSTRLEN );
      } else if ( aRemoteAddress.family == AF_INET6 ) {
         aRemoteAddress.setAsIPv6Address( &reinterpret_cast<sockaddr_in6 *>(&anAddress)->sin6_addr );
         inet_ntop( AF_INET6, aRemoteAddress.getIPv6AddressPtr(), aRemoteAddress.ip.getPointer(0), INET6_ADDRSTRLEN );
      }
      aRemoteAddress.ip.setLength( strlen( aRemoteAddress.ip.getValue() ) );
   }


/*
	if ( anAddress.ss_family == AF_INET ) {
		sockaddr_in *tmp = reinterpret_cast<sockaddr_in *>( &anAddress );

		sRemoteAddress->setValue( inet_ntoa( tmp->sin_addr ) );
		iRemotePort = ntohs( tmp->sin_port );
	} else if ( anAddress.ss_family == AF_INET6 ) {
      #ifdef GF_OS_WIN32

		sockaddr_in6 *tmp = reinterpret_cast<sockaddr_in6 *>( &anAddress );
      DWORD len = INET6_ADDRSTRLEN;

      sRemoteAddress->setSize( len );

      WSAAddressToString( reinterpret_cast<sockaddr *>( tmp ), sizeof(sockaddr_in6), NULL, sRemoteAddress->getValue(), &len );
      sRemoteAddress->setLength( len );
      #else
		sockaddr_in6 *tmp = reinterpret_cast<sockaddr_in6 *>( &anAddress );

		char in[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, tmp, in, INET6_ADDRSTRLEN);

		sRemoteAddress->setValue( in );
		iRemotePort = ntohs( tmp->sin6_port );
		#endif
   }
*/
	return (iStatus != -1);
}

bool Jumpropes::BaseSocket::isReadyToSend() {
   timeval tSelectTimeout;

   fd_set fdw;
   FD_ZERO( &fdw );
   FD_SET( iSocketId, &fdw );

   tSelectTimeout.tv_sec   = 0;
   tSelectTimeout.tv_usec  = 0;
   int iStatus = select( iSocketId + 1, NULL, &fdw, NULL, &tSelectTimeout );

   return ( iStatus > 0 );
}

bool Jumpropes::BaseSocket::connect() {
   disconnect();

/*
   if ( iAddressType == AF_INET6 ) {
      printf( "Setting up TCPSocket with IPv6 address...\n" );
   } else {
      printf( "Setting up TCPSocket with IPv4 address...\n" );
   }
*/

	iSocketId = socket( iAddressType, SOCK_STREAM, 0 );

	if ( iSocketId == -1 ) {
		return false;
	}
   bConnected = true;

	int iOn = 1;
	int iStatus = -1;

	iStatus = setsockopt( iSocketId, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>( &iOn ), sizeof( iOn ) );

   setBlocking( false );

	return ( iStatus != -1 );
}

bool Jumpropes::BaseSocket::disconnect() {
	if ( iSocketId != -1 ) {

      #ifdef GF_OS_LINUX
         close( iSocketId );
      #else
         closesocket( iSocketId );
      #endif

		iSocketId = -1;
      bConnected = false;
	}

	return true;
}

bool Jumpropes::BaseSocket::send( const String *sData, CommReturnData *errData ) {
	int iStatus = -1;

	if ( autosplit_sendbuffer.get() ) {
   	unsigned int iLen = sData->getLength();
   	unsigned int iSplit = autosplit_size.get();
   	unsigned int c = (iLen / iSplit);
   	unsigned int i = 0;
   	unsigned int p = 0;
   	for ( i = 0; i < c; i++ ) {
         while ( !isReadyToSend() ) {
            GFMillisleep(1);
         }
         iStatus = ::send( iSocketId, sData->getPointer(p), iSplit, MSG_NOSIGNAL );
         if ( iStatus == -1 ) {
            return false;
         }

         p += iSplit;
      }

      while ( !isReadyToSend() ) {
         GFMillisleep(1);
      }
      iStatus = ::send( iSocketId, sData->getPointer(p), iLen - p, MSG_NOSIGNAL );

   } else {
      while ( !isReadyToSend() ) {
         GFMillisleep(1);
      }
      iStatus = ::send( iSocketId, sData->getValue(), sData->getLength(), MSG_NOSIGNAL );
   }

	return ( iStatus != -1 );
}

bool Jumpropes::BaseSocket::receive( String *sData, CommReturnData *errData ) {
   int iStatus = -1;
   bool bIsOk = true;

   timeval tSelectTimeout;

   fd_set fdr;
   FD_ZERO( &fdr );
   FD_SET( iSocketId, &fdr );

   // iSocketId + 1 zorgt ervoor dat de range van 0..iSocketId nagelopen wordt
   // indien laatste parameter NULL is, blocked deze functie tot er iets te lezen is
   tSelectTimeout.tv_sec   = 0;
   tSelectTimeout.tv_usec  = 0;
   iStatus = select( iSocketId + 1, &fdr, NULL, NULL, &tSelectTimeout );

   if ( iStatus == -1 ) {
      bIsOk = false;
      iStatus = 0;
   } else if ( iStatus > 0 ) {
      iStatus = recv( iSocketId, sData->getValue(), sData->getSize() - 1, 0 );

    	if ( iStatus == -1 ) {
    		// er is een fout opgetreden, errno is de fout
         bIsOk = false;
    		iStatus = 0;
      } else if ( iStatus == 0 ) {
         bIsOk = false;
         iStatus = 0;
      }
   } else {
      iStatus = 0;
   }

   sData->setLength( iStatus );

	return bIsOk;
}

void Jumpropes::BaseSocket::setBlocking( bool bBlocking ) {
	#ifdef GF_OS_LINUX
    	int iFlag = 0;
    	iFlag = fcntl( iSocketId, F_GETFL );

    	if ( iFlag < 0 ) {
    		throw;
    	}

    	if ( bBlocking ) {
    		iFlag &= ~O_NONBLOCK;
    	} else {
    		iFlag |= O_NONBLOCK;
    	}

    	fcntl( iSocketId, F_SETFL, iFlag );
	#else
    	u_long iMode;
    	if ( bBlocking ) {
           iMode = 0;
        } else {
           iMode = 1;
        }

        if ( ioctlsocket( iSocketId, FIONBIO, &iMode ) == -1 ) {
            throw;
        }
	#endif
}


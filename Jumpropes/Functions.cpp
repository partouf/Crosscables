
#include <Groundfloor/Atoms/Defines.h>

#include "Functions.h"

#include <cstdio>
#include <Groundfloor/Bookshelfs/BValue.h>
#include <Groundfloor/Materials/Functions.h>


bool JRisIPv4address( const Groundfloor::String *sIp ) {
   Groundfloor::Vector *slices = Groundfloor::split( sIp, "." );
   int c = 0;

   Groundfloor::BValue *val = new Groundfloor::BValue();
   if ( slices->size() == 4 ) {
      for ( int i = 0; i < 4; i++ ) {
         Groundfloor::String *s = static_cast<Groundfloor::String *>( slices->elementAt(i) );
         if ( Groundfloor::isNumber( s->getValue(), s->getLength() ) ) {
            val->setString( s );

            int d = val->asInteger();
            if ( (d >= 0) && (d <= 255) ) {
               c++;
            }
         }
      }
   }
   delete val;

   delete slices;

   return (c == 4);
}


// http://nl.wikipedia.org/wiki/Internet_Protocol_Version_6

bool JRisIPv6address( const Groundfloor::String *sIp ) {
/*
   Groundfloor::String *sDot = new Groundfloor::String( ":" );
   Groundfloor::Vector *slices = split( sIp, sDot );
   delete sDot;
   int c = 0;

   if ( slices->size() == 8 ) {
      for ( int i = 0; i < 8; i++ ) {
         Groundfloor::String *s = static_cast<Groundfloor::String *>( slices->elementAt(i) );
         if ( isNumber( s->getValue(), s->getLength() ) ) {
            HexToInt( s );
            val->setString( s );

            int d = val->asInteger();
            if ( (d >= 0) && (d <= 255) ) {
               c++;
            }
         }
      }
   }

   delete slices;

   return (c == 4);
*/

   return false;
}

bool JRresolveIP( const Groundfloor::String *sSrcHostname, Groundfloor::String *sDestIP, bool bSpecialCheck ) {
   hostent *remoteHost;
   bool bSucces = false;

   remoteHost = gethostbyname( sSrcHostname->getValue() );

   if ( remoteHost != NULL ) {
      in_addr addr;
	   addr.s_addr = *(unsigned long *) remoteHost->h_addr_list[0];

		if ( bSpecialCheck ) {
			hostent *nonvalidHost;

         char *pTmpAddrStr = inet_ntoa( addr );
         Groundfloor::String *sRemoteHost = new Groundfloor::String( pTmpAddrStr, strlen(pTmpAddrStr) );

         nonvalidHost = gethostbyname( C_NONVALIDHOSTNAME );
         if ( nonvalidHost != NULL ) {
            in_addr nonvalidAddr;
            nonvalidAddr.s_addr = *(unsigned long *) nonvalidHost->h_addr_list[0];

            pTmpAddrStr = inet_ntoa( nonvalidAddr );
            Groundfloor::String *sNonvalidHost = new Groundfloor::String( pTmpAddrStr, strlen(pTmpAddrStr) );

            bSucces = ( !sRemoteHost->match( sNonvalidHost ) );

            if ( bSucces && (sDestIP != NULL) ) {
               sDestIP->setValue( sRemoteHost );
            }

            delete sNonvalidHost;
         } else {
            bSucces = true;

            if (sDestIP != NULL) {
               sDestIP->setValue( sRemoteHost );
            }
         }

         delete sRemoteHost;

		} else {
			if ( sDestIP != NULL ) {
            char *sAddr = inet_ntoa( addr );
				sDestIP->setValue( sAddr, strlen(sAddr) );
			}

			return true;
      }
   }

   return bSucces;
}

bool Jumpropes::JRresolveIP( const Groundfloor::String *sSrcHostname, LookupObject *aDestIP, bool bSpecialCheck ) {
   LookupObject *all = new LookupObject();
   all->name.setValue( sSrcHostname );
   if ( JRresolveAll( all ) ) {
      if ( bSpecialCheck ) {
         LookupObject *nonValidHost = new LookupObject();
         nonValidHost->name.setValue_ansi( C_NONVALIDHOSTNAME );

         while ( all->firstAddressEqualsOneOf( nonValidHost, false ) ) {
            delete all->removeElement( (unsigned int)0 );
            all->compress();

            if ( all->getAddress() == NULL ) {
               break;
            }
         }

         delete nonValidHost;
      }
   }

   if ( all->getAddress() != NULL ) {
      aDestIP->setValue( all );
   }
   delete all;

   return (aDestIP->getAddress() != NULL);
/*
   hostent *remoteHost;
   bool bSucces = false;

   remoteHost = gethostbyname( sSrcHostname->getValue() );
   if ( remoteHost != NULL ) {
      in_addr addr;
	   addr.s_addr = *(unsigned long *) remoteHost->h_addr_list[0];

		if ( bSpecialCheck ) {
			hostent *nonvalidHost;

         char *pTmpAddrStr = inet_ntoa( addr );
         Groundfloor::String *sRemoteHost = new Groundfloor::String( pTmpAddrStr, strlen(pTmpAddrStr) );

         nonvalidHost = gethostbyname( C_NONVALIDHOSTNAME );
         if ( nonvalidHost != NULL ) {
            in_addr nonvalidAddr;
            nonvalidAddr.s_addr = *(unsigned long *) nonvalidHost->h_addr_list[0];

            pTmpAddrStr = inet_ntoa( nonvalidAddr );
            Groundfloor::String *sNonvalidHost = new Groundfloor::String( pTmpAddrStr, strlen(pTmpAddrStr) );

            bSucces = ( !sRemoteHost->match( sNonvalidHost ) );

            if ( bSucces && (aDestIP != NULL) ) {
               IPAddress *ip = aDestIP->newAddress();
               ip->family = AF_INET;
               ip->ip.setValue( sRemoteHost );
            }

            delete sNonvalidHost;
         } else {
            bSucces = true;

            if (aDestIP != NULL) {
               IPAddress *ip = aDestIP->newAddress();
               ip->family = AF_INET;
               ip->ip.setValue( sRemoteHost );
            }
         }

         delete sRemoteHost;

		} else {
			if ( aDestIP != NULL ) {
            IPAddress *ip = aDestIP->newAddress();
            ip->family = AF_INET;
            char *sAddr = inet_ntoa( addr );
            ip->ip.setValue( sAddr, strlen(sAddr) );
			}

			return true;
      }
   }

   return bSucces;
*/
}


#ifdef GF_OS_WIN32
#ifdef GF_CP_MINGW
const char *inet_ntop( int af, const void *src, char *dst, socklen_t cnt ) {
        if ( af == AF_INET )  {
                sockaddr_in in;
                memset( &in, 0, sizeof(sockaddr_in) );
                in.sin_family = AF_INET;

                memcpy( &in.sin_addr, src, sizeof(in_addr));

                getnameinfo( (sockaddr *)&in, sizeof(sockaddr_in), dst, cnt, NULL, 0, NI_NUMERICHOST );

                return dst;
        } else if ( af == AF_INET6 ) {
                sockaddr_in6 in;
                memset( &in, 0, sizeof(sockaddr_in6) );
                in.sin6_family = AF_INET6;

                memcpy( &in.sin6_addr, src, sizeof(in_addr6) );

                getnameinfo( (sockaddr *)&in, sizeof(sockaddr_in6), dst, cnt, NULL, 0, NI_NUMERICHOST );

                return dst;
        }
        return NULL;
}

int inet_pton( int af, const char *src, void *dst ) {
   if ( af == AF_INET ) {
      addrinfo hints;
      addrinfo *aiList = NULL;

      memset( &hints, 0, sizeof(addrinfo) );
      hints.ai_family = AF_INET;
      hints.ai_flags = AI_NUMERICHOST;

      int iRes = getaddrinfo( src, NULL, &hints, &aiList );
      if ( iRes == 0 ) {
         addrinfo *pInfo = aiList;
         if ( pInfo != NULL ) {
            memcpy( dst, &reinterpret_cast<sockaddr_in *>(pInfo->ai_addr)->sin_addr, sizeof(in_addr) );
            freeaddrinfo( pInfo );
            return 1;
         }
      }

      return -1;
   } else if ( af == AF_INET6 ) {
      addrinfo hints;
      addrinfo *aiList = NULL;

      memset( &hints, 0, sizeof(addrinfo) );
      hints.ai_family = AF_INET6;
      hints.ai_flags = AI_NUMERICHOST;

      int iRes = getaddrinfo( src, NULL, &hints, &aiList );
      if ( iRes == 0 ) {
         addrinfo *pInfo = aiList;
         if ( pInfo != NULL ) {
            memcpy( dst, &reinterpret_cast<sockaddr_in6 *>(pInfo->ai_addr)->sin6_addr, sizeof(in6_addr) );
            freeaddrinfo( pInfo );
            return 1;
         }
      }

      return -1;
   }
   return 0;
}
#endif
#endif


bool Jumpropes::SetAndResolveIP(IPAddress *IP, const sockaddr *address)
{
   IP->family = address->sa_family;

   if (IP->family == AF_INET)
   {
      IP->setAsIPv4Address(&reinterpret_cast<const sockaddr_in *>(address)->sin_addr);

      IP->ip.setSize(INET_ADDRSTRLEN);
      inet_ntop(IP->family, IP->getIPv4AddressPtr(), IP->ip.getPointer(0), INET_ADDRSTRLEN);
      IP->ip.trimzero_ansi();

      return true;
   }
   else if (IP->family == AF_INET6)
   {
      IP->setAsIPv6Address(&reinterpret_cast<const sockaddr_in6 *>(address)->sin6_addr);

      IP->ip.setSize(INET6_ADDRSTRLEN);
      inet_ntop(IP->family, IP->getIPv6AddressPtr(), IP->ip.getPointer(0), INET6_ADDRSTRLEN);
      IP->ip.trimzero_ansi();

      return true;
   }

   return false;
}

bool JRresolveAllWithGetAddrInfo(Jumpropes::LookupObject *aDestIpArray ) {
   bool bSuccess = false;
   addrinfo aiHints;
   addrinfo *aiList = NULL;

   memset(&aiHints, 0, sizeof(aiHints));
   aiHints.ai_family = AF_UNSPEC;
   //aiHints.ai_socktype = SOCK_STREAM;
   //aiHints.ai_protocol = IPPROTO_TCP;

   int iRet = getaddrinfo( aDestIpArray->name.getValue(), NULL, &aiHints, &aiList );
   if ( iRet == 0 ) {
      int i = 0;
      addrinfo *pInfo = aiList;

      while ( pInfo != NULL ) {
         Jumpropes::IPAddress *address = new Jumpropes::IPAddress();
         address->family = pInfo->ai_family;

         if ( address->family == AF_INET ) {
            address->ip.setSize( INET_ADDRSTRLEN );
            address->setAsIPv4Address( &reinterpret_cast<sockaddr_in *>(pInfo->ai_addr)->sin_addr );
            inet_ntop( address->family, reinterpret_cast<char *>( address->getIPv4AddressPtr() ), address->ip.getPointer(0), INET_ADDRSTRLEN );

            address->ip.setLength( strlen( address->ip.getValue() ) );
            aDestIpArray->addElement( address );

            bSuccess = true;
         } else if ( address->family == AF_INET6 ) {
            address->ip.setSize( INET6_ADDRSTRLEN );
            address->setAsIPv6Address( &reinterpret_cast<sockaddr_in6 *>(pInfo->ai_addr)->sin6_addr );
            inet_ntop( address->family, reinterpret_cast<char *>( address->getIPv6AddressPtr() ), address->ip.getPointer(0), INET6_ADDRSTRLEN );

            address->ip.setLength( strlen( address->ip.getValue() ) );
            aDestIpArray->addElement( address );

            bSuccess = true;
         }

         i++;
         addrinfo *pPrev = pInfo;
         pInfo = pPrev->ai_next;
         freeaddrinfo( pPrev );
      }
   }

   return bSuccess;
}

bool Jumpropes::JRresolveAll( LookupObject *aDestIpArray ) {
   hostent *remoteHost;
   bool bSuccess = false;

   // gethostbyname() only retreives IPv4 addresses under Microsoft Windows and is deprecated as well
   remoteHost = gethostbyname( aDestIpArray->name.getValue() );
   if ( remoteHost != NULL ) {
      int i = 0;
      char *pAddr = remoteHost->h_addr_list[i];
      while ( pAddr != NULL ) {
         IPAddress *address = new IPAddress();
         address->family = remoteHost->h_addrtype;

         if ( address->family == AF_INET ) {
            address->ip.setSize( INET_ADDRSTRLEN );
            address->setAsIPv4Address( reinterpret_cast<in_addr *>( remoteHost->h_addr_list[i] ) );
            inet_ntop( address->family, pAddr, address->ip.getPointer(0), INET_ADDRSTRLEN );
            address->ip.setLength( strlen( address->ip.getValue() ) );

            aDestIpArray->addElement( address );
            bSuccess = true;
         } else if ( address->family == AF_INET6 ) {
            address->ip.setSize( INET6_ADDRSTRLEN );
            address->setAsIPv6Address( reinterpret_cast<in6_addr *>( remoteHost->h_addr_list[i] ) );
            inet_ntop( address->family, pAddr, address->ip.getPointer(0), INET6_ADDRSTRLEN );
            address->ip.setLength( strlen( address->ip.getValue() ) );

            aDestIpArray->addElement( address );
            bSuccess = true;
         }

         i++;
         pAddr = remoteHost->h_addr_list[i];
      }
   }

   if ( !bSuccess ) {
      return JRresolveAllWithGetAddrInfo( aDestIpArray );
   }

   return bSuccess;
}

bool Jumpropes::TryToGetComputerName(String *PComputername)
{
   PComputername->setLength(1024);
   if (gethostname(PComputername->getPointer(0), 1024) == 0)
   {
      PComputername->trimzero_ansi();

      return true;
   }
   else
   {
      PComputername->setLength(0);

      return false;
   }
}

std::vector<Jumpropes::NetInterface> Jumpropes::ListNetworkInterfaces()
{
   std::vector<Jumpropes::NetInterface> List;

   String Computername;
   if (!TryToGetComputerName(&Computername))
   {
      return List;
   }

   struct addrinfo hints, *ifa;
   int err;

   memset(&hints, 0, sizeof(hints));
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_family = AF_INET;

   if ((err = getaddrinfo(Computername.getValue(), NULL, &hints, &ifa)) != 0) {
      return List;
   }

   for (; ifa != NULL; ifa = ifa->ai_next) {
      if (ifa->ai_addr == NULL)
         continue;

      Jumpropes::NetInterface netif;
      netif.Name = "";// ifa->ai_canonname;

      SetAndResolveIP(&netif.IP, ifa->ai_addr);

      List.push_back(netif);
   }

   freeaddrinfo(ifa);

   return List;
}


#ifndef __JRFunctions_H
#define __JRFunctions_H

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Molecules/String.h>
#include <Groundfloor/Molecules/Vector.h>

#include "Defines.h"
#include "NetAddress.h"

#ifdef GF_OS_WIN32
   #include <winsock2.h>
   #include <ws2tcpip.h>
   #include <wspiapi.h>
#else
   #include <netdb.h>
   #include <unistd.h>
   #include <fcntl.h>
   #include <arpa/inet.h>
   #include <sys/types.h>

   #include <sys/select.h>

   #include <cstring>

   #include <sys/socket.h>
   #include <ifaddrs.h>

   #include <linux/if_link.h>
#endif

#include <vector>


namespace Jumpropes {

   /// Tries to resolve the given Hostname into an IP address
   /** If the given Hostname could be resolved True is returned and optionally
     *  sDestIP will be set to the ascii representation of the IP address.
     * sDestIP may be set to NULL.
     * If bSpecialCheck is set True, the result of the lookup will be compared
     *  to the result of the hostname loookup "nonvalid.domain.name.nonvalid".
     * This is added to make OS differences smaller and to even return false
     *  when a DNS provider like OpenDNS is used.
     * Note: sSrcHostname can be an IP Address as well!
     **/
   bool JRresolveIP(const String *sSrcHostname, LookupObject *aDestIP, bool bSpecialCheck = false);

   bool JRresolveAll(LookupObject *aDestIpArray);

   //bool JRisIPv4address( const String *sIp );
   //bool JRisIPv6address( const String *sIp );

   bool TryToGetComputerName(String *PComputername);

   bool SetAndResolveIP(IPAddress *IP, const sockaddr *address);

   struct NetInterface
   {
      std::string Name;
      Jumpropes::IPAddress IP;
   };

   std::vector<NetInterface> ListNetworkInterfaces();
}

#ifdef GF_OS_WIN32
   #ifdef GF_CP_MINGW
      int inet_pton( int af, const char *src, void *dst );
      const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);
   #endif
#endif

#endif // __JRFunctions_H

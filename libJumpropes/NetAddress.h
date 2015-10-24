
#ifndef __JRNetAddress_H
#define __JRNetAddress_H

namespace Jumpropes {
   class IPAddress;
   class LookupObject;
}

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Atoms/Freeable.h>
#include <Groundfloor/Molecules/String.h>
#include <Groundfloor/Molecules/Vector.h>
#include <Groundfloor/Molecules/Property.h>

#ifdef GF_OS_WIN32
   #include <inaddr.h>
   #include <in6addr.h>
#else
   #include <netinet/in.h>
#endif

using namespace Groundfloor;

namespace Jumpropes {

   class LookupObject : public Vector {
   protected:
   public:
      String name;

      void setValue(const LookupObject *obj);
      bool firstAddressEqualsOneOf(const LookupObject *obj, bool bBothNullIsWin = true) const;

      IPAddress *getAddress(unsigned int i = 0) const;
   };

   class IPAddress : public Freeable {
   protected:
      BYTE addressbytes[16];
   public:
      IPAddress();

      void setValue(const IPAddress *addr);
      bool equals(const IPAddress *addr);

      in_addr *getIPv4AddressPtr();
      in6_addr *getIPv6AddressPtr();

      void setAsIPv4Address(in_addr *address);
      void setAsIPv6Address(in6_addr *address);

      unsigned short family;
      String ip;
   };

}

#endif // __JRNetAddress_H

#ifndef _JRSERVERSOCKET_H_
#define _JRSERVERSOCKET_H_

#include "BaseSocket.h"

using namespace Groundfloor;

namespace Jumpropes {

   class ServerSocket : public BaseSocket
   {
   protected:
   public:
      ServerSocket();
      ~ServerSocket();

      /// Sets the socket's IP protocol. AF_INET is default, AF_INET6 is also possible
      void setAddressType(int iAFType);

      Property<int> maxconnections;

      bool connect();

      BaseSocket *waitForNewClient();
      bool waitForNewClientAndReinitialize(BaseSocket *anExistingSocket);
   };

}

#endif /*_JRSERVERSOCKET_H_*/

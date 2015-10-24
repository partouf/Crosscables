
#ifndef __JRHttpConnection_H
#define __JRHttpConnection_H

#include <Groundfloor/Atoms/Lockable.h>
#include <Groundfloor/Molecules/StringVector.h>
#include "../ThreadedConnection.h"

using namespace Groundfloor;
using namespace Jumpropes;

namespace JumpropesCommon {

   class HttpConnection : public ThreadedConnection {
   protected:
   public:
      HttpConnection(BaseSocket *aSocket);
      ~HttpConnection();

      Lockable bufferlock;
      StringVector receivedData;

      void newMessageReceived(const String *sMessage);
   };

}

#endif // __JRHttpConnection_H


#ifndef _JRTHREADEDCONNECTION_H_
#define _JRTHREADEDCONNECTION_H_

#include <Groundfloor/Materials/Thread.h>
#include "BaseSocket.h"

using namespace Groundfloor;

namespace Jumpropes {

   class ThreadedConnection : public Thread
   {
   protected:
      BaseSocket *socket;
      String *sMessage;
   public:
      ThreadedConnection(BaseSocket *aSocket);
      ~ThreadedConnection();

      BaseSocket *setSocket(BaseSocket *aSocket);
      BaseSocket *getSocket();

      void execute();

      virtual void newMessageReceived(const String *sMessage) = 0;
   };

}

#endif // _JRTHREADEDCONNECTION_H_

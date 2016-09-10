
#ifndef _JRTHREADEDSERVER_H_
#define _JRTHREADEDSERVER_H_

#include <Groundfloor/Materials/Thread.h>
#include "ServerSocket.h"

using namespace Groundfloor;

namespace Jumpropes {

   class ThreadedServer : public Thread
   {
   protected:
      ServerSocket *socket;
      bool bSocketAssigned;

      virtual void newClientConnection(BaseSocket *aClient) = 0;

   public:
      ThreadedServer();
      ThreadedServer(ServerSocket *aSocket);
      ~ThreadedServer();

      bool startListening(int iPort, int iMax);
      bool startListening(int iPort);

      ServerSocket *getServerSocket() { return socket; };

      void execute();
   };

}

#endif // _JRTHREADEDSERVER_H_

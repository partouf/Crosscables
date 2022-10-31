
#ifndef __JRTHREADEDSENDER_H
#define __JRTHREADEDSENDER_H

#include <Groundfloor/Materials/ThreadedBuffer.h>
#include "BaseSocket.h"

using namespace Groundfloor;

namespace Jumpropes {

   class ThreadedSender : public ThreadedBuffer {
   protected:
      BaseSocket *socket;
   public:
      ThreadedSender(BaseSocket *aSocket);
      ~ThreadedSender();

      void add(Freeable *obj);

      bool processObject(Freeable *obj);
   };

}

#endif // __JRTHREADEDSENDER_H


#ifndef __JKClientSocket_H
#define __JKClientSocket_H

#include "SSLBaseSocket.h"

namespace Jumpknots {

   class SSLClientSocket : public SSLBaseSocket {
   protected:
      int iVerifyResult;
   public:
      SSLClientSocket();
      ~SSLClientSocket();

      bool connect();
      //bool connect( const TGFString *sHost, int iPort );
   };

}

#endif // __JKClientSocket_H

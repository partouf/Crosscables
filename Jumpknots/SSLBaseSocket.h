
#ifndef __JKBASESOCKET_H_
#define __JKBASESOCKET_H_

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Molecules/String.h>
#include <Jumpropes/BaseSocket.h>

#include <openssl/ssl.h>

using namespace Groundfloor;
using namespace Jumpropes;

namespace Jumpknots {

   class SSLBaseSocket : public BaseSocket {
   protected:
#ifdef GF_OS_WIN32
      const SSL_METHOD *method;
#else
      const SSL_METHOD *method;
#endif
      SSL_CTX *ctx;
      SSL *ssl;
      bool bIsSlave;

   public:
      SSLBaseSocket();
      SSLBaseSocket(int iSocketId, const BaseSocket *aServer);
      ~SSLBaseSocket();

      bool connect();
      bool disconnect();

      bool send(const String *sData, CommReturnData *errData = NULL);
      bool receive(String *sData, CommReturnData *errData = NULL);
   };

}

#endif // __JKBASESOCKET_H_

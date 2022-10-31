
#include "SSLBaseSocket.h"

#include <openssl/err.h>

Jumpknots::SSLBaseSocket::SSLBaseSocket() : Jumpropes::BaseSocket() {
   bIsSlave = false;
   this->method = NULL;
   this->ctx = NULL;
   this->ssl = NULL;
}

Jumpknots::SSLBaseSocket::SSLBaseSocket(int iSocketId, const Jumpropes::BaseSocket *aServer) : Jumpropes::BaseSocket(iSocketId, aServer) {
   bIsSlave = true;
   this->method = NULL;
   this->ctx = static_cast<const SSLBaseSocket *>(aServer)->ctx;

   this->ssl = SSL_new(this->ctx);
   SSL_set_fd(this->ssl, iSocketId);
   SSL_accept(this->ssl);
}

Jumpknots::SSLBaseSocket::~SSLBaseSocket() {
   disconnect();

   if (!bIsSlave) {
      if (ctx != NULL) {
         SSL_CTX_free(this->ctx);

         this->ctx = NULL;
      }
   }
}

bool Jumpknots::SSLBaseSocket::connect() {
   return Jumpropes::BaseSocket::connect();
}

bool Jumpknots::SSLBaseSocket::disconnect() {
   if (this->ssl != NULL) {
      SSL_free(this->ssl);

      this->ssl = NULL;
   }

   return Jumpropes::BaseSocket::disconnect();
}

bool Jumpknots::SSLBaseSocket::send(const String *sData, CommReturnData *errData) {
   int iStatus = -1;

   if (autosplit_sendbuffer.get()) {
      unsigned int iLen = sData->getLength();
      unsigned int iSplit = autosplit_size.get();
      unsigned int c = (iLen / iSplit);
      unsigned int i = 0;
      unsigned int p = 0;
      for (i = 0; i < c; i++) {
         while (!isReadyToSend()) {
            GFMillisleep(1);
         }
         iStatus = SSL_write(this->ssl, sData->getPointer(p), iSplit);
         if (iStatus == -1) {
            return false;
         }

         p += iSplit;
      }

      while (!isReadyToSend()) {
         GFMillisleep(1);
      }
      iStatus = SSL_write(this->ssl, sData->getPointer(p), iLen - p);

   }
   else {
      while (!isReadyToSend()) {
         GFMillisleep(1);
      }
      iStatus = SSL_write(this->ssl, sData->getValue(), sData->getLength());
   }

   return (iStatus != -1);
}

bool Jumpknots::SSLBaseSocket::receive(String *sData, CommReturnData *errData) {
   int iStatus = -1;
   bool bIsOk = true;

   timeval tSelectTimeout;

   fd_set fdr;
   FD_ZERO(&fdr);
   FD_SET(iSocketId, &fdr);

   // iSocketId + 1 zorgt ervoor dat de range van 0..iSocketId nagelopen wordt
   // indien laatste parameter NULL is, blocked deze functie tot er iets te lezen is
   tSelectTimeout.tv_sec = 0;
   tSelectTimeout.tv_usec = 0;
   iStatus = select(iSocketId + 1, &fdr, NULL, NULL, &tSelectTimeout);

   if (iStatus == -1) {
      bIsOk = false;
      iStatus = 0;
   }
   else if (iStatus > 0) {
      iStatus = SSL_read(this->ssl, sData->getValue(), sData->getSize() - 1);

      if (iStatus == -1) {
         // er is een fout opgetreden, errno is de fout
         bIsOk = false;
         iStatus = 0;
      }
      else if (iStatus == 0) {
         bIsOk = false;
         iStatus = 0;
      }
   }
   else {
      iStatus = 0;
   }

   sData->setLength(iStatus);

   return bIsOk;
}

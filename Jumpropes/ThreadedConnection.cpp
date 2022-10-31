
#include "ThreadedConnection.h"
#include <cstdio>

Jumpropes::ThreadedConnection::ThreadedConnection( BaseSocket *aSocket ) : Thread() {
    this->socket = aSocket;

    sMessage = new String( "", 0 );
    sMessage->setSize( 1024 );

    this->setInterval( 50 );
}

Jumpropes::ThreadedConnection::~ThreadedConnection() {
   stopAndWait();
   delete sMessage;

// freeing the socket is not this class's responsibility ????
   if ( socket != 0 ) {
      delete socket;
   }
}

Jumpropes::BaseSocket *Jumpropes::ThreadedConnection::setSocket( BaseSocket *aSocket ) {
   stopAndWait();

   BaseSocket *oldSock = this->socket;
   this->socket = aSocket;

   return oldSock;
}

void Jumpropes::ThreadedConnection::execute() {
    if ( socket->receive( sMessage ) ) {
        if ( sMessage->getLength() > 0 ) {
            newMessageReceived( sMessage );
        }
    } else {
        stop();
    }
}

Jumpropes::BaseSocket *Jumpropes::ThreadedConnection::getSocket() {
    return socket;
};

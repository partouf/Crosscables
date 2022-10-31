
#include "HttpConnection.h"

#include <cstdio>

JumpropesCommon::HttpConnection::HttpConnection( BaseSocket *aSocket ) : ThreadedConnection(aSocket) {
//   receivedData.setSize( 1024 );
}

JumpropesCommon::HttpConnection::~HttpConnection() {
}

void JumpropesCommon::HttpConnection::newMessageReceived( const String *sMessage ) {
   bufferlock.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   String *sMessageCopy = new String( sMessage );
   
   receivedData.addChunk( sMessageCopy );
   
   bufferlock.unlock();
}

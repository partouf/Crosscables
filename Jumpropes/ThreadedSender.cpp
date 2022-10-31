
#include "ThreadedSender.h"
#include <Groundfloor/Atoms/Defines.h>

Jumpropes::ThreadedSender::ThreadedSender( BaseSocket *aSocket ) : ThreadedBuffer() {
   socket = aSocket;

   aBuffer.resizeVector( 100 );   // niet te laag instellen
   this->setInterval( 100 );
}

Jumpropes::ThreadedSender::~ThreadedSender() {
}


void Jumpropes::ThreadedSender::add( Freeable *obj ) {
   String *sDataCopy = new String( static_cast<String *>(obj) );
   
   ThreadedBuffer::add( sDataCopy );
}

bool Jumpropes::ThreadedSender::processObject( Freeable *obj ) {
   String *str = static_cast<String *>(obj);

   if ( socket->send( str ) ) {
      delete str;
      
      return true;
   }

   return false;
}


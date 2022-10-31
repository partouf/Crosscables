
#include "ThreadedServer.h"
#include "BaseSocket.h"
#include <cstdio>

Jumpropes::ThreadedServer::ThreadedServer() : Thread() {
    socket = new ServerSocket();
    bSocketAssigned = false;
}

Jumpropes::ThreadedServer::ThreadedServer( ServerSocket *aSocket ) : Thread() {
    bSocketAssigned = true;
    this->socket = aSocket;
}

Jumpropes::ThreadedServer::~ThreadedServer() {
    if ( !bSocketAssigned && (socket != 0) ) {
        delete socket;
    }
}

bool Jumpropes::ThreadedServer::startListening( int iPort, int iMax ) {
   socket->localPort.set( iPort );
   socket->maxconnections.set( iMax );
   
   bool listening = socket->connect();

   start();

   return listening;
}

bool Jumpropes::ThreadedServer::startListening( int iPort ) {
   return startListening( iPort, 2114125311 );
}

void Jumpropes::ThreadedServer::execute() {
    BaseSocket *client = socket->waitForNewClient();

    if ( client != 0 ) {
        newClientConnection( client );
    }
}


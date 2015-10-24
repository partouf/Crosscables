
#include "Initialize.h"

#include <Groundfloor/Atoms/Defines.h>

#ifdef GF_OS_WIN32
    #include <winsock2.h>
#endif


bool Jumpropes::initJumpropes() {
    
    #ifdef GF_OS_WIN32
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
        
        wVersionRequested = MAKEWORD( 1, 0 );
        
        err = WSAStartup( wVersionRequested, &wsaData );
        if ( err != 0 ) {
            return false;
        }
        
    /* version check?
        if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {
            WSACleanup( );
            return 1; 
        }
    */
    #endif

    return true;
}

void Jumpropes::finiJumpropes() {
    
    #ifdef GF_OS_WIN32
        WSACleanup();
    #endif

}


#ifndef __JRDefines_H
#define __JRDefines_H

#include <Groundfloor/Atoms/Defines.h>

#ifdef GF_OS_WIN32
   #include <winsock2.h>
   #include <ws2tcpip.h>

   #ifdef GF_CP_MINGW
      #define socklen_t int
   #endif
   #define MSG_NOSIGNAL 0
#else
   #include <sys/socket.h>

   #if defined(GF_OS_MAC)
      #define MSG_NOSIGNAL SO_NOSIGPIPE
   #endif
   #if defined(GF_OS_SOLARIS)
      #define MSG_NOSIGNAL 0
   #endif
#endif


#define JR_CONNECTIONTYPE_TCP    1
#define JR_CONNECTIONTYPE_UDP    2


#define C_NONVALIDHOSTNAME "nonvaliddomainname.com"


#endif //__JRDefines_H

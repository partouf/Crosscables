
#ifndef __GFLOGGER_H
#define __GFLOGGER_H

#include "Thread.h"
#include "../Molecules/String.h"
#include "../Molecules/Vector.h"
#include "../Atoms/Lockable.h"


#define NO_CRLF 0
#define UNIX_FILE 1
#define DOS_FILE 2
#define MAC_FILE 3

#define NO_OUTPUT 0
#define CONSOLE_OUTPUT 1
#define FILE_OUTPUT 2
#define FULL_OUTPUT 3

namespace Groundfloor {

   /// class is not used
   class Logger : public Thread {
   protected:
      Vector lstMessages;

      String sOutputFile;

      String sStartPreMsg;
      String sEndPreMsg;
      String sStartPostMsg;
      String sEndPostMsg;

      Lockable lBufferLock;

      int iLogType;
      int iFileType;
      int iNestlevel;

      void resetVars();

      void logInFile(const String *sMsg);
      void logInStdOut(const String *sMsg);

   public:
      void begin(const String *sMsg);
      void begin(const char *sMsg);
      void end(const String *sMsg);
      void end(const char *sMsg);


      Logger(int logType, char *logFile, int logFileType, bool verbose);
      ~Logger();

      void log(const String *sMsg);
      void log(const char *sMsg);
   };

}

#endif // __GFLOGGER_H

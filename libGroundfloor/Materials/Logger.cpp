
#include "Logger.h"

#include <cstdio>
#include <cstdlib>


Groundfloor::Logger::Logger( int logType, char *logFile, int logFileType, bool verbose ) {
   resetVars();
}

void Groundfloor::Logger::resetVars() {
   sStartPreMsg.setValue_ansi( "START: " );
   sEndPreMsg.setValue( "", 0 );
   sStartPostMsg.setValue_ansi( "END: " );
   sEndPostMsg.setValue( "", 0 );

	iLogType       = CONSOLE_OUTPUT;
   iFileType      = NO_CRLF;
   iNestlevel     = 0;
}

Groundfloor::Logger::~Logger() {
}

void Groundfloor::Logger::logInFile( const Groundfloor::String *sMsg ) {
   FILE *fp = fopen( sOutputFile.getValue(), "ab" );

   fwrite( sMsg->getValue(), sMsg->getLength(), 1, fp );

   fclose( fp );
}

void Groundfloor::Logger::logInStdOut( const Groundfloor::String *sMsg ) {
   printf( "%s", sMsg->getValue() );
}


void Groundfloor::Logger::log( const Groundfloor::String *sMsg ) {
   Groundfloor::String *sCopy = new Groundfloor::String( sMsg );

//   sCopy->prepend(  );
   lstMessages.addElement( sCopy );
}

void Groundfloor::Logger::log( const char *sMsg ) {
   Groundfloor::String *sCopy = new Groundfloor::String( sMsg );

   lstMessages.addElement( sCopy );
}



#include "FileCommunicator.h"

Groundfloor::FileCommunicator::FileCommunicator() : Groundfloor::BaseCommunicator() {
   fp = NULL;
   mode.set( GFFILEMODE_READ );
}

Groundfloor::FileCommunicator::~FileCommunicator() {
   disconnect();
}

bool Groundfloor::FileCommunicator::connect() {
   disconnect();

   int iMode = mode.get();

   bool bRead     = ((iMode & GFFILEMODE_READ) != 0);
   bool bWrite    = ((iMode & GFFILEMODE_WRITE) != 0);
   bool bAppend   = ((iMode & GFFILEMODE_APPEND) != 0);

   if ( filename.getLength() != 0 ) {
      #ifdef GF_OS_WIN32
      Groundfloor::String sTmpFile( filename.link() );
      sTmpFile.transformToWidestring( true );

      if (bAppend) {
         if ( bRead ) {
            fp = _wfopen( sTmpFile.getValueAsWide(), L"r+b" );
         } else {
            fp = _wfopen( sTmpFile.getValueAsWide(), L"ab" );
         }
      } else {
         if ( bWrite && bRead ) {
            fp = _wfopen( sTmpFile.getValueAsWide(), L"w+b" );
         } else if ( bWrite ) {
            fp = _wfopen( sTmpFile.getValueAsWide(), L"wb" );
         } else {
            fp = _wfopen( sTmpFile.getValueAsWide(), L"rb" );
         }
      }
      #else
      if (bAppend) {
         if ( bRead ) {
            fp = fopen( filename.get(), "r+b" );
         } else {
            fp = fopen( filename.get(), "ab" );
         }
      } else {
         if ( bWrite && bRead ) {
            fp = fopen( filename.get(), "w+b" );
         } else if ( bWrite ) {
            fp = fopen( filename.get(), "wb" );
         } else {
            fp = fopen( filename.get(), "rb" );
         }
      }
      #endif
   }

   bConnected = (fp != NULL);

   return bConnected;
}

bool Groundfloor::FileCommunicator::disconnect() {
   if ( bConnected ) {
      fclose( fp );

      fp          = NULL;
      bConnected  = false;
   }

   return true;
}

bool Groundfloor::FileCommunicator::send( const Groundfloor::String *sData, Groundfloor::CommReturnData *errData ) {
   if ( bConnected ) {
      lockReadWrite.lockWhenAvailable( GFLOCK_INFINITEWAIT );

      size_t iLastWritten = fwrite( sData->getPointer(0), 1, sData->getLength(), fp );

      if ( errData != NULL ) {
         errData->affected    = iLastWritten;
         errData->eof         = (feof( fp ) != 0);
         errData->errorcode   = ferror( fp );
         errData->error       = (errData->errorcode != 0);
      }

      if (flushdirectly.get()) {
         fflush(fp);
      }

      lockReadWrite.unlock();
      return true;
   }

   return false;
}

bool Groundfloor::FileCommunicator::receive( Groundfloor::String *sData, Groundfloor::CommReturnData *errData ) {
   if ( bConnected ) {
      lockReadWrite.lockWhenAvailable( GFLOCK_INFINITEWAIT );

      size_t iLastRead = fread( sData->getPointer(0), 1, sData->getSize(), fp );
      sData->setLength( iLastRead );

      if ( errData != NULL ) {
         errData->affected    = iLastRead;
         errData->eof         = (feof( fp ) != 0);
         errData->errorcode   = ferror( fp );
         errData->error       = (errData->errorcode != 0);
      }

      lockReadWrite.unlock();
      return true;
   }

   return false;
}


#include "Directory.h"

#include <cstdio>
#include "../Materials/Functions.h"

#ifdef GF_OS_LINUX
#include <sys/stat.h>
#include <cstring>
#endif

Groundfloor::Directory::Directory() {
   bOpen = false;

   #ifdef GF_OS_WIN32
   bFirstTime = false;
   #endif
}

Groundfloor::Directory::~Directory() {
   if ( bOpen ) {
      close();
   }
}

bool Groundfloor::Directory::open( const Groundfloor::String *sDir ) {
   bOpen = false;

   sCurrentDir.setValue( sDir );

   #ifdef GF_OS_WIN32
      Groundfloor::String sWildDir(&sCurrentDir);
      sWildDir.transformToWidestring( true );
      sWildDir.append_wide( L"\\*.*" );

      hDir = FindFirstFileW( reinterpret_cast<const wchar_t *>( sWildDir.getValue() ), reinterpret_cast<_WIN32_FIND_DATAW *>( &anEntryW ) );

      if ( hDir == INVALID_HANDLE_VALUE ) {
         return false;
      }

      bFirstTime = true;
   #else
      hDir = opendir( sDir->getValue() );
      if ( hDir == 0 ) {
         return false;
      }
   #endif

   bOpen = true;
   return true;
}

bool Groundfloor::Directory::next() {
   #ifdef GF_OS_WIN32
      if ( !bFirstTime ) {

         if ( !FindNextFileW( hDir, reinterpret_cast<_WIN32_FIND_DATAW *>( &anEntryW ) ) ) {
            return false;
         }

      } else {
         bFirstTime = false;
      }
   #else
      anEntry = readdir( hDir );
      if ( anEntry == 0 ) {
         return false;
      }
   #endif

   return true;
}

bool Groundfloor::Directory::close() {
   #ifdef GF_OS_WIN32
      FindClose( hDir );
   #else
      closedir( hDir );
   #endif

   bOpen = false;

   return true;
}


Groundfloor::String *Groundfloor::Directory::getFileName() {
   Groundfloor::String *sName = NULL;

   #ifdef GF_OS_WIN32
      sName = new Groundfloor::String( reinterpret_cast<const char *>( anEntryW.cFileName ), getWStrByteLen(anEntryW.cFileName) );
      sName->transformWidestringToString( true );
   #else
      sName = new Groundfloor::String( anEntry->d_name, strlen(anEntry->d_name) );
   #endif

   return sName;
}

Groundfloor::String *Groundfloor::Directory::getShortName() {
   Groundfloor::String *sName = NULL;

   if ( bOpen ) {
      #ifdef GF_OS_WIN32
         Groundfloor::String sLongPath( sCurrentDir );
         Groundfloor::AppendNativeSlash( &sLongPath, false );
         sLongPath.transformToWidestring( true );
         sLongPath.append( reinterpret_cast<const char *>( anEntryW.cFileName ), getWStrByteLen(anEntryW.cFileName) );
         sName = new Groundfloor::String();
         sName->setLength( MAX_PATH );
         sName->setLength( GetShortPathNameW( reinterpret_cast<wchar_t *>( sLongPath.getValue() ), reinterpret_cast<wchar_t *>( sName->getValue() ), MAX_PATH ) * sizeof(wchar_t) );
         sName->transformWidestringToString( true );
/*
         Groundfloor::Vector *arr = GFsplit( sName, reinterpret_cast<const char *>( S_WIDE_NATIVE_SLASH ) );
         sName->setValue( static_cast<Groundfloor::String *>( arr->elementAt( arr->size() - 1 ) ) );
         delete arr;
*/
         Groundfloor::String *slash = createHybridString( S_WIDE_NATIVE_SLASH, true );
         int iPos = sName->lastpos( slash ) + slash->getLength();
         sName->setValue( sName->getPointer(iPos), sName->getLength() - iPos );
         delete slash;
      #else
         sName = new Groundfloor::String( anEntry->d_name, strlen(anEntry->d_name) );
      #endif
   }

   return sName;
}

bool Groundfloor::Directory::isDirectory() {
   #ifdef GF_OS_WIN32
      return ((anEntryW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
   #else
      #ifdef _DIRENT_HAVE_D_TYPE
         if ( anEntry->d_type == DT_UNKNOWN ) {
            // dt_unknown, op een andere manier bepalen dan maar...
            Groundfloor::String *sTmp = new Groundfloor::String( sCurrentDir );
            GFAppendNativeSlash( sTmp );
            sTmp->append_ansi( anEntry->d_name );
            bool bIsDir = GFIsDirectory( sTmp );
            delete sTmp;

            return bIsDir;
         } else {
            return (anEntry->d_type == DT_DIR);
         }
      #else
         // d_type property is niet supported door de kernel, op een andere manier opvragen of het een directory is
         Groundfloor::String *sTmp = new Groundfloor::String( sCurrentDir );
         GFAppendNativeSlash( sTmp );
         sTmp->append_ansi( anEntry->d_name );
         bool bIsDir = GFIsDirectory( sTmp );
         delete sTmp;

         return bIsDir;
      #endif
   #endif
}

bool Groundfloor::Directory::isOpen() {
   return bOpen;
}


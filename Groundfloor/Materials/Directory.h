
#ifndef __GFDIRECTORY_H
#define __GFDIRECTORY_H

#include "../Atoms/Defines.h"
#include "../Atoms/Freeable.h"
#include "../Molecules/String.h"

#ifndef GF_OS_WIN32
#include <dirent.h>
#else
struct _FIX_WIN32_FIND_DATAW {
   DWORD dwFileAttributes;
   FILETIME ftCreationTime;
   FILETIME ftLastAccessTime;
   FILETIME ftLastWriteTime;
   DWORD nFileSizeHigh;
   DWORD nFileSizeLow;
   DWORD dwReserved0;
   DWORD dwReserved1;
   WCHAR cFileName[512];
   WCHAR cAlternateFileName[28];
};
#endif

namespace Groundfloor {

   /** Directory is a helper class to browse through a directory index.
    *  Be aware that there is no support for wildcards yet on the Linux platform.
    */
   class Directory : public Freeable
   {
   protected:
      String sCurrentDir;
      bool bOpen;
#ifdef GF_OS_WIN32
      bool bFirstTime;
      HANDLE hDir;
      WIN32_FIND_DATAA anEntryA;
      _FIX_WIN32_FIND_DATAW anEntryW;
#else
      DIR *hDir;
      struct dirent *anEntry;
#endif
   public:
      /// Initialize a closed index.
      Directory();
      ~Directory();

      /// Opens a directory for browsing, you need to call next() after this in order to retreive the first element. sDir should be in UTF8.
      bool open(const String *sDir);
      /// Go to the first or next element.
      bool next();
      /// Close browsing.
      bool close();

      bool isOpen();

      /// Retreive the filename of the current element in UTF8. The returned String must be freeed after using it.
      String *getFileName();
      String *getShortName();
      /// Checks whether or not the current element is a directory.
      bool isDirectory();
   };

}

#endif // __GFDIRECTORY_H


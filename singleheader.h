
#ifndef __GFInitialize_H
#define __GFInitialize_H

namespace Groundfloor {

	/// Initializes custom memory mapping system
	bool initGroundfloor();
	/// Frees custom memory mapping system
	bool finiGroundfloor();

}

#endif // __GFInitialize_H


#ifndef __GFFreeable_H_
#define __GFFreeable_H_
/***************************************************************************/

#include <cstdio>
#include <cstring>

namespace Groundfloor {

	/** TGFFreeable adds a virtual destructor, so that when
	 *  an inherited class is created, it can still be destroyed by calling
	 *  delete (TGFFreeable *)object;
	 */
	class Freeable {
	private:
		Freeable& operator=(const Freeable* s); // not allowed to make deep copies
	protected:
		virtual const char* classname() { return "Freeable"; }
	public:
		Freeable() {};
		/** a default virtual destructor with no implementation */
		virtual ~Freeable() {};

		virtual int DebugString(char *buffer, long bufferlen);
	};

}

/***************************************************************************/
#endif // __GFFreeable_H_


#ifndef __GFCALLABLE_H
#define __GFCALLABLE_H


namespace Groundfloor {

/** TGFCallable defines an abstruct function execute() that
  * other objects may call upon.
  */
class Callable: public Groundfloor::Freeable
{
    public:
         /// abstract function to implement
        virtual void execute() = 0;
};

}

#endif // __GFCALLABLE_H


#ifndef __GFMemFuncs_H
#define __GFMemFuncs_H

#include <cstdlib>

// To use the system defaults directly, use these defines
#define GFMalloc(a) malloc(a)
#define GFFree(a) free(a)
#define GFRealloc(a,c) realloc(a,c)


namespace Groundfloor {
	void nullifyMem(void *p, unsigned int count);
}

#endif //__GFMemFuncs_H


#ifndef __GFDEFINES_H
#define __GFDEFINES_H

#include <cstddef>

#if defined(__linux) || defined(macintosh) || (defined(__MACH__) && defined(__APPLE__)) || defined(__FreeBSD__) || defined(__FreeBSD) || (defined (__SVR4) && defined (__sun))
#define GF_OS_LINUX
#define GF_CP_GCC

#if defined(macintosh) || (defined(__MACH__) && defined(__APPLE__))
#define GF_OS_MAC
#define GF_OS_BSD
#endif
#if defined(__FreeBSD__) || defined(__FreeBSD)
#define GF_OS_BSD
#endif
#if (defined (__SVR4) && defined (__sun))
#define GF_OS_SOLARIS
#endif
#else
#ifdef _WIN32
#define GF_OS_WIN32

#if defined(_MSC_VER)
#define GF_CP_MVC
#endif

#ifdef __MINGW32__
#define GF_CP_MINGW
#endif
#else
#define GF_OS_UNKNOWN
#define GF_CP_UNKNOWN
#endif
#endif


#ifdef GF_OS_LINUX
#include <unistd.h>
#define GFMillisleep(a) usleep(a*1000)

	typedef long long __int64;
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned long DWORD;
#else
	// we'll never want winsock1
#define _WINSOCKAPI_
#include <windows.h>
#define GFMillisleep(a) Sleep(a)
#endif

#include <cstdio>


#define MILLISLEEP(a) DoNotUseThisFunction(a)

#define GFWIN32NEXTLINE {13,10,0}
#define GFLINUXNEXTLINE {10,0}
#define GFMACOSNEXTLINE {10,0}

#ifdef GF_OS_WIN32
#define GFNATIVENEXTLINE GFWIN32NEXTLINE
#else
#ifdef GF_OS_MAC
#define GFNATIVENEXTLINE GFMACOSNEXTLINE
#else
#define GFNATIVENEXTLINE GFLINUXNEXTLINE
#endif
#endif


	const char S_ANSI_WIN32_SLASH[] = "\\";
	const char S_ANSI_LINUX_SLASH[] = "/";

	const wchar_t S_WIDE_WIN32_SLASH[] = L"\\";
	const wchar_t S_WIDE_LINUX_SLASH[] = L"/";

#ifdef GF_OS_WIN32
#define S_ANSI_NATIVE_SLASH S_ANSI_WIN32_SLASH
#define S_WIDE_NATIVE_SLASH S_WIDE_WIN32_SLASH
#else
#define S_ANSI_NATIVE_SLASH S_ANSI_LINUX_SLASH
#define S_WIDE_NATIVE_SLASH S_WIDE_LINUX_SLASH
#endif

	//#define GFTRACING

#ifdef GFTRACING
#define GFTRACE_ASSERT(b) if ( b ) { printf( "%d:<%s>:%s(ASSERT Failed)\n", __LINE__, __FILE__, __FUNCTION__ ); }
#define GFTRACE() printf( "%d:<%s>:%s\n", __LINE__, __FILE__, __FUNCTION__ )
#define GFTRACE_D(d) printf( "%d:<%s>::%s(%d)\n", __LINE__, __FILE__, __FUNCTION__, d )
#define GFTRACE_SD(s,d) printf( "%d:<%s>::%s(%d)\n", __LINE__, __FILE__, __FUNCTION__, s, d )
#define GFTRACE_X(d) printf( "%d:<%s>::%s(%x)\n", __LINE__, __FILE__, __FUNCTION__, d )
#define GFTRACE_S(s) printf( "%d:<%s>::%s(%s)\n", __LINE__, __FILE__, __FUNCTION__, s )
#else
#define GFTRACE_ASSERT(b) /* b */
#define GFTRACE() /*  */
#define GFTRACE_D(d) /* d */
#define GFTRACE_SD(s,d) /* s d */
#define GFTRACE_X(d) /* d */
#define GFTRACE_S(s) /* s */
#endif


#endif // __GFDEFINES_H


#ifndef __GFLOCKABLE_H
#define __GFLOCKABLE_H


#ifdef GF_OS_LINUX
#include <pthread.h>
#endif

#ifdef GF_OS_WIN32
#define GFLOCK_NOWAIT 0
#define GFLOCK_INFINITEWAIT INFINITE
#else
#define GFLOCK_NOWAIT 0
#define GFLOCK_INFINITEWAIT -1
#endif

namespace Groundfloor {

	unsigned long getCurrentThreadID();

	/** TGFLockable is a simple wrapper around a native mutex.
	 *  When compiled for the Linux platform, it uses the PThread library.
	 *  The mutex is thread-orientated, so calling lock() functions within the same
	 *  thread will not cause the function to fail the second time.
	 *  If a second thread attempts to lock it, it will fail until it is unlocked by the first thread.
	 */
	class Lockable : public Freeable
	{
	protected:

#ifdef GF_OS_WIN32
		HANDLE hndMutex;
#else
		pthread_mutex_t hndMutex;
		pthread_mutexattr_t pAttr;

#if !defined(CLOCK_REALTIME) || defined(GF_OS_MAC)
		int iFunctionTime;
#endif
#endif
		int iNestingLevel;

	public:
		/// Creates a native mutex without initial ownership.
		Lockable();
		/// Frees the mutex.
		~Lockable();

		/// Tries to obtain ownership over the mutex or just fails. (same as lockWhenAvailable(GFLOCK_NOWAIT))
		bool lock();
		/// Tries to obtain ownership over the mutex, when the specified timeout (in milliseconds) has elapsed or the ownership is obtained, the function exits.
		/// The defines GFLOCK_NOWAIT or GFLOCK_INFINITEWAIT may also be used to specify iTimeout.
		bool lockWhenAvailable(int iTimeout = GFLOCK_INFINITEWAIT);
		/// Releases the mutex for other threads to access.
		bool unlock();
	};

	class LockableScope : public Freeable
	{
	protected:
		Lockable *lock;
	public:
		LockableScope(Lockable *lock);
		~LockableScope();
	};

}

#endif // __GFMUTEXABLE_H



#ifndef __GFSTRING_H_
#define __GFSTRING_H_
/***************************************************************************/

#include <iostream>


bool bytewisematch( const char *s1, const char *s2, unsigned int iLen );
bool bytewisematch_reference( const char *s1, const char *s2, unsigned int iLen );

namespace Groundfloor {

	/** String is a class that defines a variable length string
	 *   that can be manipulated and queried by member functions.
	 *   Internally it is always treated as binary data, unless a member function indicates the use of zero-terminated strings.
	 *   The string does not - and will not ever - know internally whether it contains an ansi-string or a wide-string,
	 *    therefor it is recommended using widestring functions with caution.
	 */
	class String : public Freeable {
	private:
		String& operator=(const String* s); // not allowed to make deep copies
	protected:
		const char* classname() { return "String"; }

		unsigned int strlength;
		unsigned int size;
		char *value;

	public:
		/// Initializes the string to "", length 0. Not recommended to use.
		String();
		/// Initializes the string to sValue of given length.
		String(const char *sValue, unsigned int iLength);
		String(const unsigned char *sValue, unsigned int iLength);
		String(const String *sValue);
		String(const std::string sValue);

		/// Initializes the string to sValue using strlen() for length (\\0 terminated)
		String(const char *sValue);       // ANSI
		String(const wchar_t *sValue);    // Wide (Windows UTF-16 / Linux UTF-32)

		// copy constructor
		String(const String &obj);

		~String();

		//---------------------------------------------------------------------------
		// binary-oriented base functions

		/// Copies the given binary string into itself, freeing the old string data.
		bool setValue(const char *sValue, unsigned int iLength);
		bool setValue(const unsigned char *sValue, unsigned int iLength);

		/// Appends the given binary string to the current string, adjusts the length and resizes memory space if necessary.
		bool append(const char *sValue, unsigned int iLength);
		/// Prepends the given binary string to the current string, adjusts the length and always remaps the string data.
		bool prepend(const char *sValue, unsigned int iLength);

		/// Returns whether or not the string equals the string sNeedle by length and content
		bool match(const char *sNeedle, unsigned int l) const;
		/// Returns whether or not the string starts with the substring sNeedle
		bool startsWith(const char *sNeedle, unsigned int l) const;
		/// Returns whether or not the string ends with the substring sNeedle
		bool endsWith(const char *sNeedle, unsigned int l) const;

		/// Returns the first index of the substring sNeedle, searching from iStart to the end of the string.
		int pos(unsigned int iStart, const char *sNeedle, unsigned int iNeedleLen) const;

		/// Returns the last index of the substring sNeedle, searching from 0 to the end of the string.
		/*** todo: make this function search backwards ***/
		int lastpos(const char *sNeedle, unsigned int l) const;

		//---------------------------------------------------------------------------

		bool match(const String *sNeedle) const;
		bool match(const std::string sValue) const;
		bool startsWith(const String *sNeedle) const;
		bool endsWith(const String *sNeedle) const;

		int pos(unsigned int iStart, const String *sNeedle) const;
		int lastpos(const String *sNeedle) const;

		/// Appends the given String to the current string, adjusts the length and resizes memory space if necessary.
		bool append(const String *sValue);
		/// Appends the given String of given length to the current string, adjusts the length and resizes memory space if necessary.
		bool append(const String *sValue, unsigned int iLength);

        bool append(const std::string sValue);

		/// Appends the given char/byte to the current string, adjusts the length and resizes memory space if necessary.
		bool append(char aChar);

		/// Copies the given String into itself, freeing the old string data.
		bool setValue(const String *sValue);

		bool setValue(const std::string sValue);

		/// Replace substring a with substring b
		bool replace(const String *sSubStringA, const String *sSubStringB);

		/// Remove a piece of the string from start to end;
		bool remove(unsigned int iStartPos, unsigned int iEndPos);

		/// Prepends the given String to the current string, adjusts the length and always remaps the string data.
		bool prepend(const String *sValue);
		/// Prepends the given String of given length to the current string, adjusts the length and always remaps the string data.
		bool prepend(const String *sValue, unsigned int iLength);

		/// Prepends the given char/byte to the current string, adjusts the length and resizes memory space if necessary.
		bool prepend(char aChar);

		/// Remaps the currently used memory space to a length of iSize. If iSize is shorter than the length of the string, the string is truncated.
		/** If memory failes to allocate; false is returned.
		  * Also a precautionary false and printf message is given if the size exceeds 0x3FFFFFF,
		  *  in order to avoid memory violations. Please use StringVector for big chunks of data.
		  **/
		bool setSize(unsigned int iSize);
		bool setSize_old(unsigned int iSize);

		/// Sets the length of the string, remapping the memory used if the iLength is longer than the size. Places a \\0 at the end of the string (at length + 1).
		/** Fails if internal setSize() failed.
		  **/
		bool setLength(unsigned int iLength);

		/// Returns the currently set length of the string.
		unsigned int getLength() const;
		/// Returns the currently allocated size of the string data in bytes.
		unsigned int getSize() const;


		//---------------------------------------------------------------------------

		/// Returns a pointer to the beginning of the string data that is currently used.
		/** Use this function with caution, as the location of the string data will be remapped as soon
		 * as the string is deleted or resized by one of the following functions:
		 *  setValue(), prepend(), append(), setSize(), setLength()
		 */
		char *getValue() const;
		/// Returns a pointer reinterpreted as a wchar_t*, the contents of the string remain unchanged.
		/// (To actually convert ansi to wide, use the function transformToWidestring())
		wchar_t *getValueAsWide() const;

		/// Returns a pointer to the string from given position. Use with even more caution than getValue().
		char *getPointer(unsigned int iPosition) const;

		/// Causes the length of the string to be adjusted so that there are no trailing characters of the following subset: \\r, \\n, \\t and ' '.
		void rtrim_ansi(bool bOnlyRemoveCrlf = false);
		/// left trim (\\r, \\n, \\t and ' ')
		void ltrim_ansi();

		void trimzero_ansi();

		bool match_ansi(const char *sNeedle) const;
		bool match_wide(const wchar_t *sNeedle) const;
		bool startsWith_ansi(const char *sNeedle) const;
		bool endsWith_ansi(const char *sNeedle) const;

		int pos_ansi(unsigned int iStart, const char *sNeedle) const;
		int pos_ansi(const char *sNeedle) const;

		int lastpos_ansi(const char *sNeedle) const;

		bool setValue_ansi(const char *sValue);
		bool setValue_wide(const wchar_t *sValue);

		bool append_ansi(const char *sValue);
		bool append_wide(const wchar_t *sValue);
		bool prepend_ansi(const char *sValue);
		bool prepend_wide(const wchar_t *sValue);
		bool replace_ansi(const char *sSubStringA, const char *sSubStringB);

		/// Makes all lowercase alpha characters uppercase.
		void uppercase_ansi();
		void uppercase_wide();
		/// Makes all uppercase alpha characters lowercase.
		void lowercase_ansi();
		void lowercase_wide();

		/// Transforms the current string into a wchar_t* based string based on the current locality,
		/// if bCurrentIsUtf8 is true the string is assumed to be UTF-8 instead of the current locality
		bool transformToWidestring(bool bCurrentIsUtf8, bool bFakeIfFailed = true);
		/// Transforms the current wchar_t* based string into an ansi string that matches the current locality,
		/// if bCurrentIsUtf8 is true the string is transformed into an UTF-8 string instead of using the current locality
		bool transformWidestringToString(bool bForceToUtf8, bool bFakeIfFailed = true);



		/// operators
		String& operator=(const char* s);
		String& operator=(const wchar_t* s);
		String& operator=(const std::string s);

		String& operator<<(const String *s);
		String& operator<<(const char *s);
		String& operator<<(const wchar_t *s);

		friend std::ostream& operator<<(std::ostream& Ostr, String* s);
		//friend std::ostream& operator<<(std::ostream& Ostr, String s);
	};

}

/***************************************************************************/
#endif // __GFSTRING_H_


#ifndef __GFBaseCommunicator_H
#define __GFBaseCommunicator_H


namespace Groundfloor {

	/** Basic error and statistics information that can be utilized when
	  * sending and receiving data.
	  */
	class CommReturnData : public Freeable {
	public:
		CommReturnData();
		~CommReturnData();

		bool eof;
		bool error;
		int errorcode;
		__int64 affected;
	};

	/** TGFBaseCommunicator is intended to be a base class for stream-oriented communication classes.
	 */
	class BaseCommunicator : public Freeable {
	protected:
		bool bConnected;

	public:
		/// initializes object, sets connection status to false
		BaseCommunicator();
		/// automatically disconnects before destruction
		~BaseCommunicator();

		/// returns and/or checks whether or not the communicator is still connected
		virtual bool isConnected();

		/// connect (open) the stream, returns false if the attempt failed
		virtual bool connect() = 0;
		/// disconnect (close) the stream
		virtual bool disconnect() = 0;

		/// sends data (as binary string), returns false if attempt failed, fills errData when object is given
		virtual bool send(const String *sData, CommReturnData *errData = NULL) = 0;
		/// receives data (as binary string), returns false if attempt failed, fills errData when object is given
		///  potentially overwrites contents of given data string (sData->getSize() is used as bufferlength)
		virtual bool receive(String *sData, CommReturnData *errData = NULL) = 0;
	};

}

#endif // __GFBaseCommunicator_H


#ifndef __GFThread_H_
#define __GFThread_H_
/***************************************************************************/


#ifdef GF_OS_LINUX
   #include <pthread.h>
#endif

namespace Groundfloor {

   /** Thread creates a new native thread where the execution code is called from.
    *  Add your own code by overriding the execute() function, this function gets
    *  called every given interval during the timespan of the thread.
    *  Warning: do not call stopAndWait() within the execute() running code,
    *   the thread will deadlock on this call.
    */
   class Thread : public Callable {
   protected:
      bool     bRunning;
      bool     bStartedExecutionMethod;
      bool     bShouldTerminate;
      bool     bExceptionOccurred;

#ifdef GF_OS_LINUX
      pthread_t aPThreadStruct;
#endif

      void    *pHandle;
      bool     bThreadIsCreated;

      unsigned int iCurrentInterval;
      unsigned int iDefaultSleepValue;

      void cleanup();

   public:
      /// Constructor. Initializes the interval at 100ms. The thread will not be started yet.
      Thread();
      /// Destructor. Stops the thread and exits.
      ~Thread();

      /// Returns whether or not the executionloop() is still running.
      bool isRunning();

      /// Spawns a new thread to work in.
      void start();
      /// Signals executionloop() to stop looping, and exits.
      /** Do not delete this object right after calling stop(), check for its running state first. Or better: use stopAndWait() */
      void stop();
      /// Signals executionloop() to stop looping, and waits until the executionloop has exited as well.
      void stopAndWait();

      /// Default waiting loop until the thread has stopped, do not run while in execute
      void waitWhileRunning();

      /// Waits for both threads until they've stopped, do not run while in execute
      void join(Thread *anOtherThread);

      /// Sets the interval at which the execute() function is called.
      void setInterval(unsigned int iInterval);

      /// This function must stay public, yet should not be called.
      /** It is the entrypoint of the native thread and will loop every set interval until the thread is stopped. */
      void executionloop();
   };

}

/***************************************************************************/
#endif // __GFThread_H_

#ifndef __GFRESOURCE_H
#define __GFRESOURCE_H


namespace Groundfloor {

   void generateResourceFrom( const String *source, const String *destination, const String *name );

}

#endif // __GFRESOURCE_H


#ifndef __GFDIRECTORY_H
#define __GFDIRECTORY_H


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


//---------------------------------------------------------------------------

#ifndef GFVectorH
#define GFVectorH
//---------------------------------------------------------------------------



namespace Groundfloor {

	/** TGFVector is a dynamic array with autoresize behaviour.
	 *
	 *  Note:
	 *  In every function with an index reference, there is a check if the index
	 *  is within bounds of the vector. If you don't want the vector to check this,
	 *  you can compile the class with the define GFVECTOR_NOINDEXCHECK.
	 */
	class Vector: public Freeable {
	protected:
	   Freeable **paVector;
	   unsigned int iAllocated;
	   unsigned int iElementCount;

	   unsigned int iPreAllocateCount;

	   unsigned int iRemovedCount;
   
	   unsigned int ptrsize;

	public:
	   /// To enable/disable the ability to insert an object on insertSomewhere() without checking if the removeElement() function was used before.
	   /** The default on creation is False. */
	   bool forceInsert;
	   /// To enable/disable the behaviour of auto-deleting all objects in the array on deletion.
	   /** The default on creation is True. */
	   bool autoClear;

	   /// Default constructor, sets Preallocation count on 1 and initializes the array with 0 objects.
	   Vector();
	   /// Constructor with preallocation count argument.
	   /** If a memory-remap is scheduled on addElement()
		* or insert functions, the size of the array is incremented by this Preallocation count.
		* This is intended as a speedup feature, use 1 to be more memory efficient.
		*/
	   Vector( unsigned int iPreAlloc );

	   /// Copy constructor
	   Vector(const Vector &AnotherVector);

	   /// Destructor, frees memory and optionally deletes all containing objects.
	   ~Vector();

      void setPreAlloc(unsigned int iPreAlloc);

	   /// Adds an object to the end of the array, resizing if necessary.
	   int addElement(Freeable *pElement );
	   /// Inserts the pointer to pElement at the first position that is not occupied by another element.
	   /** Resizes the vector if necessary. */
	   int insertSomewhere(Freeable *pElement );
	   /// Inserts the pointer to pElement at position iAt, shifting 0 or more elements to their index +1.
	   /** Resizes the vector if necessary. */
	   int insertAt( unsigned int iAt, Freeable *pElement );

	   /// Returns a pointer to the object that is at index i.
	   Freeable *elementAt( unsigned int i ) const;

	   /// Removes the element at index i by replacing it by a NULL-value and returns the removed object pointer.
	   /** The destructor of the element will not be called by this function. */
	   Freeable *removeElement( unsigned int i );
	   /// Replaces the element at index i by pElement and returns the object pointer that was replaced.
	   Freeable *replaceElement( unsigned int i, Freeable *pElement );

	   /// Removes (and returns) the first element in the vector, and shifts 2 to size() to the front
	   Freeable *removeFirstElementAndShiftToLeft();

	   /// Swaps the element at index i with the element at index j
	   void swapElements( unsigned int i, unsigned int j );

	   /// Removes the element obj by replacing it by a NULL-value.
	   /** The destructor of the element will not be called by this function. */
	   void removeElement( const Freeable *pElement );

	   /// Remaps the used memory for the dynamic array, truncating the array if its new size is smaller than the original size.
	   bool resizeVector( unsigned int size );
	   /// Calls the destructors of every remaining object in the array, and sets the array size to 0.
	   void clear();

	   /// Returns the index of pElement in the dynamic array. If the object was not found, -1 is returned.
	   int findElement( const Freeable *pElement ) const;

	   /// Returns the element count of the array. Removed/nilled elements are not subtracted from this count.
	   unsigned int size() const;
	   /// Returns the ammount of elements the array can contain at the moment.
	   unsigned int allocated() const;

	   /// Compresses the array by element shifting and reallocation.
	   /** Shifts out removed or nilled elements from the array and resets the element count
		* and finally remaps the array to a more enclosed allocation.
		*/
	   void compress();

	   /// Copies the used array to a newly allocated piece of memory that is assigned to the given vector.
	   /** Replaces the old mapping without calling the destructors of the old elements. */
	   bool fastCopy( Vector *into );

	};

}

//---------------------------------------------------------------------------
#endif


#ifndef __GFCallbacks_H
#define __GFCallbacks_H


#define GFCreateNotify(a,b,c,d)    new NotifyFunctionPointer<a,b>( c, d )
#define GFCreateRetreive(a,b,c,d)  new RetreiveFunctionPointer<a,b>( c, d )

namespace Groundfloor {

	// template implementaties moeten helaas in de header file, omdat iedere
	//  keer bij een nieuw type bij [new NotifyFunction<type>()] een nieuwe implementatie
	//  moet worden aangemaakt en worden gecompileerd in de uiteindelijke binary.

	template <class P>
	class NotifyAbstract : public Callable
	{
	protected:
		P aDefaultParam;
	public:
		NotifyAbstract() : Callable() {
			aDefaultParam = 0;
		}
		~NotifyAbstract() {
		}

		void setDefaultParam(P aParam) {
			aDefaultParam = aParam;
		}

		virtual void execute(P aParam) = 0;
	};

	template <class P>
	class RetreiveAbstract : public Freeable
	{
	public:
		RetreiveAbstract() : Freeable() {
		}
		~RetreiveAbstract() {
		}

		virtual P retreive() = 0;
	};



	template <class P>
	class MultiNotify : public NotifyAbstract<P>
	{
	protected:
		Groundfloor::Vector *aNotifyList;

		P aDefaultParam;
	public:
		MultiNotify() : NotifyAbstract<P>::NotifyAbstract() {
			aNotifyList = new Vector();
			aNotifyList->autoClear = true;
			aDefaultParam = 0;
		}

		MultiNotify(MultiNotify<P> *pNotify) : NotifyAbstract<P>::NotifyAbstract() {
			aNotifyList = new Vector();
			pNotify->aNotifyList->fastCopy(aNotifyList);
			aNotifyList->autoClear = false;
			aDefaultParam = pNotify->aDefaultParam;
		}

		~MultiNotify() {
			delete aNotifyList;
		}

		void setDefaultParam(P aParam) {
			aDefaultParam = aParam;
		}

		void execute() {
			execute(aDefaultParam);
		}

		void execute(P aParam) {
			unsigned int c = aNotifyList->size();
			for (unsigned int i = 0; i < c; i++) {
				NotifyAbstract<P> *obj = static_cast<NotifyAbstract<P> *>(aNotifyList->elementAt(i));
				if (obj != 0) {
					obj->execute(aParam);
				}
			}
		}

		void addNotify(NotifyAbstract<P> *aNotify) {
			aNotifyList->addElement(aNotify);
		}

		void clearNotifies() {
			aNotifyList->clear();
		}

		void removeNotify(NotifyAbstract<P> *aNotify) {
			aNotifyList->removeElement(aNotify);
		}

		NotifyAbstract<P> *getNotify(unsigned int i) {
			return static_cast<NotifyAbstract<P> *>(aNotifyList->elementAt(i));
		}

		unsigned int getNotifyCount() {
			return aNotifyList->size();
		}

		// operators
		MultiNotify& operator+= (NotifyAbstract<P> *aNotify) {
			aNotifyList->addElement(aNotify);
			return *this;
		}
	};


	template <class P, class T>
	class NotifyFunctionPointer : public NotifyAbstract<P>
	{
	protected:
		typedef void (T::*FuncCallbackOneParam)(P);

		T *pObject;
		FuncCallbackOneParam pFunction;

	public:
		NotifyFunctionPointer(T *pObject, FuncCallbackOneParam pFunction) : NotifyAbstract<P>::NotifyAbstract() {
			this->pObject = pObject;
			this->pFunction = pFunction;
		};

		~NotifyFunctionPointer() {
		};

		void execute() {
			(pObject->*pFunction)(this->aDefaultParam);
		};

		void execute(P aParam) {
			(pObject->*pFunction)(aParam);
		};
	};

	template <class P, class T>
	class RetreiveFunctionPointer : public RetreiveAbstract<P>
	{
	protected:
		typedef P(T::*FuncCallbackNoParam)();

		T *pObject;
		FuncCallbackNoParam pFunction;

	public:
		RetreiveFunctionPointer(T *pObject, FuncCallbackNoParam pFunction) : RetreiveAbstract<P>::RetreiveAbstract() {
			this->pObject = pObject;
			this->pFunction = pFunction;
		};

		~RetreiveFunctionPointer() {
		};

		P retreive() {
			return (pObject->*pFunction)();
		};
	};


}

#endif // __GFCallbackPointerClasses_H


//---------------------------------------------------------------------------

#ifndef GFStringLookupH
#define GFStringLookupH
//---------------------------------------------------------------------------


namespace Groundfloor {

   class SimpleStringLookupObject : public Freeable {
   public:
      SimpleStringLookupObject(const char *sName, Freeable *anObject);
      SimpleStringLookupObject(const String *sName, Freeable *anObject);
      ~SimpleStringLookupObject();

      String name;
      Freeable *object;
   };

   class SimpleStringLookup : public Vector {
   public:
      SimpleStringLookup();
      ~SimpleStringLookup();

      void addObjectByString(const char *sName, Freeable *anObject);
      Freeable *getObjectByString(const String *sName, bool bCaseSensitive = true);
      Freeable *getObjectByString(const char *sName, bool bCaseSensitive = true);
      void removeObjectByString(const char *sName);

      void insertObjectSortedByLength(const char *sName, Freeable *anObject, bool bAscending = true);
      void insertObjectSortedByLength(const String *sName, Freeable *anObject, bool bAscending = true);

	  void deleteAndClear();
   };

}

//---------------------------------------------------------------------------
#endif


#ifndef __GFStringVector_H
#define __GFStringVector_H


namespace Groundfloor {

	class StringVectorRange : public Freeable {
	protected:
	public:
		StringVectorRange(unsigned int a = 0, unsigned long b = 0, unsigned int c = 0, unsigned long d = 0);
		StringVectorRange(StringVectorRange *aRange);
		~StringVectorRange();

		void copyValues(StringVectorRange *aRange);

		unsigned int start_ind;
		unsigned long start_pos;
		unsigned int end_ind;
		unsigned long end_pos;
	};


	/** StringVector is a dynamic string array with autoresize behaviour.
	 *  It provides a scale of functions that allow multiple strings
	 *   to be seen as One, without requiring a single large memory block.
	 *  This class will be of benefit when dealing with large chunks of data
	 *   that need to be in memory for process.
	 *  It also allows hypothetically a larger amount data then a normal String
	 *   can store, however this remains untested.
	 *
	 *  Please note that this class does NOT natively support thread-safety, but
	 *   will not require outside locking either when used in a normal fassion.
	 *  Outside locking is recommended though when you know the thread is being
	 *   written to and read from by seperate threads.
	 *
	 *  Note that several functions on this class use entirely different arguments
	 *   and return values than most functions in the Groundfloor library.
	 */
	class StringVector : public Vector {
	protected:
	public:
		StringVector();
		~StringVector();

		/// Appends a string onto the Vector using addElement()
		/** Terminology for added strings are Chunks, hench the name.
		  * Strings are not copied, but added as pointer that will be Freed
		  *  automatically on deletion of the vector when the autoClear default
		  *  has not been changed.
		  **/
		void addChunk(String *pString);

		/// Retreives the pointer to the String given the specified chunkindex.
		/** Ranges from 0 to size() minus one **/
		String *getChunk(unsigned int iChunkIndex);

		/// Creaties a new String combined from various chunks.
		/** Chunkindex ranges from 0 to size() minus one. iStartPos is the starting
		  *  position from where the stringcopy must commence. iStartPos can be
		  *  anywhere situated in the starting Chunk or a chunk after that.
		  *  iLength is the specified length of the string to be created and thus
		  *  also the range from which must be copied. If the given iLength is
		  *  beyond the current content of the StringVector, it will be truncated
		  *  and can be compared with the String->getLength() function.
		  * The created String is not freed automatically.
		  **/
		String *copy(unsigned int iChunkIndex, unsigned long iStartPos, unsigned int iLength);

		/// Creates a new String combined from various chunks.
		/** This copy function is intended to work with the data given back by
		  *  the pos() function. Using specific chunkindices and positions, a
		  *  new String will include all data between and on the given Start
		  *  and End indices. Do not confuse the position 0 with a length of 0,
		  *  a position has a length of 1 as in a regular String. Given positions
		  *  do not have to be in range of the given chunks. The length of the
		  *  string is dynamically allocated, which makes this function more
		  *  cpu intensive than the first copy function.
		  * The created String is not freed automatically.
		  **/
		String *copy(StringVectorRange *aRange);

		/// Determines the starting and end position of a given Needle.
		/** This functions uses a pointer to a user allocated TStringVectorRange for
		  *  index storage. The start_ind and start_pos variables are [in,out] recycled
		  *  to return the starting point of the Needle. The end_ind and end_pos
		  *  variables are [out] only used to return the end of the needle, while
		  *  search range is determined by the function itself.
		  *
		  * True is returned when the Needle has been found, all integers with
		  *  pointers are filled with positioning data.
		  *
		  * False is returned when the Needle has not been found, integers
		  *  will remain unaltered.
		  **/
		bool pos(StringVectorRange *aRange, const char *sNeedle, unsigned int iNeedleLen);

		bool pos_ansi(StringVectorRange *aRange, const char *sNeedle);

		String *copyStringBetween_ansi(const char *sStrA, const char *sStrB);


		/// Returns the total accummulated length from iStartChunk till the end
		///  of the last chunk of data.
		unsigned long getLength(unsigned int iStartChunk);

		/// Returns the chunk index and position to a given flat position
		StringVectorRange *translatePosToChunkedIndex(unsigned long iPos);

		/// special function that subtracts 1 from start_pos and checks for underflow translates it to a valid position if needed
		void startMinusOne(StringVectorRange *range);
		/// special function that subtracts 1 from end_pos and checks for underflow translates it to a valid position if needed
		void endMinusOne(StringVectorRange *range);
	};

}

#endif


#ifndef _GFFifoVector_H
#define _GFFifoVector_H


namespace Groundfloor {

	/** TGFFifoVector is an extension to TGFVector to
	 *  be able to loop its own objects with a fixed vector size.
	 *  It uses TGFLockable objects to be thread-safe.
	 */
	class FifoVector : public Vector {
	protected:
		Lockable aReadLock;
		Lockable aWriteLock;
		bool bWaitingForSpace;

		int iCurrent;
		int iLastAdded;

		void init();
	public:
		FifoVector();
		FifoVector(unsigned int iPrealloc);
		~FifoVector();

		/// Checks whether or not there are still objects to pop-off the vector
		bool isEmpty();

		/// Checks whether or not the vector is full.
		/** If this is true, it might be wise to pop() something off of the vector instead of adding something. */
		bool isFull();

		/// Push an object onto the vector.
		/** If the vector is full, it will wait until there has been a pop() */
		void push(Freeable *obj);
		/// Pop an object off that was pushed first onto the vector.
		Freeable *pop();
		/// Retreive the object that is next to Pop off the vector, but only returns it and does not remove it from the vector.
		Freeable *trypop();
	};
}

#endif // _GFFifoVector_H


#ifndef __GFLOGGER_H
#define __GFLOGGER_H



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


#ifndef __GFCALLBACKTHREAD_H
#define __GFCALLBACKTHREAD_H



namespace Groundfloor {

   /** CallbackThread is a runnable thread that will call
     * the execute function of a given Callable object every given interval.
     */
   class CallbackThread : public Thread
   {
   protected:
      Callable *aCallableObject;

   public:
      CallbackThread(Callable *anObject, int iSleeptime);
      ~CallbackThread();

      /// Implementation of execute() that calls the execute() member function of another object passed at creation.
      void execute();
   };


   /** CallbackThread is a runnable thread that will call
     * the execute function of the onTimerEvent MultiNotify object every given interval.
     */
   class Timer : public Thread {
   protected:
   public:
      Timer();
      ~Timer();

      MultiNotify<Freeable *> onTimerEvent;

      /// Implementation of execute()
      void execute();
   };


   template <class P>
   class RunnableFunction : public Thread {
   protected:
      typedef void(*FuncCallbackOneParam)(P);

      FuncCallbackOneParam pFunction;
      P aParam;
   public:
      RunnableFunction(FuncCallbackOneParam pFunction) : Thread() {
         this->pFunction = pFunction;

         start();
      }
      RunnableFunction(FuncCallbackOneParam pFunction, P aParam) {
         this->pFunction = pFunction;
         this->aParam = aParam;

         start();
      }
      ~RunnableFunction() {
         stop();
      }

      P getParam() {
         return aParam;
      }

      void runWithParam(P aParam) {
         this->aParam = aParam;

         start();
      }

      void execute() {
         (*pFunction)(aParam);

         stop();
      }
   };

   template <typename P, typename R>
   class RunnableFunctionWithReturn : public Thread {
   protected:
      typedef R(*FuncCallbackOneParam)(P);
      typedef void(*FuncReturnCallbackOneParam)(R);

      FuncCallbackOneParam pFunction;
      FuncReturnCallbackOneParam pReturnFunction;
      P aParam;
      R aReturn;
   public:
      RunnableFunctionWithReturn(FuncCallbackOneParam pFunction, P aParam, FuncReturnCallbackOneParam pReturnFunction) : Thread() {
         this->pFunction = pFunction;
         this->pReturnFunction = pReturnFunction;
         this->aParam = aParam;

         start();
      }
      ~RunnableFunctionWithReturn() {
         stop();
      }

      P getParam() {
         return aParam;
      }

      R getReturnValue() {
         return aReturn;
      }

      void runWithParam(P aParam) {
         this->aParam = aParam;

         start();
      }

      void execute() {
         aReturn = (*pFunction)(aParam);
         if (pReturnFunction != 0) {
            (*pReturnFunction)(aReturn);
         }

         stop();
      }
   };

}

#endif // __GFCALLBACKTHREAD_H


#ifndef __GFProperty_H
#define __GFProperty_H


/** BasicProperty, don't use this base class.
  *  Properties are object containers with functions to get/set the object (or object-pointer).
  *  The internalSet() and internalGet() functions are public to serve the implementor functions of the
  *   get() and set() functions that may be customized by non-inheriting classes.
  **/
namespace Groundfloor {
	template <class T>
	class BasicProperty : public Freeable
	{
	protected:
		T anObject;
	public:
		/// initializes internal object to 0
		BasicProperty() : Freeable() {
			this->anObject = 0;
		}
		~BasicProperty() {
		}

		/// the default SET function (invokes internalSet())
		virtual void set(T object) {
			internalSet(object);
		}
		/// the default GET function (invokes internalGet())
		virtual T get() const {
			return internalGet();
		}

		/// sets the internal object to given object
		void internalSet(T object) {
			this->anObject = object;
		}
		/// returns the internal object
		T internalGet() const {
			return this->anObject;
		}
	};

	/// Property is the default property class to use,
	///  use it in the public section of your classes
	///  defined by for example: Property<int> myInteger;
	///  or Property<TMyObject *> myObject;
	template <class T>
	class Property : public BasicProperty<T>
	{
	protected:
		NotifyAbstract<T> *aNotify;
		RetreiveAbstract<T> *aRetreive;

		bool bAutoFreeNotify;
		bool bAutoFreeRetreive;
	public:
		/// initializes property, sets notify/retreive functions to NULL
		Property();
		/// finalizes property and deletes notify/retreive functions if AutoFree is True
		~Property();

		/// sets the notifier function that is invoked when the SET function is called
		/// notifier must conform to format [void yourfunction( <T> data )]
		/** EXAMPLE: property.setNotify( GFCreateNotify( TMyObject *, TMyClass, this, &TMyClass::myFunction ) );
		 */
		void setNotify(NotifyAbstract<T> *anEventHandler, bool bAutoFree = true);
		/// sets the retreive function that is invoked when the GET function is called
		void setRetreive(RetreiveAbstract<T> *anEventHandler, bool bAutoFree = true);

		/// implemenation SET function: calls Notify function if set, otherwise it invokes internalSet() directly
		void set(T object);
		/// implemenation GET function: calls Retreive function if set, otherwise it invokes internalGet() directly
		T get() const;
	};

	/// special class for string-properties, don't use this base class
	class BasicStringProperty : public Freeable
	{
	protected:
		String anObject;
	public:
		/// initializes internal string to ""
		BasicStringProperty();
		~BasicStringProperty();

		/// Returns a pointer to the String that is used.
		/// Do not call this function unless you really need it due to optimisation for example.
		String *link(bool bDoRetreive = true);

		/// invokes
		virtual void set(const char *object);
		virtual void set(const String *object);
		virtual char *get() const;

		unsigned int getLength();

		void internalSetCopy(const String *object);
		void internalSet(const char *object);
		char *internalGet() const;
	};

	/// String-property-class.
	/// The reason why there is a seperate property-class for strings, is because changes to the actual string
	///  cannot be tracked when a normal [Property<String *> myProperty] is used.
	/// By forcing the developer to use char * set- and get-values, we know exactly when the internal string is being changed.
	class StringProperty : public BasicStringProperty
	{
	protected:
		NotifyAbstract<const char *> *aNotify;
		RetreiveAbstract<char *> *aRetreive;

		bool bAutoFreeNotify;
		bool bAutoFreeRetreive;
	public:
		/// sets internal string to "" and sets notify/retreive functions to NULL
		StringProperty();
		~StringProperty();

		/// sets the notify function, must conform to [void myFunction( const char *myText )]
		void setNotify(NotifyAbstract<const char *> *anEventHandler, bool bAutoFree = true);
		/// sets the retreive function, must conform to [char *myFunction()]
		void setRetreive(RetreiveAbstract<char *> *anEventHandler, bool bAutoFree = true);

		/// invokes notify function if available, otherwise internalSet() is invoked directly
		void set(const char * object);
		void set(const String *object);
		/// invokes retreive function if available, otherwise internalGet() is invoked directly
		char *get() const;

		String *link(bool bDoRetreive = true);
	};

	/// base class for widechar strings, do not use this base class
	class BasicWideStringProperty : public Freeable
	{
	protected:
		String anObject;
	public:
		BasicWideStringProperty();
		~BasicWideStringProperty();

		String *link();

		virtual void set(const wchar_t *object);
		virtual void set(const String *object);
		virtual wchar_t *get() const;

		unsigned int getLength();

		virtual void internalSet(const wchar_t *object);
		virtual void internalSetCopy(const String *object);
		wchar_t *internalGet() const;
	};

	/// Special property class for Widestrings, same as StringProperty, except it forces the use of wchar_t * values.
	class WideStringProperty : public BasicWideStringProperty
	{
	protected:
		NotifyAbstract<const wchar_t *> *aNotify;
		RetreiveAbstract<wchar_t *> *aRetreive;

		bool bAutoFreeNotify;
		bool bAutoFreeRetreive;
	public:
		WideStringProperty();
		~WideStringProperty();

		void setNotify(NotifyAbstract<const wchar_t *> *anEventHandler, bool bAutoFree = true);
		void setRetreive(RetreiveAbstract<wchar_t *> *anEventHandler, bool bAutoFree = true);

		void set(const wchar_t * object);
		void set(const String *object);
		wchar_t *get() const;
	};


	//----------------------------------------------------------------------------



	template <class T>Property<T>::Property() {
		this->aNotify = 0;
		this->aRetreive = 0;
	}

	template <class T>Property<T>::~Property() {
		if ((this->aNotify != 0) && (this->bAutoFreeNotify)) {
			delete this->aNotify;
		}

		if ((this->aRetreive != 0) && (this->bAutoFreeRetreive)) {
			delete this->aRetreive;
		}
	}

	template <class T>void Property<T>::setNotify(NotifyAbstract<T> *anEventHandler, bool bAutoFree) {
		if ((this->aNotify != 0) && (this->bAutoFreeNotify)) {
			delete this->aNotify;
		}

		this->aNotify = anEventHandler;
		this->bAutoFreeNotify = bAutoFree;
	}

	template <class T>void Property<T>::setRetreive(RetreiveAbstract<T> *anEventHandler, bool bAutoFree) {
		if ((this->aRetreive != 0) && (this->bAutoFreeRetreive)) {
			delete this->aRetreive;
		}

		this->aRetreive = anEventHandler;
		this->bAutoFreeRetreive = bAutoFree;
	}

	template <class T>void Property<T>::set(T object) {
		if (this->aNotify != 0) {
			this->aNotify->execute(object);
		}
		else {
			this->internalSet(object);
		}
	}

	template <class T>T Property<T>::get() const {
		if (this->aRetreive != 0) {
			return this->aRetreive->retreive();
		}
		else {
			return this->internalGet();
		}
	}
}

//----------------------------------------------------------------------------

#endif // __GFProperty_H



#ifndef __GFBValue_H
#define __GFBValue_H

namespace Groundfloor {
   class BValue;
}



namespace Groundfloor {
   class BValue : public Freeable {
   private:
      BValue& operator=(const BValue* s); // not allowed to make deep copies
   protected:
      String sInternalValue;
      double dInternalValue;
      __int64 iInternalValue;

      void identifyBooleanString();

   public:
      /// initializes to 0-values
      BValue();
      /// initializes and copies value from anotherValue
      BValue(BValue *anotherValue);
      ~BValue();

      /// set to a FLDTYPE_X constant depending on what SET function was used last
      Property<int> lasttypeset;
      /// precision of double value to reflect in automatically generated string representations
      Property<int> precision;

      /// sets data to match the data in anotherValue
      void setValue(const BValue *anotherValue);
      /// compares data to anotherValue and returns true if their equal
      bool equals(const BValue *anotherValue) const;

      /// returns String representation of set value
      String *asString();
      /// returns integer representation of set value
      long asInteger() const;
      /// returns int64 representation of set value
      __int64 asInt64() const;
      /// returns double representation of set value
      double asDouble() const;
      /// returns boolean representation of set value
      bool asBoolean() const;
      /// returns 'timestamp' representation of set value (currently same as asInteger(), but unsigned)
      unsigned int asTimestamp() const;

      /// sets current value to string, tries to automatically extract integer representation and boolean representation
      void setString(const char *sValue);
      /// sets current value to string, tries to automatically extract integer representation and boolean representation
      void setString(const char *sValue, unsigned int iLength);
      /// sets current value to string, tries to automatically extract integer representation and boolean representation
      void setString(const String *sValue);
      /// sets integer value, automatically sets string representation to an ascii translation of the integer
      void setInteger(long iValue);
      /// sets int64 value (and truncated integer value), automatically sets string representation to an ascii translation of the integer (currently only 32bits of the value)
      void setInt64(__int64 iValue);
      /// sets double value, automatically sets string representation to an ascii translation of the double, uses precision for number of decimals, uses '.' as seperator
      void setDouble(double dValue);
      /// sets boolean value (integer 1 when true, 0 when false), automatically sets string representation to 'true' or 'false'
      void setBoolean(bool bValue);
      /// sets timestamp value, automatically sets string representation to standard date format "yyyy-mm-dd hh:mm:ss" or "yyyy-mm-ddThh:mm:ss" when bXml is set true
      void setTimestamp(__int64 iValue, bool bXml = false);



      // operators
      friend void operator<<(std::ostream& Ostr, const BValue* s) {
         Ostr << s->sInternalValue.getValue();
      }
      friend void operator<<(String& Ostr, const BValue* s) {
         Ostr.append(&s->sInternalValue);
      }
      /*
            friend void operator<<(BValue& Ostr, const char* s) {
               Ostr.append_ansi( s );
            }
      */
   };
}

#endif // __GFBValue_H



#ifndef __GFBAttribute_H
#define __GFBAttribute_H

namespace Groundfloor {
   class FBAttribute;
}


namespace Groundfloor {
   /// attribute as used in BNode, following a key=value format.
   class BAttribute : public Freeable {
   public:
      BAttribute();
      BAttribute(const BAttribute *attrib);
      BAttribute(const char *sKey);
      BAttribute(const char *sKey, const char *sValue);
      BAttribute(const char *sKey, const String *sValue);
      BAttribute(const char *sKey, bool bValue);
      BAttribute(const char *sKey, long iValue);
      BAttribute(const char *sKey, double iValue, short iDecimal);
      ~BAttribute();

      String key;
      BValue value;
      bool hideFromXML;
   };
}

#endif // __GFBAttribute_H


#ifndef __GFFileCommunicator_H
#define __GFFileCommunicator_H

namespace Groundfloor {
	class FileCommunicator;
}


#include <cstdio>

#define GFFILEMODE_READ    1
#define GFFILEMODE_WRITE   2
#define GFFILEMODE_APPEND  4

namespace Groundfloor {

	/** Communicator implementation for Files
	 */
	class FileCommunicator : public BaseCommunicator {
	protected:
		FILE *fp;
		Lockable lockReadWrite;

	public:
		FileCommunicator();
		~FileCommunicator();

		/// filename is assumed to be UTF-8
		StringProperty filename;
		Property<int> mode;
		Property<bool> flushdirectly;

		/// opens file
		bool connect();
		/// closes file
		bool disconnect();

		/// writes data to file
		bool send(const String *sData, CommReturnData *errData = NULL);
		/// reads data from file (sData->getSize() is used as bufferlength)
		bool receive(String *sData, CommReturnData *errData = NULL);
	};
}

#endif // __GFFileCommunicator_H


#ifndef __GFBAnnounce_H
#define __GFBAnnounce_H

namespace Groundfloor {
   class BNode;
   class BNodePath;
   class BField;
}

#endif // __GFBAnnounce_H

#ifndef __GFBFields_H
#define __GFBFields_H


namespace Groundfloor {
   class BFields : public Vector {
   public:
      BFields();
      ~BFields();

      /// returns field index with given name, -1 if not found
      int getFieldIndex(const char *sName, unsigned int len);
      /// returns field index with given name, -1 if not found
      int getFieldIndex_ansi(const char *sName);
      /// returns field index with given name, -1 if not found
      int getFieldIndex(const String *sName);

      /// returns field at given index
      BField *getField(unsigned int iIndex);

      /// appends a newly created field, usage not recommended
      BField *newField();
      BField *newField(char *n, int dt);
   };
}

#endif // __GFBFields_H



#ifndef __GFBNode_H
#define __GFBNode_H

#include <vector>

namespace Groundfloor {
   class BNodePath : public Freeable {
   protected:
      std::vector<unsigned int> path;

      int current;
   public:
      /// path initialization to length zero
      BNodePath();
      ~BNodePath();

      /// appends i to path
      void append(unsigned int i);
      /// clears entire path vector
      void clear();

      /// resets path pointer
      unsigned int traverse_reset();

      /// returns previous value
      unsigned int traverse_previous();
      /// returns first or next value
      unsigned int traverse_next();

      /// returns true if path pointer is at the beginning of the path
      bool traverse_isBoF();
      /// returns true if path pointer is at the end of the path, or when the path length is zero
      bool traverse_isEoF();

      void readFromString(const char *sPath);
   };

   /// BNode is a class tailored to represent simple XML Nodes containing attributes in key=value format, and nesting of child-nodes
   class BNode : public Freeable {
   protected:
      Vector *arrAttributes;
      Vector *arrChildren;
      BNode *parent;

      unsigned int iChildrenPreAlloc;
      unsigned int iAttribPreAlloc;

   public:
      /// initialize node with no name
      BNode(unsigned int iChildrenPreAlloc = 7);
      /// initialize node with given name
      BNode(const char *sName, unsigned int iChildrenPreAlloc = 7);
      /// automatically deletes all attributes and child-nodes
      ~BNode();

      String name;
      String content;

      BNode *getParentNode();

      /// adds attribute to the node, given attribute is automatically deleted
      void addAttribute(BAttribute *anAttrib);
      /// adds child-node to the node, given node is automatically deleted
      void addChildNode(BNode *aNode);

      /// number of child-nodes
      unsigned int childCount() const;
      /// number of attributes
      unsigned int attributeCount() const;

      /// copy attributes from given node
      void copyAttributes(BNode *aNode);

      /// returns attribute at index i, returns NULL when out of bounds
      BAttribute *getAttribute(unsigned int i) const;
      /// returns attribute if found by name
      BAttribute *getAttributeByName(const char *sName) const;
      /// returns node at index i, returns NULL when out of bounds
      BNode *getChild(unsigned int i) const;

      /// returns node that has the given name, or NULL
      BNode *getChildByName(const char *sName) const;

      /// returns node that has the given name and where the attribute with given name has the given value
      BNode *getChildByNameAndAttribute(const char *sName, const char *sAttributeName, const BValue *sAttributeValue) const;

      /// returns node that contains the given attribute with the given value, searches the complete tree
      BNode *findAttributeThroughRecursiveSearch(const char *sAttributeName, const BValue *sAttributeValue);

      /// find or create and add an attribute with given name
      BAttribute *findOrCreateAttribute(const char *sName);

      /// traverses 1 position in given NodePath and uses current path reference as index, returns NULL if anything went wrong
      BNode *getChildAndTraverse(BNodePath *aPath) const;

      /// traverses the whole path and returns the last valid child-node in the path, if no valid nodes are found in the path; "this" is returned
      BNode *getChildByPath(BNodePath *aPath);

      /// links pointer to content from either the first child, or itself if no children
      String *getFirstChildsContent();

      /// flatten all child contents into a newly allocated string
      String *flattenChildContent();

      void unlinkChild(BNode *aChild);
   };
}

#endif // __GFBNode_H


#ifndef __GFBField_H
#define __GFBField_H


#define FLDTYPE_STRING 0
#define FLDTYPE_INT 1
#define FLDTYPE_DOUBLE 2
#define FLDTYPE_DATE 3
#define FLDTYPE_INT64 4
#define FLDTYPE_BOOL 5
#define FLDTYPE_UNKNOWN 255

#define FLDFLAG_NORMAL 0
#define FLDFLAG_PRIMARY 1


namespace Groundfloor {
   class BField : public Freeable {
   public:
      /// intializes field to unnamed, datatype -1, no maximum length, flagged normal
      BField();
      BField(char *n, int dt);
      ~BField();

      /// name of field
      String name;
      /// datatype of data
      Property<int> datatype;
      /// maximum length of data, set to -1 when no maximum length needed
      Property<int> maxlength;
      /// flags belonging to this field
      Property<int> flags;
   };
}

#endif // __GFBField_H



#ifndef _GFThreadedBuffer_H
#define _GFThreadedBuffer_H


namespace Groundfloor {

   /** TGFThreadedBuffer implements a Thread that will process objects one-by-one
    *  which are stored into a GFFifoVector.
    */
   class ThreadedBuffer : public Thread {
   protected:
      Lockable lockBuffer;
      FifoVector aBuffer;
      bool bDeleteAfterProcess;
      bool bStopWhenEmpty;
   public:
      ThreadedBuffer();
      ~ThreadedBuffer();

      /// To add a new object to the vector that should be processed.
      void add(Freeable *obj);
      /// Thread execute function, this is internally used and should not be called.
      void execute();

      /// Checks whether or not the Fifovector is empty.
      bool isEmpty();
      /// Checks whether or not the Fifovector is full.
      /** If the fifovector is full, it is wise to stop adding objects or start processing objects - in order to prevent a deadlock. */
      bool isFull();

      /// If you want the execute function to delete the object that was processed succesfully, set this to true. However it is recommended you delete the object yourself.
      void setDeleteAfterProcess(bool bDelete);

      /// By Default this is fault, set it to true when you have filled the buffer so the thread will stop when the buffer is emptied
      /** Use this if you want to be able to use thread functions like join() or others without having to check if the buffer is empty yourself */
      void setStopWhenEmpty(bool bSet);

      /// Function to be implemented. It is recommended to always return True.
      virtual bool processObject(Freeable *obj) = 0;
   };


}

#endif // _GFThreadedSender_H



#ifndef __GFFileWriter_H
#define __GFFileWriter_H



namespace Groundfloor {

   class BaseWriter : public ThreadedBuffer {
   public:
      BaseWriter();
      ~BaseWriter();

      /// obj is assumed to be a TGFString that is copied to a new TGFString and placed in the buffer
      virtual void add(const Freeable *obj);
   };

   class StringWriter : public BaseWriter {
   public:
      StringWriter();
      ~StringWriter();

      StringVector Output;

      bool processObject(Freeable *obj);
   };

   /** TGFFileWriter is a Threaded Buffer that periodically writes
     *  data from the buffer to a file.
     * Warning: if you prematurely delete this object, data might be lossed.
     *  To prevent loss of data, implement a protection loop by e.g.: while ( !filewriter.isEmpty() ) { GFMilliSleep(100); );
    */
   class FileWriter : public BaseWriter {
   protected:
      FileCommunicator file;
      bool bAppendmode;

   public:
      FileWriter();
      ~FileWriter();

      void setFlushDirectly(bool yesno);

      /// opens file in Write mode (or Append mode if bAppendMode is true), sFile must be in UTF8
      void open(const String *sFile, bool bAppendmode = false);
      /// opens file in Write mode (or Append mode if bAppendMode is true), sFile must be in UTF8
      void open(const char *sFile, bool bAppendmode = false);
      /// closes file (warning: does not automatically flush buffer!)
      void close();

      bool processObject(Freeable *obj);
   };

}

#endif // __GFFileWriter_H



#ifndef __GFBRecord_H
#define __GFBRecord_H


namespace Groundfloor {
   class BRecord : public Vector {
   public:
      BRecord();
      ~BRecord();

      /// appends a new empty value to record, usage not recommended
      BValue *newValue();
      /// returns value by given index
      BValue *getValue(unsigned int iIndex);

      /// creates or deletes values to match the given number of values
      void setValueCount(unsigned int iCount);

      /// returns true if the integer representation of the value at iColumn equals iNeedle
      bool matchInteger(unsigned int iColumn, int iNeedle);
   };
}

#endif // __GFBRecord_H



#ifndef __GFBRecords_H
#define __GFBRecords_H


namespace Groundfloor {
   class BRecords : public Vector {
   public:
      // initializes empty vector of records
      BRecords();
      ~BRecords();

      /// returns record at given index
      BRecord *getRecord(unsigned int iIndex);
      /// appends a new empty record, usage not recommended
      BRecord *newRecord();

      /// Looks up first record where given columnindex equals the given integer value iNeedle.
      ///  This is primarily added to simplify ID-field lookups.
      BRecord *findRecordByInteger(unsigned int iColumn, int iNeedle);
   };
}

#endif // __GFBRecords_H



#ifndef __GFBTable_H
#define __GFBTable_H



namespace Groundfloor {
   /// represents a table with fields and records
   class BTable : public Freeable {
   public:
      /// Fields and records are initialized to NULL,
      ///  setting the fields and records must be done manually.
      /// Recommended: Try to fill the fields and records before setting properties.
      BTable();
      ~BTable();

      Property<BFields *> fields;
      Property<BRecords *> records;
   };
}

#endif // __GFBTable_H



#ifndef __AbstractSquirrel_H
#define __AbstractSquirrel_H





namespace Groundfloor {

   /// commreturndata extension for errorstring
   class SquirrelReturnData : public CommReturnData {
   public:
      String errorstring;
   };

   class AbstractSquirrelConnection : public Freeable {
   protected:
      bool bConnected;
   public:
      AbstractSquirrelConnection() : Freeable() {
         bConnected = false;
      };
      ~AbstractSquirrelConnection() {
      };

      /// connects
      virtual bool connect() = 0;
      /// disconnects
      virtual bool disconnect() = 0;

      virtual bool isConnected() {
         return bConnected;
      }
   };


   /// base class for database connections
   /** The generic way of using this class to traverse through records is:
     *
     * mySquirrel->open();
     * mySquirrel->fetchFields( myFields );
     * while ( mySquirrel->next() ) {
     *    mySquirrel->fetchRecord( myRecord );
     * }
     * mySquirrel->close();
     */
   class AbstractSquirrel : public Freeable {
   protected:
      AbstractSquirrelConnection *connection;
   public:
      AbstractSquirrel(AbstractSquirrelConnection *pConnection) : Freeable() {
         this->connection = pConnection;
      };
      ~AbstractSquirrel() {
      };

      /// opens 'resource' (a table, a query, depending on implementation)
      virtual bool open(SquirrelReturnData *errData = NULL) = 0;
      /// closes 'resource'
      virtual bool close() = 0;

      /// returns the number of individual rows or records (after open())
      virtual __int64 getRecordCount() = 0;

      /// returns true if a 'resource' has been opened succesfully
      virtual bool isOpen() = 0;

      /// returns true if the current row/record is the First row/record
      virtual bool isFirst() = 0;
      /// returns true if the current row/record is the Last row/record
      virtual bool isLast() = 0;

      /// sets current row/record pointer to the first or next row/record.
      virtual bool next() = 0;

      /// returns the fields selected into the resource, should only be called once
      virtual bool fetchFields(BFields *aFields) = 0;
      /// returns the current row/record
      virtual bool fetchRecord(BRecord *aRecord) = 0;
   };

}

#endif // __AbstractSquirrel_H



#ifndef __RemoteSquirrel_H
#define __RemoteSquirrel_H


namespace Groundfloor {
   /// specialized Squirrel implementation for Remote Databases
   class RemoteSquirrelConnection : public AbstractSquirrelConnection {
   public:
      RemoteSquirrelConnection();
      ~RemoteSquirrelConnection();

      /// hostname (implementation should automatically resolve this into an IP if needed)
      StringProperty host;
      /// port to connect to on the host
      Property<int> port;
      /// username to use when logging in to the server
      StringProperty username;
      /// password to use
      StringProperty password;
   };

   /// for Local Databases
   class LocalSquirrelConnection : public AbstractSquirrelConnection {
   public:
      LocalSquirrelConnection();
      ~LocalSquirrelConnection();

      StringProperty path;
   };


   /// specialized Squirrel implementation for Remote Databases
   class RemoteSQL : public AbstractSquirrel {
   protected:
      SimpleStringLookup params;
      String sCurrentQuery;

      virtual void prepareQuery();

      /// replaces occurences of sToken (parameters) with the given value
      /** depending on the lastsettype of the value the function should escape the value according to the specific database's rules, setting bIncludeQuotation to false is not recommended, as it could negate portability */
      virtual void replaceInQuery(const char *sToken, BValue *aValue, bool bIncludeQuotations = true) = 0;
   public:
      RemoteSQL(RemoteSquirrelConnection *pConnection);
      ~RemoteSQL();

      /// sets the current query to sQuery
      virtual void setQuery(const String *sQuery, bool bReplaceBackticksWithDoubleQuotes = true);

      /// returns ptr to current query
      virtual String *getCurrentQuery();

      virtual BValue *findOrAddParam(const char *sToken);
      virtual BValue *findOrAddParam(const String *sName);

      /// return (if possible) the last id that was inserted or updated by use of an autonumbering type or sequence
      /** this function should only return the id that was used last in this exact session/connection */
      virtual __int64 getLastID(const char *sTableName, SquirrelReturnData *errData = NULL) = 0;
   };
}

#endif // __RemoteSquirrel_H



#ifndef _GFFunctions_H_
#define _GFFunctions_H_


namespace Groundfloor {

   /// returns position of c in string s, or -1 if not found
   int strpos(const char *s, char c);

   /// converts sInput (binary data) to Base64, overwriting sOutput
   void StrToBase64(const String *sInput, String *sOutput);

   /// converts sInput (binary data) to Ascii85, overwriting sOutput
   void StrToAscii85(const String *sInput, String *sOutput);

   /// appends the 2-character-hexadecimal-value of c to sOutput
   void CharToHex(char c, String *sOutput);
   /// overwrites sOutput with full per-byte hexidecimal values, values per byte are not seperated
   void StrToHex(const String *sInput, String *sOutput);

   /// overwrites sOutput with full bit-to-ascii conversion of sInput, set bits are translated into '1', unset bits are '0'
   /// Bits are ordered from MSB to LSB (big endian).
   void StrToBin(const String *sInput, String *sOutput);

   /// prepends the hexadecimal ascii representation of i, length depends on compiler implementation of long (32 bits or 64 bits)
   void LongToHex(unsigned long i, String *sOutput);
   /// returns the integer value represented in the hexadecimal string sInput
   unsigned int HexToInt(const String *sInput);

   /// simple implementation of compressing sInput to binary tree sOutputData and per-node translation sOutputTable.
   /// only compresses single characters/bytes.
   //void StrToCharHuffman( const String *sInput, String *sOutputData, String *sOutputTable );
   void StrToCharHuffman(const String *sInput, String *sOutputData, String *sOutputTable1, String *sOutputTable2);

   /// returns original string from binary tree data and translation table
   void CharHuffmanToStr(const String *sInputData, const String *sInputTable1, const String *sInputTable2, String *sOutputString);

   void EncodeUrlEntities(String *s);

   void EncodeHtmlEntities(String *s);
   void DecodeHtmlEntities(String *s);

   ////////////////////////////////////////////////////////////////////////////////

   /// returns filesize of sFilePath, assuming UTF8 string
   unsigned long GetFileSize(const String *sFilePath);

   /// returns true if file exists, assuming UTF8 string
   bool FileExists(const String *sFilePath);
   /// returns true if path is a directory, assuming UTF8 string
   bool IsDirectory(const String *sFilePath);

   /// creates new directory
   bool CreateDir(const String *sFilePath);

   /// returns current directory represented as UTF8 string
   String *GetCurrentDir();

   String *createHybridString(const void *s, bool bWide);
   void PrependNativeSlash(String *sPath, bool bWide = false);
   void AppendNativeSlash(String *sPath, bool bWide = false);
   void ConvertToNativeSlashes(String *sPath, bool bWide = false);

   void ConvertToURISlashes(String *sPath, bool bWide = false);

   /// returns timestamp representing the current date and time
   __int64 GetTimestamp();

   /// timestamp to string following the given format (http://www.cplusplus.com/reference/ctime/strftime/)
   String *TimestampToStr(const char *format, __int64 timestamp);

   /// translates string into timestamp, acceptable formats are "hh:mm", "hh:mm:ss", "yyyy-mm-dd", "yyyy-mm-dd hh:mm" or "yyyy-mm-dd hh:mm:ss"
   /** bTZCorrection can be optionally set to False to Not include a correction that counters the behaviour - of the functions that are used - to translate the timestamp to the local timezone.
        unless you know what you're doing, you should leave these at their default True values to get proper date translations that are compatible with BValue's dates and timestamps */
   __int64 DateStrToTimestamp(String *sDate, bool bTZCorrection = true, bool bDaylightSavingsCorrection = true);


   //------------------------------------------------------------------------------

   /// Splits given string sHaystack into new strings that are stored in a new Vector,
   ///  manual deletion of returned vector is required.
   Vector *split(const String *sHaystack, const char *sNeedle, int iMax = -1);

   void split_p(Vector *pSlicesVector, const String *sHaystack, const char *sNeedle, int iMax = -1);

   Vector split_m(const String *sHaystack, const char *sNeedle, int iMax = -1);

   /// Determines whether given string sNeedle solely contains numbers between 0 and 9
   bool isNumber(const char *sNeedle, unsigned int len);

   /// returns the length in bytes of given widestring
   unsigned int getWStrByteLen(const wchar_t *s);

}

#endif //_GFFunctions_H_


#ifndef __GFCsvSettings_H
#define __GFCsvSettings_H


namespace Groundfloor {
   class CsvSettings : public Freeable {
   public:
      CsvSettings();
      ~CsvSettings();

      Property<bool> show_columnnames;

      String sep_column;
      String sep_record;
      String sep_string;
   };

   class XmlSettings : public Freeable {
   public:
      XmlSettings();
      ~XmlSettings();

      String node_open;
      String node_close;
      String node_space;
      String node_end;
      String sep_node;
      String sep_string;
      String sep_keyval;
   };
}

#endif // __GFCsvSettings_H



#ifndef __BFunctions_H
#define __BFunctions_H



namespace Groundfloor {
   void BCsvWrite_Table(BaseWriter *aWriter, const BTable *aSource, CsvSettings *aSettings);
   void BCsvExport_Table(const String *sDestination, const BTable *aSource, CsvSettings *aSettings);

   void BXmlWrite_Node(BaseWriter *aWriter, BNode *aSource, XmlSettings *aSettings, bool bWriteDefaultHeader);
   void BXmlExport_Node(const String *sDestination, BNode *aSource, XmlSettings *aSettings, bool bWriteDefaultHeader);

   BNode *BXmlImport_FromString(const String *sXML);
   BNode *BXmlImport_FromFile(const String *sSourceFile);

   BNode *BCsvImport_FromFile(const String *sSourceFile, const CsvSettings *aSettings);

   /// lossy function to try and generate a table from Nodes (attributes are lost, contents are translated to recordvalues)
   BTable *BNodeContentsToTable(const BNode *aNode, int iDefaultDatatype = FLDTYPE_STRING);
   /// lossy function to try and generate a table from Nodes (attributes are translated to recordvalues, contents are lost)
   BTable *BNodeAttributesToTable(const BNode *aNode, int iDefaultDatatype = FLDTYPE_STRING);

   /// semi-lossless function to generate Nodes from a table (every recordvalue is a seperate node)
   BNode *BTableToNodeContents(const BTable *aTable, const char *sTableNodeName, const char *sRecordNodeName);
   /// semi-lossless function to generate Nodes from a table (every recordvalue is a seperate attribute)
   BNode *BTableToNodeAttributes(const BTable *aTable, const char *sTableNodeName, const char *sRecordNodeName);
}

#endif // __BFunctions_H


#ifndef __GFGarbageCollector_H
#define __GFGarbageCollector_H




#define GC_LIVEFOREVER 4294967295UL

namespace Groundfloor {

   /** GarbageStamp class
    */
   class GarbageStamp : public Freeable {
   protected:
      __int64 iCreationTime;
      __int64 iDeletionTime;
      Freeable *anObject;

   public:
      GarbageStamp(Freeable *anObject, __int64 iTimeToLive);
      ~GarbageStamp();

      __int64 getCreationTime();
      __int64 getDeletionTime();
   };

   /** GarbageCollector class.
     * The GarbageCollector is not a typical kind of garbage collection mechanism,
     *  does not depend on usage or references to the object, but on a predefined TTL (time-to-live).
     * Therefor the garbage collector should not be used on a regular basis, but only in situations
     *  where manually deleting an object is either not possible or too complicated.
     */
   class GarbageCollector : public Thread {
   protected:
      Vector aGarbageCollection;
      unsigned int iDefaultTimeToLive;

      unsigned int iObjectsAdded;
      unsigned int iObjectsDeleted;

   public:
      /// initializes vector, variables and sets the default time-to-live to 5 seconds
      GarbageCollector();
      /// automatically deletes all objects
      ~GarbageCollector();

      /// adds a new GarbageStamp with given object to the vector.
      /// If given iTimeToLive is set to GC_LIVEFOREVER, the object will be kept
      ///  alive untill the garbagecollector is deleted.
      void addToQueue(Freeable *anObject, unsigned int iTimeToLive = 0);

      /// the default ttl that is used when no custom ttl is given
      void setDefaultTimeToLive(unsigned int iDefaultTimeToLive);

      unsigned int getItemsAdded();
      unsigned int getItemsDeleted();

      /// implementation of thread-loop, disposing of garbage when timers have expired
      void execute();
   };


   /// instantiates the primary GarbageCollector object
   bool initGlobalGarbageCollector(unsigned int iInterval = 5000);
   /// deletes the garbagecollector, freeing all remaining garbage
   void finiGlobalGarbageCollector();

   /// returns the number of objects added to the garbagecollector
   unsigned int GarbageStats_itemsAdded();
   /// returns the number of objects that have been disposed
   unsigned int GarbageStats_itemsDeleted();


   /// adds the given object to the garbagecollector, return the same object
   Freeable *Disposable(Freeable *object, unsigned int iTTL = 0);
   /// adds the given String-object to the garbagecollector, return the same object
   String *Disposable(String *object, unsigned int iTTL = 0);

   /// creates a new String from given str, adds it to the garbagecollector, and returns the object
   String *DisposableStr(const char *str, unsigned int iTTL = 0);

   /// creates a new BValue from given str, adds it to the garbagecollector, and returns the object
   BValue *DisposableStrValue(const char *str, unsigned int iTTL = 0);

}

#endif // __GFGarbageCollector_H



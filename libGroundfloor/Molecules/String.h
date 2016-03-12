
#ifndef __GFSTRING_H_
#define __GFSTRING_H_
/***************************************************************************/

#include <iostream>

#include "../Atoms/Freeable.h"
#include "../Atoms/Defines.h"

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
		String(const String *sValue);

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
		bool startsWith(const String *sNeedle) const;
		bool endsWith(const String *sNeedle) const;

		int pos(unsigned int iStart, const String *sNeedle) const;
		int lastpos(const String *sNeedle) const;

		/// Appends the given String to the current string, adjusts the length and resizes memory space if necessary.
		bool append(const String *sValue);
		/// Appends the given String of given length to the current string, adjusts the length and resizes memory space if necessary.
		bool append(const String *sValue, unsigned int iLength);

		/// Appends the given char/byte to the current string, adjusts the length and resizes memory space if necessary.
		bool append(char aChar);

		/// Copies the given String into itself, freeing the old string data.
		bool setValue(const String *sValue);

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

		String& operator<<(const String *s);
		String& operator<<(const char *s);
		String& operator<<(const wchar_t *s);

		friend std::ostream& operator<<(std::ostream& Ostr, String* s);
		//friend std::ostream& operator<<(std::ostream& Ostr, String s);
	};

}

/***************************************************************************/
#endif // __GFSTRING_H_

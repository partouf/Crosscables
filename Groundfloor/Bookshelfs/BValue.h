
#ifndef __GFBValue_H
#define __GFBValue_H

namespace Groundfloor {
   class BValue;
}

#include "../Atoms/Freeable.h"
#include "../Molecules/String.h"
#include "../Molecules/Property.h"

#include "../Atoms/Defines.h"

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


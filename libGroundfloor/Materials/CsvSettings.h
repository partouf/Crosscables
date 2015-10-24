
#ifndef __GFCsvSettings_H
#define __GFCsvSettings_H

#include "../Atoms/Freeable.h"
#include "../Molecules/Property.h"
#include "../Molecules/String.h"

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


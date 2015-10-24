
#include "CsvSettings.h"

Groundfloor::CsvSettings::CsvSettings() {
   show_columnnames.set( false );

   sep_column.setValue_ansi( ";" );
   sep_record.setValue_ansi( "\r\n" );
   sep_string.setValue_ansi( "\"" );
}

Groundfloor::CsvSettings::~CsvSettings() {
}

Groundfloor::XmlSettings::XmlSettings() {
   node_open.setValue_ansi( "<" );
   node_close.setValue_ansi( ">" );
   node_end.setValue_ansi( "/" );
   node_space.setValue_ansi( " " );
   sep_string.setValue_ansi( "\"" );
   sep_keyval.setValue_ansi( "=" );

   sep_node.setValue_ansi( "\r\n" );
}

Groundfloor::XmlSettings::~XmlSettings() {
}

/*****************************************************************************
 * File Name:	 TableConvert.c
 * Description:	 Generic XtRString to Enum Conversion Using Tables
 * Author:	 Simon Leinen  <simon@leinen.ch>
 * Date Created:  5-Jul-91
 ****************************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include "TableConvert.h"

#if NeedFunctionPrototypes
static Boolean table_convert(Display*,XrmValue*,Cardinal*,XrmValue*,XrmValue*,XtPointer*);
#else /* not NeedFunctionPrototypes */
static Boolean table_convert();
#endif /* not NeedFunctionPrototypes */

typedef struct _InternalLookupTableEntryRec
{
  XrmQuark	quark;
  int		value;
}
InternalLookupTableEntryRec, * InternalLookupTableEntry;

typedef struct _InternalLookupTableRec
{
  InternalLookupTableEntryRec
	      * entries;
  Cardinal	n_entries;
}
InternalLookupTableRec, * InternalLookupTable;

extern void
set_table_converter(to_type,entries,n_entries)
     String to_type;
     LOOKUP_TABLE_ENTRY * entries;
     Cardinal n_entries;
{
  Cardinal k;
  InternalLookupTable internal_lookup_table;
  XtConvertArgList arglist = XtNew(XtConvertArgRec);

  internal_lookup_table = XtNew(InternalLookupTableRec);
  internal_lookup_table->n_entries = n_entries;
  internal_lookup_table->entries
    = (InternalLookupTableEntryRec *)
      XtCalloc(n_entries,sizeof(InternalLookupTableEntryRec));
  for (k=0; k<n_entries; ++k)
    {
      char buffer[1000];
      XmuCopyISOLatin1Lowered(buffer,entries[k].name);
      internal_lookup_table->entries[k].quark = XrmStringToQuark(buffer);
      internal_lookup_table->entries[k].value = entries[k].value;
    }
  arglist[0].address_mode = XtImmediate;
  arglist[0].address_id = (XtPointer) internal_lookup_table;
  arglist[0].size = sizeof internal_lookup_table;
  XtSetTypeConverter(XtRString,to_type,table_convert,
		     arglist,1,
		     XtCacheNone,(XtDestructor)0);
}

static Boolean
table_convert(display,args,n_args,from,to,converter_data)
     Display * display;
     XrmValue * args;
     Cardinal * n_args;
     XrmValue * from;
     XrmValue * to;
     XtPointer * converter_data;
{
  unsigned k;
  char buffer[1000];
  InternalLookupTable table = * (InternalLookupTable *) args[0].addr;
  XrmQuark quark;

  XmuCopyISOLatin1Lowered(buffer,(String) from->addr);
  quark = XrmStringToQuark(buffer);

  for (k = 0; k < table->n_entries; ++k)
    if (quark==table->entries[k].quark)
      {
	if (to->addr!=0)
	  {
	    if (to->size < sizeof(int))
	      {
		to->size = sizeof(int);
		return False;
	      }
	    *(int *) to->addr = table->entries[k].value;
	  }
	else
	  {
	    to->addr = (XtPointer) & table->entries[k].value;
	    to->size = sizeof(int);
	  }
	return True;
      }
  XtDisplayStringConversionWarning
    (display,(char *)from->addr,"AnEnumerationType");
  return False;
}

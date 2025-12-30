/*****************************************************************************
 * File Name:	 TableConvert.h
 * Description:	 Generic XtRString to Enum Conversion Using Tables
 * Author:	 Simon Leinen  <simon@leinen.ch>
 * Date Created:  5-Jul-91
 ****************************************************************************/

#ifndef _TableConvert_h_
#define _TableConvert_h_

#include <X11/Xmu/Xmu.h>

struct lookup_table_entry_ {
  char *name;
  int value;
  } ;
typedef struct lookup_table_entry_ LOOKUP_TABLE_ENTRY;

#if NeedFunctionPrototypes
extern void set_table_converter(String,LOOKUP_TABLE_ENTRY *,Cardinal);
#else /* not NeedFunctionPrototypes */
extern void set_table_converter(/*String,LOOKUP_TABLE_ENTRY *,Cardinal*/);
#endif /* not NeedFunctionPrototypes */

#endif /* not _TableConvert_h_ */

/*****************************************************************************
 * File Name:	 swisswatch.c
 * Description:	 Swiss X Watch
 * Author:	 Simon Leinen  <simon@switch.ch>
 * Date Created: 10-Mar-92
 * RCS $Header: /home/leinen/X/src/swisswatch-0.06/RCS/swisswatch.c,v 1.1 1999/12/15 09:20:11 leinen Exp leinen $	 
 * RCS $Log: swisswatch.c,v $
 * RCS Revision 1.1  1999/12/15 09:20:11  leinen
 * RCS Initial revision
 * RCS	 
 ****************************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include "SwissWatch.h"
#include <stdlib.h>
#include <stdio.h>

#if NeedFunctionPrototypes
static Widget initialize_user_interface(int*argcp,char**argv);
static void quit(Widget,XEvent *,String *,Cardinal *);
#else /* not NeedFunctionPrototypes */
static Widget initialize_user_interface();
static void quit();
#endif /* not NeedFunctionPrototypes */

extern int
main(argc, argv)
     int argc;
     char *argv[];
{
  (void) initialize_user_interface(& argc, argv);
  XtMainLoop();
  return 0;
}


static XrmOptionDescRec options[] =
{
  {"-fg",	  "*Foreground",       XrmoptionSepArg, NULL},
  {"-bg",	  "*Background",       XrmoptionSepArg, NULL},
  {"-foreground", "*Foreground",       XrmoptionSepArg, NULL},
  {"-background", "*Background",       XrmoptionSepArg, NULL},
  {"-tick",	  "*swissWatch.tickTime",  XrmoptionSepArg, NULL},
  {"-railroad",	  "*swissWatch.railroad", XrmoptionNoArg, (XtPointer)"True"},
  {"-sbb",	  "*swissWatch.railroad", XrmoptionNoArg, (XtPointer)"True"},
  {"-cff",	  "*swissWatch.railroad", XrmoptionNoArg, (XtPointer)"True"},
  {"-ffs",	  "*swissWatch.railroad", XrmoptionNoArg, (XtPointer)"True"},
  {"-noshape",	  "*swissWatch.shapeWindow", XrmoptionNoArg, (XtPointer)"False"},
  {"-circular",	  "*swissWatch.circular", XrmoptionNoArg, (XtPointer)"True"},
};

static XtActionsRec actions[] = {
    {"quit",    quit},
};

static Atom wm_delete_window;

static Widget
initialize_user_interface(argcp, argv)
     int * argcp;
     char ** argv;
{
  Widget top_level;
  XtAppContext app_c;

  top_level = XtInitialize(argv[0], "SwissWatch",
			   options, XtNumber(options),
			   argcp, argv);
  if (* argcp != 1)
    {
      fprintf (stderr, "usage: %s [toolkitoptions ...]\n", argv[0]);
      exit (42);
    }
  app_c = XtWidgetToApplicationContext(top_level);
  XtAppAddActions(app_c, actions, XtNumber(actions));
  XtVaCreateManagedWidget("swissWatch",
			  swissWatchWidgetClass, top_level, NULL);
  XtRealizeWidget(top_level);
  XtOverrideTranslations
    (top_level, XtParseTranslationTable ("<Message>WM_PROTOCOLS: quit()"));
  wm_delete_window = XInternAtom(XtDisplay(top_level), "WM_DELETE_WINDOW",
				 False);
  (void) XSetWMProtocols (XtDisplay(top_level), XtWindow(top_level),
			  &wm_delete_window, 1);
  return top_level;
}

static void quit(w, event, params, num_params)
     Widget w;
     XEvent *event;
     String *params;
     Cardinal *num_params;
{
  if (event->type == ClientMessage &&
      event->xclient.data.l[0] != wm_delete_window) {
    XBell(XtDisplay(w), 0);
  } else {
    XCloseDisplay(XtDisplay(w));
    exit(0);
  }
}

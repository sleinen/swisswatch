#ifndef _SwissWatchP_h_
#define _SwissWatchP_h_

#include <X11/CompositeP.h>
#include "SwissWatch.h"

typedef struct _SwissWatchClassPart {
  int		dummy;
} SwissWatchClassPart;

typedef struct _SwissWatchClassRec {
  CoreClassPart      core_class;
  CompositeClassPart composite_class;
  SwissWatchClassPart swiss_watch_class;
} SwissWatchClassRec;

extern SwissWatchClassRec swissWatchClassRec;

typedef struct _SwissWatchPart {
  unsigned	n_children;
  float		tick_time;
  double	now;
  long		now_local_sec;
  double	frac_sec;
  double	expected;
  XtIntervalId	timeout;
  Boolean	shape_window;
  Boolean	circular;
  Boolean	swiss_mode;
#ifdef SHAPE
  GC		shape_gc;
  Pixmap	shape_mask;
  int		shape_width;
  int		shape_height;
#endif /* SHAPE */
  GC		bg_gc;
  Pixmap	mark_pic;
  Pixmap	pic;
  int		rad_x;
  int		rad_y;
  int		cx;
  int		cy;
} SwissWatchPart;

typedef struct _SwissWatchRec {
    CorePart      core;
    CompositePart composite;
    SwissWatchPart swiss_watch;
} SwissWatchRec;

#ifdef NeedFunctionPrototypes
#define SwissWatchInheritResize (((void *)(Widget, int, int)) 0)
#else /* not NeedFunctionPrototypes */
#define SwissWatchInheritResize (((void *)()) 0)
#endif /* not NeedFunctionPrototypes */

#endif /* not _SwissWatchP_h_ */

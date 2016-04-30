/*****************************************************************************
 * File Name:	 SwissWatch.c
 * Description:	 Swisswatch widget class -- implementation
 * Author:	 Simon Leinen (simon@liasun5)
 * Date Created:  6-May-92
 * RCS $Header$	 
 * RCS $Log$	 
 ****************************************************************************/

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Xmu.h>
#include <X11/Xos.h>
#define SHAPE 1
#ifdef SHAPE
#include <X11/extensions/shape.h>
#endif /* SHAPE */
#include "SwissWatcP.h"
#include "HandOrMarP.h"
#include "Hand.h"
#include "Mark.h"
#include <math.h>

#if NeedFunctionPrototypes
static void Initialize(Widget,Widget,ArgList,Cardinal*);
static void Realize(Widget,XtValueMask*,XSetWindowAttributes*);
static void Redisplay(Widget,XEvent*,Region);
static void Resize(Widget);
static void Destroy(Widget);
static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal *);
static void ClassInitialize(void);
static void convert_string_to_widget_class(XrmValue*,Cardinal*,XrmValue*,XrmValue*);
static void adapt_size(SwissWatchWidget);
static void tock(SwissWatchWidget,int);
static void tick(XtPointer,XtIntervalId *);
static void drawhands(SwissWatchWidget);
static void drawmarks(SwissWatchWidget);
static void destroy_gcs(SwissWatchWidget);
static void get_gcs(SwissWatchWidget);
#else /* not NeedFunctionPrototypes */
static void Initialize();
static void Realize();
static void Redisplay();
static void Resize();
static void Destroy();
static Boolean SetValues();
static void ClassInitialize();
static void convert_string_to_widget_class();
static void adapt_size();
static void tick();
static void tock();
static void drawhands();
static void drawmarks();
static void destroy_gcs();
static void get_gcs();
#endif /* not NeedFunctionPrototypes */

#if !defined(__STDC__)
extern int sprintf();
extern int gettimeofday();
#endif /* __STDC__ */

#define offset(slot) XtOffsetOf(SwissWatchRec,swiss_watch.slot)

static XtResource resources[] =
{
  {XtNnChildren, XtCNChildren, XtRInt,sizeof(Cardinal),
     offset(n_children),
     XtRImmediate, (XtPointer)0},
  {XtNtickTime, XtCTickTime, XtRFloat,sizeof(float),
     offset(tick_time),
     XtRString, (XtPointer)"2.0"},
#ifdef SHAPE
  {XtNshapeWindow,XtCShapeWindow,XtRBoolean,sizeof(Boolean),
     offset(shape_window),XtRString,"true"},
#endif
  {XtNcircular,XtCCircular,XtRBoolean,sizeof(Boolean),
     offset(circular),XtRString,"false"},
  {XtNrailroad,XtCRailroad,XtRBoolean,sizeof(Boolean),
     offset(swiss_mode),XtRString,"false"},
};

#undef offset

static CompositeClassExtensionRec extension_rec = {
    /* next_extension */  NULL,
    /* record_type */     NULLQUARK,
    /* version */         XtCompositeExtensionVersion,
    /* record_size */     sizeof(CompositeClassExtensionRec),
    /* accepts_objects */ TRUE,
};

#define superclass (&compositeClassRec)

SwissWatchClassRec swissWatchClassRec = {
  { /******* CorePart *******/
    /* superclass	    */	(WidgetClass) superclass,
    /* class_name	    */	"SwissWatch",
    /* widget_size	    */	sizeof(SwissWatchRec),
    /* class_initialize     */  ClassInitialize,
    /* class_part_initialize*/	NULL,
    /* class_inited	    */	FALSE,
    /* initialize	    */	Initialize,
    /* initialize_hook      */	NULL,		
    /* realize		    */	Realize,
    /* actions		    */	NULL,
    /* num_actions	    */	0,
    /* resources	    */	resources,
    /* num_resources	    */	XtNumber(resources),
    /* xrm_class	    */	NULLQUARK,
    /* compress_motion      */	FALSE,
    /* compress_exposure    */	TRUE,
    /* compress_enterleave  */  FALSE,
    /* visible_interest     */	FALSE,
    /* destroy		    */	Destroy,
    /* resize		    */	Resize,
    /* expose		    */	Redisplay,
    /* set_values	    */	SetValues,
    /* set_values_hook      */	NULL,			
    /* set_values_almost    */	XtInheritSetValuesAlmost,  
    /* get_values_hook      */	NULL,			
    /* accept_focus	    */	NULL,
    /* version		    */	XtVersion,
    /* callback_offsets     */  NULL,
    /* tm_table		    */  NULL,
    /* query_geometry	    */  XtInheritQueryGeometry,
    /* display_accelerator  */	XtInheritDisplayAccelerator,
    /* extension	    */  NULL
  },
  { /**** CompositePart *****/
    /* geometry_handler     */  XtInheritGeometryManager,
    /* change_managed       */  XtInheritChangeManaged,
    /* insert_child	    */  XtInheritInsertChild,
    /* delete_child	    */  XtInheritDeleteChild,
    /* extension	    */  (XtPointer) & extension_rec,
    },
  {
    /* dummy		    */	0,
  }
};

#define WIDTH(widget) ((widget)->core.width)
#define HEIGHT(widget) ((widget)->core.height)
#define RAD_X(w)	((w)->swiss_watch.rad_x)
#define RAD_Y(w)	((w)->swiss_watch.rad_y)
#define CX(w)		((w)->swiss_watch.cx)
#define CY(w)		((w)->swiss_watch.cy)

WidgetClass swissWatchWidgetClass = (WidgetClass) &swissWatchClassRec;

typedef struct _ChildResources
{
  WidgetClass		class;
}
ChildResources;

static XtResource child_resources[] =
{
  {XtNclass, XtCClass, XtRWidgetClass,sizeof(WidgetClass),
     XtOffsetOf(ChildResources,class),
     XtRString, (XtPointer)"mark"}
};

static void
Initialize(req, new, args, num_args)
     Widget req;
     Widget new;
     ArgList args;
     Cardinal * num_args;
{
#define w_new ((SwissWatchWidget) new)
  unsigned k;
  char child_name[12];
  ChildResources resource_struct;

  for (k = 0; k < w_new->swiss_watch.n_children; ++k)
    {
      sprintf(child_name, "child_%d", k);
      XtGetSubresources(req, & resource_struct,
			child_name, "Child",
			child_resources, XtNumber(child_resources),
			NULL, 0);
      XtVaCreateManagedWidget(child_name, resource_struct.class,
			      new, NULL);
    }
#ifdef SHAPE
  {
    int shape_event_base, shape_error_base;
    if (w_new->swiss_watch.shape_window && !XShapeQueryExtension(XtDisplay(new), 
						       &shape_event_base, 
						       &shape_error_base))
      w_new->swiss_watch.shape_window = False;
    w_new->swiss_watch.shape_mask = 0;
    w_new->swiss_watch.shape_gc = 0;
    w_new->swiss_watch.shape_width = 0;
    w_new->swiss_watch.shape_height = 0;
  }
#endif				/* SHAPE */
#undef w_new
}

static void
ClassInitialize()
{
  XtAddConverter(XtRString, XtRWidgetClass, convert_string_to_widget_class, NULL, 0);
}

#define ForAllChildren(w, child) \
  for ( (child) = (Widget *) (w)->composite.children ; \
        (child) < (Widget *) ( (w)->composite.children + \
				 (w)->composite.num_children ) ; \
        (child)++ )


static void
Redisplay(w, event, region)
     Widget w;
     XEvent * event;
     Region region;
{
#define ww ((SwissWatchWidget) w)
#define ev ((XExposeEvent *) event)
  if (event==0)
    XCopyArea(XtDisplay(w),ww->swiss_watch.pic,XtWindow(w),ww->swiss_watch.bg_gc,
	      0,0,w->core.width,w->core.height,0,0);
  else
    XCopyArea(XtDisplay(w),ww->swiss_watch.pic,XtWindow(w),ww->swiss_watch.bg_gc,
	      ev->x,ev->y,ev->width,ev->height,ev->x,ev->y);
#undef ev
#undef ww
}

static void
convert_string_to_widget_class(args, num_args, from, to)
     XrmValue* args;
     Cardinal* num_args;
     XrmValue* from;
     XrmValue* to;
{
  char lower_name[1000];
  to->size = sizeof(WidgetClass);
  XmuCopyISOLatin1Lowered(lower_name, (char *) from->addr);
  if (!strcmp(lower_name, "hand"))
    { to->addr = (XtPointer) & handWidgetClass; return; }
  if (!strcmp(lower_name, "mark"))
    { to->addr = (XtPointer) & markWidgetClass; return; }
  to->size = 0;
  to->addr = NULL;
}

static void
Realize(w, mask, attributes)
     Widget w;
     XtValueMask* mask;
     XSetWindowAttributes* attributes;
{
#define ww ((SwissWatchWidget) w)

  Widget * childp;
  (*superclass->core_class.realize) (w, mask, attributes);
  get_gcs(ww);
  tock(ww,TRUE);
  adapt_size(ww);
  ForAllChildren(ww, childp)
    {
      RealizeHandOrMark((HandOrMarkWidget) (* childp));
    }
#undef ww
}

static void
tock(w, init_p)
     SwissWatchWidget w;
     int init_p;
{
  double now;
  double next;
  double tick_time = w->swiss_watch.tick_time;

  struct tm * tm_now;
  struct timeval tvnow;

  gettimeofday(& tvnow, (struct timezone *) 0);
  tm_now = localtime(&tvnow.tv_sec);
#if HAVE_GMTOFF
  w->swiss_watch.now_local_sec = tvnow.tv_sec+tm_now->tm_gmtoff;
#else /* not HAVE_GMTOFF */
  w->swiss_watch.now_local_sec
    = tm_now->tm_sec+tm_now->tm_min*60+tm_now->tm_hour*3600+tm_now->tm_mday*86400;
  w->swiss_watch.frac_sec = tvnow.tv_usec*1e-6;
#endif /* not HAVE_GMTOFF */

  now = tvnow.tv_sec+1e-6*tvnow.tv_usec;
  if (init_p)
    {
      int int_tick = (int) tick_time;

      if (int_tick==0)
	int_tick = 1;
      if (int_tick > 1)
	{
	  int next_sec = ((tm_now->tm_sec + int_tick) / int_tick);
	  next_sec *= int_tick;
	  next = tvnow.tv_sec + next_sec - tm_now->tm_sec;
	}
      else
	next = (tvnow.tv_sec/int_tick+1)*int_tick;
    }
  else
    {
      next = w->swiss_watch.expected+tick_time;
      while(next < now)
	next += tick_time;
    }
  w->swiss_watch.expected = next;
#ifdef DEBUG
  printf("%8f %8f %6d\n", tick_time, next-now, (int) ((next-now)*1000));
#endif /* 0 */
  XtAppAddTimeOut(XtWidgetToApplicationContext((Widget)w),
		  (int) ((next-now)*1000),
		  tick,
		  (XtPointer)w);
}

static void
tick(closure, interval)
     XtPointer closure;
     XtIntervalId * interval;
{
  tock((SwissWatchWidget) closure, FALSE);
  drawhands((SwissWatchWidget) closure);
  Redisplay((Widget) closure, (XEvent*) 0, (Region) 0);
}

static void
Resize(w)
     Widget w;
{
#define ww ((SwissWatchWidget) w)
  Display * display = XtDisplay(w);

  if (XtIsRealized (w))
    {
      XFreePixmap(display,ww->swiss_watch.mark_pic);
      XFreePixmap(display,ww->swiss_watch.pic);
      adapt_size(ww);
    }
#undef ww
}

static void
adapt_size(w)
     SwissWatchWidget w;
{
  Display * display = XtDisplay(w);
  int depth = DefaultDepthOfScreen(XtScreen(w));
  Widget * childp;
  double scale;

  w->swiss_watch.mark_pic = XCreatePixmap(display,XtWindow(w),WIDTH(w),HEIGHT(w),depth);
  w->swiss_watch.pic = XCreatePixmap(display,XtWindow(w),WIDTH(w),HEIGHT(w),depth);
  w->swiss_watch.cx = WIDTH(w)/2;
  w->swiss_watch.cy = HEIGHT(w)/2;
  if (w->swiss_watch.circular)
    w->swiss_watch.rad_x = w->swiss_watch.rad_y = scale
      = (CX(w) < CY(w)) ? CX(w) : CY(w);
  else
    w->swiss_watch.rad_x = CX(w), w->swiss_watch.rad_y = CY(w),
    scale = sqrt((double) (CX(w)*CY(w)));
#ifdef SHAPE
  if (w->swiss_watch.shape_window)
    {
      Pixmap shape_mask;
      XGCValues xgcv;
      int x;
      int y;
      Widget parent;

      shape_mask = XCreatePixmap (XtDisplay (w), XtWindow (w),
				  WIDTH(w), HEIGHT(w), 1);
      if (!w->swiss_watch.shape_gc)
	w->swiss_watch.shape_gc = XCreateGC (XtDisplay((Widget) w), shape_mask, 0, &xgcv);

      /* erase the pixmap */
      XSetForeground (XtDisplay (w), w->swiss_watch.shape_gc, 0);
      XFillRectangle (XtDisplay (w), shape_mask, w->swiss_watch.shape_gc,
		      0, 0, WIDTH(w), HEIGHT(w));
      XSetForeground (XtDisplay (w), w->swiss_watch.shape_gc, 1);
      XFillArc (XtDisplay (w), shape_mask,
		w->swiss_watch.shape_gc,
		CX(w)-RAD_X(w),CY(w)-RAD_Y(w),
		2*RAD_X(w),2*RAD_Y(w),
		0,360*64);
      /* Find the highest enclosing widget and shape it */
      x = 0;
      y = 0;
      for (parent = (Widget) w; XtParent (parent); parent = XtParent (parent))
	{
	  x = x + parent->core.x + parent->core.border_width;
	  y = y + parent->core.y + parent->core.border_width;
	}

      XShapeCombineMask (XtDisplay (parent), XtWindow (parent), ShapeBounding,
			 x, y, shape_mask, ShapeSet);

      /* erase the pixmap */
      XSetForeground (XtDisplay (w), w->swiss_watch.shape_gc, 0);
      XFillRectangle (XtDisplay (w), shape_mask, w->swiss_watch.shape_gc,
		      0, 0, WIDTH(w), HEIGHT(w));
      XSetForeground (XtDisplay (w), w->swiss_watch.shape_gc, 1);
      /* draw the clip shape */
      XFillArc (XtDisplay (w),shape_mask,w->swiss_watch.shape_gc,
		CX(w)-RAD_X(w),CY(w)-RAD_Y(w),
		2*RAD_X(w),2*RAD_Y(w),
		0,360*64);
      XShapeCombineMask (XtDisplay (w), XtWindow (w), ShapeClip, 
			 0, 0, shape_mask, ShapeSet);
      XFreePixmap (XtDisplay (w), shape_mask);
    }
#endif /* SHAPE */
  ForAllChildren(w, childp)
    {
      ResizeHandOrMark((HandOrMarkWidget) (* childp), RAD_X(w), RAD_Y(w));
    }
  if (XtIsRealized((Widget) w))
    {
      drawmarks(w);
      drawhands(w);
    }
}

static void
Destroy(w)
     Widget w;
{
#define ww ((SwissWatchWidget) w)
#ifdef SHAPE
  destroy_gcs(ww);
  XFreePixmap(XtDisplay(w), ww->swiss_watch.shape_mask);
#endif /* SHAPE */
  XtReleaseGC(w, ww->swiss_watch.bg_gc);
  XFreePixmap(XtDisplay(w), ww->swiss_watch.mark_pic);
  XFreePixmap(XtDisplay(w), ww->swiss_watch.pic);
  if (ww->swiss_watch.timeout)
    XtRemoveTimeOut(ww->swiss_watch.timeout);
#undef ww
}

static void
drawhands(ww)
     SwissWatchWidget ww;
{
  Widget * childp;
#define w ((Widget) ww)
  XCopyArea(XtDisplay(w), ww->swiss_watch.mark_pic,ww->swiss_watch.pic,ww->swiss_watch.bg_gc,
	    0,0,WIDTH(w),HEIGHT(w),0,0);
  ForAllChildren(ww, childp)
    {
      if (XtIsSubclass(*childp, handWidgetClass))
	DrawHandOrMark((HandOrMarkWidget)*childp, ww->swiss_watch.pic);
    }
#undef w
}

static void
drawmarks(ww)
     SwissWatchWidget ww;
{
  Widget * childp;
#define w ((Widget) ww)
  XFillRectangle(XtDisplay(w),ww->swiss_watch.mark_pic,ww->swiss_watch.bg_gc,
		 0,0,ww->core.width,ww->core.height);
  ForAllChildren(ww, childp)
    {
      if (XtIsSubclass(*childp, markWidgetClass))
	DrawHandOrMark((HandOrMarkWidget)*childp, ww->swiss_watch.mark_pic);
    }
#undef w
}

static Boolean
SetValues(current, request, new, args, num_args)
     Widget current;
     Widget request;
     Widget new;
     ArgList args;
     Cardinal * num_args;
{
#define w_cur ((SwissWatchWidget) current)
#define w_new ((SwissWatchWidget) new)
#define CHANGED(FIELD) (w_cur->FIELD != w_new->FIELD)
  Boolean needs_redisplay = FALSE;
  Boolean needs_reset = FALSE;
  Boolean needs_new_gcs = FALSE;

  if (CHANGED (core.background_pixel))
    needs_redisplay = needs_reset = needs_new_gcs = TRUE;

  if (needs_new_gcs && XtIsRealized (current))
    {
      destroy_gcs(w_cur);
      get_gcs(w_new);
    }
  if (needs_reset)
    SwissWatchReset(w_new);
  return needs_redisplay;
#undef CHANGED
#undef w_cur
#undef w_new
}

static void
destroy_gcs(w)
     SwissWatchWidget w;
{
  XtReleaseGC((Widget) w, w->swiss_watch.shape_gc);
}

static void
get_gcs(w)
     SwissWatchWidget w;
{
  XGCValues values;
  values.foreground = w->core.background_pixel;
  w->swiss_watch.bg_gc = XtGetGC ((Widget) w, GCForeground, & values);
}

extern void
SwissWatchReset(w)
     SwissWatchWidget w;
{
  drawmarks(w);
  drawhands(w);
}

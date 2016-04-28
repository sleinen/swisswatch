/*****************************************************************************
 * File Name:	 HandOrMark.c
 * Description:	 HandOrMark widget class -- implementation
 * Author:	 Simon Leinen (simon@liasun5)
 * Date Created:  6-May-92
 * RCS $Header$	 
 * RCS $Log$	 
 ****************************************************************************/

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include "HandOrMarP.h"
#include "SwissWatcP.h"
#include "TableConvert.h"
#include <math.h>

#define offset(slot) XtOffsetOf(HandOrMarkRec,hand_or_mark.slot)

static XtResource resources[] =
{
  {XtNinner, XtCInner,
     XtRFloat,sizeof(float), offset(inner),
     XtRString, (XtPointer)"0.0"},
  {XtNouter, XtCOuter,
     XtRFloat,sizeof(float), offset(outer),
     XtRString, (XtPointer)"0.0"},
  {XtNforeground, XtCForeground,
     XtRPixel,sizeof(Pixel), offset(foreground),
     XtRString, XtDefaultForeground},
  {XtNcycle, XtCCycle, XtRInt,sizeof(int), offset(cycle),
     XtRString, "4711"},
  {XtNstrokeWidth,XtCStrokeWidth,
     XtRDimension,sizeof(Dimension),offset(stroke_width),
     XtRString,"0"},
  {XtNstrokeWidthR,XtCStrokeWidthR,
     XtRFloat,sizeof(float),offset(stroke_width_r),
     XtRString,"0"},
  {XtNcap,XtCCap,
     XtRCapStyle,sizeof(int),offset(cap_style),XtRImmediate,
     (XtPointer)CapButt},
  {XtNjoin,XtCJoin,
     XtRJoinStyle,sizeof(int),offset(join_style),XtRImmediate,
     (XtPointer)JoinMiter},
  {XtNshape,XtCShape,
     XtRTickShape,sizeof(int),offset(shape_style),XtRImmediate,
     (XtPointer)ShapeRectangle},
  {XtNrender,XtCRender,
     XtRTickRenderType,sizeof(int),offset(render_type),XtRImmediate,
     (XtPointer)ShapeRectangle},
  {XtNwidth, XtCWidth, XtRFloat, sizeof(float), offset(width),
     XtRString, "0.0"},
  {XtNphase, XtCPhase, XtRFloat, sizeof(float), offset(phase),
     XtRString, "0"},
  {XtNcenterX, XtCCenterX, XtRFloat, sizeof(float), offset(cx),
     XtRString, "0"},
  {XtNcenterY, XtCCenterY, XtRFloat, sizeof(float), offset(cy),
     XtRString, "0"},
};

#define RAD_X(w)	((w)->swiss_watch.rad_x)
#define RAD_Y(w)	((w)->swiss_watch.rad_y)
#define CX(w)		((w)->swiss_watch.cx)
#define CY(w)		((w)->swiss_watch.cy)

#undef offset

#if NeedFunctionPrototypes
static void InitializeClass(void);
static void Destroy(Widget);
static void Realize(Widget);
static void Resize(Widget, int, int);
static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal *);
static void get_hand_or_mark_gcs(HandOrMarkWidget);
static void destroy_gcs(HandOrMarkWidget);
#else /* not NeedFunctionPrototypes */
static void InitializeClass();
static void Destroy();
static void Realize();
static void Resize();
static Boolean SetValues();
static void get_hand_or_mark_gcs();
static void destroy_gcs();
#endif /* not NeedFunctionPrototypes */

HandOrMarkClassRec handOrMarkClassRec =
{
  {
    /* superclass	  */	(WidgetClass) &objectClassRec,
    /* class_name	  */	"HandOrMark",
    /* widget_size	  */	sizeof(HandOrMarkRec),
    /* class_initialize   */    InitializeClass,
    /* class_part_initialize*/	NULL,
    /* class_inited       */	FALSE,
    /* initialize	  */	NULL,
    /* initialize_hook    */	NULL,		
    /* rect1		  */    NULL,
    /* rect2		  */	NULL,
    /* rect3		  */	0,
    /* resources	  */	resources,
    /* num_resources	  */    XtNumber(resources),
    /* xrm_class	  */	NULLQUARK,
    /* rect4              */    FALSE,
    /* rect5              */    FALSE,
    /* rect6              */    FALSE,
    /* rect7              */    FALSE,
    /* destroy		  */	Destroy,
    /* resize		  */	NULL,
    /* expose		  */	NULL,
    /* set_values	  */	SetValues,
    /* set_values_hook    */	NULL,			
    /* pad                */    NULL,
    /* get_values_hook    */	NULL,			
    /* pad                */    NULL,
    /* version		  */	XtVersion,
    /* callback_offsets   */    NULL,
    /* pad                */    NULL,
    /* pad                */    NULL,
    /* pad                */    NULL,
    /* extension	    */  NULL,
  },
  {
    /* draw		  */	NULL,
    /* resize 		  */	Resize,
    /* realize		  */	Realize,
  }
};

WidgetClass handOrMarkWidgetClass = (WidgetClass) &handOrMarkClassRec;

static void
Realize(w)
     Widget w;
{
}

static void
get_hand_or_mark_gcs(w)
     HandOrMarkWidget w;
{
  XGCValues values;
  XtGCMask mask;
  SwissWatchWidget parent = (SwissWatchWidget) XtParent((Widget) w);
  int radius = sqrt((double) RAD_X(parent)*RAD_Y(parent));

  if (w->hand_or_mark.shape_style==ShapeCircle
      && w->hand_or_mark.render_type==ShapeFill
      && RAD_X(parent)==RAD_Y(parent))
    {
      w->hand_or_mark.use_draw_line_to_draw_circle = True;
      if (w->hand_or_mark.stroke_width_r)
	w->hand_or_mark.stroke_width = (int) (w->hand_or_mark.stroke_width_r*radius);
      else if (w->hand_or_mark.width)
	w->hand_or_mark.stroke_width = (int) (w->hand_or_mark.width*radius);
      w->hand_or_mark.cap_style = CapRound;
    }
  else
    {
      w->hand_or_mark.use_draw_line_to_draw_circle = False;
      if (w->hand_or_mark.stroke_width_r)
	w->hand_or_mark.stroke_width = (int) (w->hand_or_mark.stroke_width_r*radius);
    }
  values.foreground = w->hand_or_mark.foreground;
  values.line_width = w->hand_or_mark.stroke_width;
  values.cap_style = w->hand_or_mark.cap_style;
  values.join_style = w->hand_or_mark.join_style;
  mask = GCForeground | GCLineWidth | GCCapStyle | GCJoinStyle;
  w->hand_or_mark.fg_gc = XtGetGC((Widget) parent, mask, &values);
}

static void
Destroy(w)
     Widget w;
{
#define ww ((HandOrMarkWidget) w)
  destroy_gcs(ww);
#undef ww
}

static LOOKUP_TABLE_ENTRY cap_table[] =
{
  { "notlast", CapNotLast },
  { "butt", CapButt },
  { "round", CapRound },
  { "projecting", CapProjecting },
  { "capnotlast", CapNotLast },
  { "capbutt", CapButt },
  { "capround", CapRound },
  { "capprojecting", CapProjecting }
};

static LOOKUP_TABLE_ENTRY join_table[] =
{
  { "miter", JoinMiter },
  { "round", JoinRound },
  { "bevel", JoinBevel },
  { "joinmiter", JoinMiter },
  { "joinround", JoinRound },
  { "joinbevel", JoinBevel }
};

static LOOKUP_TABLE_ENTRY tick_shape_table[] =
{
  { "triangle", ShapeTriangle },
  { "triangular", ShapeTriangle },
  { "rectangle", ShapeRectangle },
  { "rectangular", ShapeRectangle },
  { "arrow", ShapeArrow },
  { "arrowed", ShapeArrow },
  { "circle", ShapeCircle },
  { "circular", ShapeCircle }
};

static LOOKUP_TABLE_ENTRY tick_render_table[] =
{
  { "outline", ShapeOutline },
  { "outlined", ShapeOutline },
  { "fill", ShapeFill },
  { "filled", ShapeFill }
};

static void
InitializeClass()
{
  set_table_converter
    (XtRCapStyle,cap_table,XtNumber(cap_table));
  set_table_converter
    (XtRJoinStyle,join_table,XtNumber(join_table));
  set_table_converter
    (XtRTickShape,tick_shape_table,XtNumber(tick_shape_table));
  set_table_converter
    (XtRTickRenderType,tick_render_table,XtNumber(tick_render_table));
}

extern void
ResizeHandOrMark(hand_or_mark, width, height)
     HandOrMarkWidget hand_or_mark;
     int width;
     int height;
{
  HandOrMarkWidgetClass class
    = (HandOrMarkWidgetClass) hand_or_mark->object.widget_class;
  while (class->hand_or_mark_class.resize==0)
    class = (HandOrMarkWidgetClass) class->object_class.superclass;
  (* class->hand_or_mark_class.resize) ((Widget) hand_or_mark, width, height);
}

extern void
RealizeHandOrMark(hand_or_mark)
     HandOrMarkWidget hand_or_mark;
{
  HandOrMarkWidgetClass class
    = (HandOrMarkWidgetClass) hand_or_mark->object.widget_class;
  while (class->hand_or_mark_class.realize==0)
    class = (HandOrMarkWidgetClass) class->object_class.superclass;
  (* class->hand_or_mark_class.realize) ((Widget) hand_or_mark);
}

static void
Resize(w, width, height)
     Widget w;
     int width;
     int height;
{
#define hw ((HandOrMarkWidget) w)
  if (XtIsRealized(XtParent(w)))
    {
      destroy_gcs(hw);
      get_hand_or_mark_gcs(hw);
    }
#undef hw
}

extern void
DrawShape(w,d,gc,s,c)
     HandOrMarkWidget w;
     Drawable d;
     GC gc;
     double s;
     double c;
{
  SwissWatchWidget parent = (SwissWatchWidget) XtParent((Widget) w);
#define TX(x) (CX(parent)+(int)((w->hand_or_mark.cx+(x))*RAD_X(parent)))
#define TY(y) (CY(parent)-(int)((w->hand_or_mark.cy+(y))*RAD_Y(parent)))

  switch (w->hand_or_mark.shape_style)
    { case ShapeTriangle:
	if (w->hand_or_mark.width < 1e-3)
	  { XDrawLine(XtDisplay((Widget) parent),d,gc,
		      TX(c*w->hand_or_mark.inner),TY(s*w->hand_or_mark.inner),
		      TX(c*w->hand_or_mark.outer),TY(s*w->hand_or_mark.outer));
	  }
	else
	  { XPoint p[4];
	    p[0].x = TX(c*w->hand_or_mark.outer);
	    p[0].y = TY(s*w->hand_or_mark.outer);
	    p[1].x = TX(c*w->hand_or_mark.inner-s*w->hand_or_mark.width/2);
	    p[1].y = TY(s*w->hand_or_mark.inner+c*w->hand_or_mark.width/2);
	    p[2].x = TX(c*w->hand_or_mark.inner+s*w->hand_or_mark.width/2);
	    p[2].y = TY(s*w->hand_or_mark.inner-c*w->hand_or_mark.width/2);
	    switch (w->hand_or_mark.render_type)
	      { case ShapeOutline:
		  p[3] = p[0];
		  XDrawLines(XtDisplay((Widget) parent),d,gc,&p[0],4,CoordModeOrigin);
		  break;
		case ShapeFill:
		  XFillPolygon(XtDisplay((Widget) parent),d,gc,&p[0],3,Convex,CoordModeOrigin);
		  break;
		}
	  }
	break;
      case ShapeRectangle:
      case ShapeArrow:
	if (w->hand_or_mark.width < 1e-3)
	  { XDrawLine(XtDisplay((Widget) parent),d,gc,
		      TX(c*w->hand_or_mark.inner),TY(s*w->hand_or_mark.inner),
		      TX(c*w->hand_or_mark.outer),TY(s*w->hand_or_mark.outer));
	  }
	else
	  { XPoint p[6];
	    Cardinal k=0;
#define ADDPOINT(X,Y) (p[k].x = TX(X),p[k].y = TY(Y),++k)
	    ADDPOINT((c*w->hand_or_mark.inner)-(s*w->hand_or_mark.width/2),
		     (s*w->hand_or_mark.inner)+(c*w->hand_or_mark.width/2));
	    ADDPOINT((c*w->hand_or_mark.inner)+(s*w->hand_or_mark.width/2),
		     (s*w->hand_or_mark.inner)-(c*w->hand_or_mark.width/2));
	    ADDPOINT((c*w->hand_or_mark.outer)+(s*w->hand_or_mark.width/2),
		     (s*w->hand_or_mark.outer)-(c*w->hand_or_mark.width/2));
	    if (w->hand_or_mark.shape_style==ShapeArrow)
	      {
		ADDPOINT(c*(w->hand_or_mark.outer+w->hand_or_mark.width/2),
			 s*(w->hand_or_mark.outer+w->hand_or_mark.width/2));
	      }
	    ADDPOINT((c*w->hand_or_mark.outer)-(s*w->hand_or_mark.width/2),
		     (s*w->hand_or_mark.outer)+(c*w->hand_or_mark.width/2));
	    switch (w->hand_or_mark.render_type)
	      { case ShapeOutline:
		  p[k] = p[0],++k;
		  XDrawLines(XtDisplay((Widget) parent),d,gc,&p[0],k,CoordModeOrigin);
		  break;
		case ShapeFill:
		  XFillPolygon(XtDisplay((Widget) parent),d,gc,&p[0],k,Convex,CoordModeOrigin);
		  break;
		}
#undef ADDPOINT
	  }
	break;
      case ShapeCircle:
	    switch (w->hand_or_mark.render_type)
	      { case ShapeOutline:
		  XDrawArc(XtDisplay((Widget) parent),d,gc,
			   TX(c*w->hand_or_mark.outer-w->hand_or_mark.width/2),
			   TY(s*w->hand_or_mark.outer+w->hand_or_mark.width/2),
			   (int) (RAD_X(parent)*w->hand_or_mark.width),
			   (int) (RAD_Y(parent)*w->hand_or_mark.width),
			   0,360*64);
		  break;
		case ShapeFill:
		  if (w->hand_or_mark.use_draw_line_to_draw_circle)
		    {
		      int cx = TX(c*w->hand_or_mark.outer);
		      int cy = TY(s*w->hand_or_mark.outer);
		      XDrawLine(XtDisplay((Widget) parent),d,gc,cx,cy,cx,cy);
		    }
		  else
		    XFillArc(XtDisplay((Widget) parent),d,gc,
			     TX(c*w->hand_or_mark.outer-w->hand_or_mark.width/2),
			     TY(s*w->hand_or_mark.outer+w->hand_or_mark.width/2),
			     (int) (RAD_X(parent)*w->hand_or_mark.width),
			     (int) (RAD_Y(parent)*w->hand_or_mark.width),
			     0,360*64);
		  break;
		}
	break;
      }
#undef TX
#undef TY
}

extern void
DrawHandOrMark(w, d)
     HandOrMarkWidget w;
     Drawable d;
{
  Widget parent = XtParent((Widget) w);

  HandOrMarkWidgetClass class
    = (HandOrMarkWidgetClass) w->object.widget_class;
  while (class->hand_or_mark_class.draw==0)
    class = (HandOrMarkWidgetClass) class->object_class.superclass;
  (* class->hand_or_mark_class.draw) ((Widget) w, d,
				      parent->core.width, parent->core.height);
}

static Boolean
SetValues(current, request, new, args, num_args)
     Widget current;
     Widget request;
     Widget new;
     ArgList args;
     Cardinal * num_args;
{
#define h_cur ((HandOrMarkWidget) current)
#define h_new ((HandOrMarkWidget) new)
  Boolean needs_parent_reset = FALSE;
  Boolean needs_redisplay = FALSE;
  Boolean needs_new_gcs = FALSE;

#define CHANGED(FIELD) (h_cur->FIELD != h_new->FIELD)
  if (CHANGED (hand_or_mark.stroke_width)
      && !CHANGED (hand_or_mark.stroke_width_r))
    h_new->hand_or_mark.stroke_width_r = 0.0;

  if (CHANGED (hand_or_mark.inner)
      || CHANGED (hand_or_mark.cx)
      || CHANGED (hand_or_mark.cy)
      || CHANGED (hand_or_mark.outer)
      || CHANGED (hand_or_mark.cycle)
      || CHANGED (hand_or_mark.width)
      || CHANGED (hand_or_mark.phase)
      || CHANGED (hand_or_mark.shape_style)
      || CHANGED (hand_or_mark.render_type))
    needs_redisplay = needs_parent_reset = TRUE;

  if (CHANGED (hand_or_mark.foreground)
      || CHANGED (hand_or_mark.stroke_width)
      || CHANGED (hand_or_mark.stroke_width_r)
      || CHANGED (hand_or_mark.cap_style)
      || CHANGED (hand_or_mark.join_style))
    needs_redisplay = needs_parent_reset = needs_new_gcs = TRUE;

  if (needs_new_gcs || XtIsRealized(XtParent(current)))
    {
      destroy_gcs(h_cur);
      get_hand_or_mark_gcs(h_new);
    }
  if (needs_parent_reset)
    {
      SwissWatchReset((SwissWatchWidget) XtParent(current));
    }
  return needs_redisplay;
#undef CHANGED
#undef h_cur
#undef h_new
}


static void
destroy_gcs(w)
     HandOrMarkWidget w;
{
  XtReleaseGC((Widget) w, w->hand_or_mark.fg_gc);
}

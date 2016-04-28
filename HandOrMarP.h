#ifndef _HandOrMarkP_h_
#define _HandOrMarkP_h_

#include <X11/ObjectP.h>
#include "HandOrMark.h"

#define XtRCapStyle		"CapStyle"
#define XtRJoinStyle		"JoinStyle"
#define XtRTickShape		"TickShape"
#define XtRTickRenderType	"TickRenderType"

#if NeedFunctionPrototypes
typedef void (* XslDrawHandOrMarkProc) (Widget, Drawable, int, int);
typedef void (* XslResizeHandOrMarkProc) (Widget, int, int);
typedef void (* XslRealizeHandOrMarkProc) (Widget);
#else /* not NeedFunctionPrototypes */
typedef void (* XslDrawHandOrMarkProc) ();
typedef void (* XslResizeHandOrMarkProc) ();
typedef void (* XslRealizeHandOrMarkProc) ();
#endif /* not NeedFunctionPrototypes */

typedef struct _HandOrMarkClassPart
{
  XslDrawHandOrMarkProc		draw;
  XslResizeHandOrMarkProc	resize;
  XslRealizeHandOrMarkProc	realize;
} HandOrMarkClassPart;

typedef struct _HandOrMarkClassRec
{
  ObjectClassPart	object_class;
  HandOrMarkClassPart	hand_or_mark_class;
} HandOrMarkClassRec;

extern HandOrMarkClassRec handOrMarkClassRec;

typedef enum
{
  ShapeTriangle,
  ShapeRectangle,
  ShapeArrow,
  ShapeCircle,
}
HandOrMarkShape;

typedef enum
{
  ShapeOutline,
  ShapeFill,
}
HandOrMarkOutline;

typedef struct _HandOrMarkPart
{
  float		inner;
  float		outer;
  long		pad_0;
  Dimension	stroke_width;
  float		stroke_width_r;
  int		cycle;
  Pixel		foreground;
  GC		fg_gc;
  float		width;
  float		phase;
  int		shape_style;
  int		cap_style;
  int		join_style;
  int		render_type;
  float		cx;
  float		cy;
  Boolean	use_draw_line_to_draw_circle;
} HandOrMarkPart;

typedef struct _HandOrMarkRec {
    ObjectPart  object;
    HandOrMarkPart hand_or_mark;
} HandOrMarkRec;

#if NeedFunctionPrototypes
extern void DrawShape(HandOrMarkWidget, Drawable, GC, double, double);
extern void DrawHandOrMark(HandOrMarkWidget, Drawable);
extern void ResizeHandOrMark(HandOrMarkWidget, int, int);
extern void RealizeHandOrMark(HandOrMarkWidget);
#else /* not NeedFunctionPrototypes */
extern void DrawShape();
extern void DrawHandOrMark();
extern void ResizeHandOrMark();
extern void RealizeHandOrMark();
#endif /* not NeedFunctionPrototypes */

#endif /* not _HandOrMarkP_h_ */

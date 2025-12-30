/*****************************************************************************
 * File Name:	 Mark.c
 * Description:	 Mark widget class -- implementation
 * Author:	 Simon Leinen  <simon@leinen.ch>
 * Date Created:  6-May-92
 ****************************************************************************/

#define _XOPEN_SOURCE

#include <X11/IntrinsicP.h>
#include "MarkP.h"
#include <math.h>

#if NeedFunctionPrototypes
static void draw(Widget, Drawable, int, int);
#else /* not NeedFunctionPrototypes */
static void draw();
#endif /* not NeedFunctionPrototypes */

MarkClassRec markClassRec =
{
  {
    /* superclass	  */	(WidgetClass) & handOrMarkClassRec,
    /* class_name	  */	"Mark",
    /* widget_size	  */	sizeof(MarkRec),
    /* class_initialize   */    NULL,
    /* class_part_initialize*/	NULL,
    /* class_inited       */	FALSE,
    /* initialize	  */	NULL,
    /* initialize_hook    */	NULL,		
    /* pad                */    NULL,
    /* pad		  */	NULL,
    /* pad       	  */	0,
    /* resources	  */	NULL,
    /* num_resources	  */    0,
    /* xrm_class	  */	NULLQUARK,
    /* pad                */    FALSE,
    /* pad                */    FALSE,
    /* pad                */    FALSE,
    /* pad                */    FALSE,
    /* destroy		  */	NULL,
    /* pad		  */	NULL,
    /* expose		  */	NULL,
    /* set_values	  */	NULL,
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
    /* draw		  */	draw,
    /* resize 		  */	0,
    /* realize		  */	0,
  },
  {
    /* dummy		  */	0,
  },
};

WidgetClass markWidgetClass = (WidgetClass) &markClassRec;

static void
draw(w, d, width, height)
     Widget w;
     Drawable d;
     int width;
     int height;
{
#define mw ((MarkWidget) w)
  int cycle = mw->hand_or_mark.cycle;
  unsigned k;
  double sinus, cosinus;

  for (k = 0; k < cycle; ++k)
    {
      double ang = M_PI_2-((double)k/(double)cycle*2*M_PI);
#if HAVE_SINCOS
      sincos(ang, &sinus, &cosinus);
#else				/* not HAVE_SINCOS */
      sinus = sin(ang);
      cosinus = cos(ang);
#endif				/* not HAVE_SINCOS */
      DrawShape((HandOrMarkWidget)mw,
		d,
		mw->hand_or_mark.fg_gc,
		sinus, cosinus);
    }
#undef mw
}

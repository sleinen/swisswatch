/*****************************************************************************
 * File Name:	 Hand.c
 * Description:	 Hand widget class -- implementation
 * Author:	 Simon Leinen  <simon@switch.ch>
 * Date Created:  6-May-92
 * RCS $Header: /home/leinen/X/src/swisswatch-0.06/RCS/Hand.c,v 1.1 1999/12/15 09:17:23 leinen Exp leinen $	 
 * RCS $Log: Hand.c,v $
 * RCS Revision 1.1  1999/12/15 09:17:23  leinen
 * RCS Initial revision
 * RCS	 
 ****************************************************************************/

#define _XOPEN_SOURCE

#include <X11/IntrinsicP.h>
#include "HandP.h"
#include "SwissWatcP.h"
#include <math.h>

#if NeedFunctionPrototypes
static void draw(Widget, Drawable, int, int);
#else /* not NeedFunctionPrototypes */
static void draw();
#endif /* not NeedFunctionPrototypes */

#if (defined(sun) || defined(__sun__)) && !defined (SYSV)
#define HAVE_SINCOS 1
#endif

HandClassRec handClassRec =
{
  {
    /* superclass	  */	(WidgetClass) & handOrMarkClassRec,
    /* class_name	  */	"Hand",
    /* widget_size	  */	sizeof(HandRec),
    /* class_initialize   */    NULL,
    /* class_part_initialize*/	NULL,
    /* class_inited       */	FALSE,
    /* initialize	  */	NULL,
    /* initialize_hook    */	NULL,		
    /* rect1		  */    NULL,
    /* rect2		  */	NULL,
    /* rect3		  */	0,
    /* resources	  */	NULL,
    /* num_resources	  */    0,
    /* xrm_class	  */	NULLQUARK,
    /* rect4              */    FALSE,
    /* rect5              */    FALSE,
    /* rect6              */    FALSE,
    /* rect7              */    FALSE,
    /* destroy		  */	NULL,
    /* resize		  */	NULL,
    /* expose		  */	NULL,
    /* set_values	  */	NULL,
    /* set_values_hook    */	NULL,			
    /* set_values_almost  */    NULL,
    /* get_values_hook    */	NULL,			
    /* rect9              */    NULL,
    /* version		  */	XtVersion,
    /* callback_offsets   */    NULL,
    /* rect10             */    NULL,
    /* query_geometry	  */    NULL,
    /* rect11		  */    NULL,
    /* extension
	    */  NULL,
  },
  {
    /* draw		  */	draw,
    /* resize 		  */	0,
    /* realize		  */	0,
  },
  {
    /* dummy		  */	0,
  }
};

WidgetClass handWidgetClass = (WidgetClass) &handClassRec;

static void
draw(w, d, width, height)
     Widget w;
     Drawable d;
     int width;
     int height;
{
#define hw ((HandWidget) w)
  SwissWatchWidget parent = (SwissWatchWidget) XtParent(w);
  double dnow = parent->swiss_watch.now_local_sec+parent->swiss_watch.frac_sec;
  double cycle = hw->hand_or_mark.cycle;
  double ang;
  double sinus, cosinus;

  if (parent->swiss_watch.swiss_mode)
    {
      if (cycle < 464)
	{ ang = fmod(dnow-hw->hand_or_mark.phase,60.0);
	  if (ang < 57.5)
	    { ang = (ang * 2 * M_PI * 62.6) / 3600;
	    }
#if 0
	  else if (ang > 58.9)
	    { if (nexttick.tv_usec > 500000) nexttick.tv_sec ++;
	      nexttick.tv_usec = 0;
	      ang = 0;
	    }
#endif				/* 0 */
	  else
	    { ang = 0;
	    }
	}
      else
	{ ang = dnow - hw->hand_or_mark.phase;
	  ang -= fmod(ang,60.0) - .0001; /* fudge factor - ensure desired rounding */
	  ang = (fmod(ang,cycle) * 2 * M_PI) / cycle;
	}
    }
  else
    {
      ang = (fmod(dnow-hw->hand_or_mark.phase,cycle) * 2 * M_PI) / cycle;
    }
#if HAVE_SINCOS
  sincos((double) (M_PI_2-ang), &sinus, &cosinus);
#else /* not HAVE_SINCOS */
  sinus = sin((double) (M_PI_2-ang));
  cosinus = cos((double) (M_PI_2-ang));
#endif /* not HAVE_SINCOS */
  DrawShape((HandOrMarkWidget)hw,
	    d,
	    hw->hand_or_mark.fg_gc,
	    sinus, cosinus);
#ifdef DEBUG
  printf("draw %s %s %f %f\n",
	 w->core.widget_class->core_class.class_name,
	 XrmNameToString(w->core.xrm_name),
	 ang,
	 ang/M_PI*180.0);
#endif /* DEBUG */
#undef hw
}

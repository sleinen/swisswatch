/*****************************************************************************
 * File Name:	 SwissWatch.h
 * Description:	 SwissWatch widget -- public header file
 * Author:	 Simon Leinen  <simon@leinen.ch>
 * Date Created: 12-Mar-92
 ****************************************************************************/

#ifndef _SwissWatch_h_
#define _SwissWatch_h_

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 destroyCallback     Callback		Pointer		NULL
 height		     Height		Dimension	computed at create
 icon		     Icon		Pixmap		0
 label		     Label		String		NULL
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 sensitive	     Sensitive		Boolean		True
 value		     Value		String		NULL
 width		     Width		Dimension	computed at create
 x		     Position		Position	0
 y		     Position		Position	0
 nChildren	     NChildren		Cardinal	0
 tickTime	     TickTime		Float		1.0
 railroad		     Railroad		Boolean		False
 circular	     Circular		Boolean		False
 shapeWindow	     ShapeWindow	Boolean		True (if the SHAPE
							      extension is
							      available)
 */

#define XtNnChildren	"nChildren"
#define XtNtickTime	"tickTime"
#define XtNrailroad	"railroad"
#define XtNcircular	"circular"
#define XtNtickTime	"tickTime"
#define XtNshapeWindow	"shapeWindow"

#define XtCNChildren	"NChildren"
#define XtCTickTime	"TickTime"
#define XtCRailroad	"Railroad"
#define XtCCircular	"Circular"
#define XtCTickTime	"TickTime"
#define XtCShapeWindow	"ShapeWindow"

typedef struct _SwissWatchClassRec	*SwissWatchWidgetClass;
typedef struct _SwissWatchRec	*SwissWatchWidget;

extern WidgetClass swissWatchWidgetClass;

#if NeedFunctionPrototypes
extern void SwissWatchReset(SwissWatchWidget);
#else
extern void SwissWatchReset();
#endif

#endif /* not _SwissWatch_h_ */

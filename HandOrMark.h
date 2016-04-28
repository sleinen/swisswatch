#ifndef _HandOrMark_h_
#define _HandOrMark_h_

#define XtNclass	"class"
#define XtNinner	"inner"
#define XtNouter	"outer"
#define XtNcycle	"cycle"
#define XtNstrokeWidth	"strokeWidth"
#define XtNstrokeWidthR	"strokeWidthR"

#define XtCClass	"Class"
#define XtCInner	"Inner"
#define XtCOuter	"Outer"
#define XtCCycle	"Cycle"
#define XtCStrokeWidth	"StrokeWidth"
#define XtCStrokeWidthR	"StrokeWidthR"

#define XtNcap		"cap"
#define XtNjoin		"join"
#define XtNshape	"shape"
#define XtNrender	"render"
#define XtNphase	"phase"
#define XtNcolor	"color"
#define XtNcenterX	"centerX"
#define XtNcenterY	"centerY"

#define XtCCap		"Cap"
#define XtCJoin		"Join"
#define XtCShape	"Shape"
#define XtCRender	"Render"
#define XtCPhase	"Phase"
#define XtCCenterX	"CenterX"
#define XtCCenterY	"CenterY"

typedef struct _HandOrMarkClassRec	*HandOrMarkWidgetClass;
typedef struct _HandOrMarkRec	*HandOrMarkWidget;

extern WidgetClass handOrMarkWidgetClass;

#endif /* not _HandOrMark_h_ */

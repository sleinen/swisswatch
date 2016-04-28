#ifndef _MarkP_h_
#define _MarkP_h_

#include "HandOrMarP.h"
#include "Mark.h"

typedef struct _MarkClassPart {
  int		dummy;
} MarkClassPart;

typedef struct _MarkClassRec {
    ObjectClassPart object_class;
    HandOrMarkClassPart hand_or_mark_class_part;
    MarkClassPart mark_class_part;
} MarkClassRec;

extern MarkClassRec markClassRec;

typedef struct _MarkPart {
  int		dummy;
} MarkPart;

typedef struct _MarkRec {
    ObjectPart  object;
    HandOrMarkPart hand_or_mark;
    MarkPart mark;
} MarkRec;

#endif /* not _MarkP_h_ */

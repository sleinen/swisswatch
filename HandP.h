#ifndef _HandP_h_
#define _HandP_h_

#include "HandOrMarP.h"
#include "Hand.h"

typedef struct _HandClassPart {
  int		dummy;
} HandClassPart;

typedef struct _HandClassRec {
    ObjectClassPart object_class;
    HandOrMarkClassPart hand_or_mark_class_part;
    HandClassPart hand_class_part;
} HandClassRec;

extern HandClassRec handClassRec;

typedef struct _HandPart {
  int		dummy;
} HandPart;

typedef struct _HandRec {
    ObjectPart  object;
    HandOrMarkPart hand_or_mark;
    HandPart hand;
} HandRec;

#endif /* not _HandP_h_ */

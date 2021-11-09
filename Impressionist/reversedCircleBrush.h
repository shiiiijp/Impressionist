//
// reversedCircleBrush.h
//
// The header file for Point Brush. 
//

#ifndef REVERSEDCIRCLEBRUSH_H
#define REVERSEDCIRCLEBRUSH_H

#include "impBrush.h"

class ReversedCircleBrush : public ImpBrush
{
public:
	ReversedCircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif

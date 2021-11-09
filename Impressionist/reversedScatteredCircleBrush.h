//
// reversedScatteredCircleBrush.h
//
// The header file for Point Brush. 
//

#ifndef REVERSEDSCATTEREDCIRCLEBRUSH_H
#define REVERSEDSCATTEREDCIRCLEBRUSH_H

#include "impBrush.h"

class ReversedScatteredCircleBrush : public ImpBrush
{
public:
	ReversedScatteredCircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif

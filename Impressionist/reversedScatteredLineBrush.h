//
// reversedScatteredLineBrush.h
//
// The header file for Point Brush. 
//

#ifndef REVERSEDSCATTEREDLINEBRUSH_H
#define REVERSEDSCATTEREDLINEBRUSH_H

#include "impBrush.h"

class ReversedScatteredLineBrush : public ImpBrush
{
public:
	ReversedScatteredLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif

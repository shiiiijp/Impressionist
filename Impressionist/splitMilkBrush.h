//
// splitMilkBrush.h
//
// The header file for Point Brush. 
//

#ifndef SPLITMILKBRUSH_H
#define SPLITMILKBRUSH_H

#include "impBrush.h"

class SplitMilkBrush : public ImpBrush
{
public:
	SplitMilkBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif

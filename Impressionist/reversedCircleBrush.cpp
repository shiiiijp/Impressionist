//
// reversedCircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "reversedCircleBrush.h"
#include <math.h>

#define PI 3.1415

extern float frand();

ReversedCircleBrush::ReversedCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ReversedCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize((float)size);

	BrushMove(source, target);
}

void ReversedCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	//スライダーつけたあと

	if (pDoc == NULL) {
		printf("ReversedCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	//SetColorAlpha( source, alpha );
	//ダイアログのスライダーからブラシの大きさを取得
	int size = pDoc->getSize();
	float alpha = (pDoc->getAlpha()) / 100.0;
	int div = 12;
	float radius = size / 2.0;
	float Ax, Ay;

	//引数がGL_POLYGONの場合、凸多角形を描画
	glBegin(GL_POLYGON);
	SetColorAlphaR(source, alpha);
	for (int i = 0; i < div; i++) {
		Ax = target.x + radius * cos(2 * PI * i / div);
		Ay = target.y + radius * sin(2 * PI * i / div);
		glVertex2f(Ax, Ay);
	}
	glEnd();
}

void ReversedCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);

}


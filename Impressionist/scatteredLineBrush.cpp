//
// scatteredLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredLineBrush.h"

#include <math.h>
#define PI 3.1415

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize((float)size);

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	//スライダーつけたあと

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	//SetColorAlpha( source, alpha );
	//ダイアログのスライダーからブラシの大きさを取得
	int size = pDoc->getSize();
	int angle = pDoc->getAngle();
	int width = pDoc->getWidth();
	float alpha = (pDoc->getAlpha()) / 100.0;
	int amount = pDoc->getAmount();
	float Ax, Ay, Bx, By, Randx, Randy, Randa;
	Point color;

	for (int i = 0; i < amount; i++) {
		glLineWidth(width);
		glBegin(GL_LINES);
		Randx = target.x - size + rand() % (size * 2);
		Randy = target.y - size + rand() % (size * 2);
		Randa = rand() % 360;
		Ax = Randx + size * cos(PI * Randa / 180);
		Ay = Randy + size * sin(PI * Randa / 180);
		Bx = Randx - size * cos(PI * Randa / 180);
		By = Randy - size * sin(PI * Randa / 180);
		color.x = Randx;
		color.y = Randy;
		SetColorAlpha(color, alpha);
		glVertex2f(Ax, Ay);
		glVertex2f(Bx, By);
		glEnd();
	}
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);

}

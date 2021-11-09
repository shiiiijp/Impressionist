//
// scatteredCircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredCircleBrush.h"

#include <math.h>
#define PI 3.1415

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize((float)size);

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	//スライダーつけたあと

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	//SetColorAlpha( source, alpha );
	//ダイアログのスライダーからブラシの大きさを取得
	int size = pDoc->getSize();
	float alpha = (pDoc->getAlpha()) / 100.0;
	int amount = pDoc->getAmount();
	int rad = pDoc->getRad();
	int div = 12;
	float radius = rad;
	float Ax, Ay, Randx, Randy;
	Point color;

	//引数がGL_POLYGONの場合、凸多角形を描画
	for (int i = 0; i < amount; i++) {
		glBegin(GL_POLYGON);
		Randx = target.x - size / 2 + rand() % size;
		Randy = target.y - size / 2 + rand() % size;
		color.x = Randx;
		color.y = Randy;
		SetColorAlpha(color, alpha);
		for (int i = 0; i < div; i++) {
			Ax = Randx + radius * cos(2 * PI * i / div);
			Ay = Randy + radius * sin(2 * PI * i / div);
			glVertex2f(Ax, Ay);
		}
		glEnd();
	}
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);

}


//
// splitMilkBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "splitMilkBrush.h"

#include <math.h>
#define PI 3.1415

extern float frand();

SplitMilkBrush::SplitMilkBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void SplitMilkBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize((float)size);

	BrushMove(source, target);
}

void SplitMilkBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	//スライダーつけたあと

	if (pDoc == NULL) {
		printf("SplitMilkBrush::BrushMove  document is NULL\n");
		return;
	}

	//SetColorAlpha( source, alpha );
	//ダイアログのスライダーからブラシの大きさを取得
	int size = pDoc->getSize();
	float alpha = (pDoc->getAlpha()) / 100.0;
	int div = 12;
	float radius = 1.0;
	float Ax, Ay, Randx, Randy;

	//引数がGL_POLYGONの場合、凸多角形を描画
	glBegin(GL_POLYGON);
	SetColorAlpha(source, alpha);
	for (int i = 0; i < 10; i++) {
		Randx = target.x - size / 2 + rand() % size;
		Randy = target.y - size / 2 + rand() % size;
		glVertex2f(Randx, Randy);
	}
	glEnd();
}

void SplitMilkBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);

}


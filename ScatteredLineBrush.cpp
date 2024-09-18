#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredlinebrush.h"
#include <cmath>

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	int randNum = round(frand() + 3);
	int line_size = pDoc->getSize();

	for (int i = 0; i < randNum; i++) {
		int newX_multiplier = round(frand() * 3 - frand() * 3) * line_size / 3;
		int newY_multiplier = round(frand() * 2 - frand() * 2) * line_size / 3;

		Point newSource = Point(source.x + newX_multiplier, source.y + newY_multiplier);

		glBegin(GL_LINES);
		SetColor(newSource);

		glVertex2d(target.x - line_size / 2 + newX_multiplier, target.y + newY_multiplier);
		glVertex2d(target.x + line_size - line_size / 2 + newX_multiplier, target.y + newY_multiplier);
		glEnd();
	}
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
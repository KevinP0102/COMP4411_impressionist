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

	glLineWidth(pDoc->getLineWidth());

	int randNum = round(frand() + 3);
	int line_size = pDoc->getSize();

	for (int i = 0; i < randNum; i++) {
		int newX_multiplier = round(frand() * 3 - frand() * 3) * line_size / 3;
		int newY_multiplier = round(frand() * 2 - frand() * 2) * line_size / 3;

		Point newSource = Point(source.x + newX_multiplier, source.y + newY_multiplier);

		glPushMatrix();

		glTranslatef(target.x + newX_multiplier, target.y + newY_multiplier, 0);
		glRotatef(pDoc->getLineAngle(), 0, 0, 1);

		glBegin(GL_LINES);
		SetColor(newSource);

		glVertex2d(-line_size / 2, 0);
		glVertex2d(line_size - line_size / 2, 0);
		glEnd();

		glPopMatrix();
	}
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
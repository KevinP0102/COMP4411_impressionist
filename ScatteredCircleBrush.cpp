#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredcirclebrush.h"
#include <cmath>

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	int segment = 100;
	GLfloat angle;
	float circle_radius = pDoc->getSize();

	glBegin(GL_TRIANGLE_FAN);
	SetColor(source);

	for (int i = 0; i < segment; i++) {
		angle = 2.0f * M_PI * float(i) / float(segment);
		glVertex2f(target.x + circle_radius * cosf(angle), target.y + circle_radius * sinf(angle));
	}

	glEnd();

	int randNum = round(frand() + 3);
	int line_size = pDoc->getSize();

	for (int i = 0; i < randNum; i++) {
		int newX_multiplier = round(frand() * 3 - frand() * 3) * line_size / 5;
		int newY_multiplier = round(frand() * 2 - frand() * 2) * line_size / 5;

		Point newSource = Point(source.x + newX_multiplier, source.y + newY_multiplier);

		glBegin(GL_TRIANGLE_FAN);
		SetColor(newSource);

		for (int i = 0; i < segment; i++) {
			angle = 2.0f * M_PI * float(i) / float(segment);
			glVertex2f(target.x + newX_multiplier + circle_radius * cosf(angle), target.y + newY_multiplier + circle_radius * sinf(angle));
		}

		glEnd();
	}
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
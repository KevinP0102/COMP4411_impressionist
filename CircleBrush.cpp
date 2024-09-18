#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"
#include <cmath>

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}

	int segment = 100;
	GLfloat angle;
	float circle_radius = pDoc->getSize();

	glBegin(GL_TRIANGLE_FAN);
	SetColor(source);

	for (int i = 0; i < segment; i++) {
		angle = 2.0f * 3.1415926f * float(i) / float(segment);
		glVertex2f(target.x + circle_radius * cosf(angle), target.y + circle_radius * sinf(angle));
	}
	
	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
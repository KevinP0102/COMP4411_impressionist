#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	int line_size = pDoc->getSize();

	glLineWidth(pDoc->getLineWidth());

	glPushMatrix();

	glTranslatef(target.x, target.y, 0);
	glRotatef(pDoc->getLineAngle(), 0, 0, 1);

	glBegin(GL_LINES);
	SetColor(source);

	glVertex2d(- line_size / 2, 0);
	glVertex2d(line_size - line_size / 2, 0);
	glEnd();

	glPopMatrix();

	
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
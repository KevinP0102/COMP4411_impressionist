#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredpointbrush.h"
#include <cmath>

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	glPointSize(1);

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();

	for (int i = target.x - size / 2; i < target.x + size / 2; i++) {
		for (int j = target.y - size / 2; j < target.y + size / 2; j++) {
			if (frand() < 0.3) {
				Point newSource = Point(source.x + i - target.x, source.y + j - target.y);

				glBegin(GL_POINTS);
				SetColor(newSource);

				glVertex2d(i, j);

				glEnd();
			}
		}
	}

}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
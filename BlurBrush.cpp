#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "blurbrush.h"

extern float frand();

BlurBrush::BlurBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void BlurBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void BlurBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int halfSize = size / 2;
	
	const int kernelSize = 3;
	const int halfKernelSize = kernelSize / 2;
	const float kernelSum = kernelSize * kernelSize;

	for (int y = -halfSize; y <= halfSize; ++y) {
		for (int x = -halfSize; x <= halfSize; ++x) {
			Point blurPoint(target.x + x, target.y + y);
			applyBlur(blurPoint);
		}
	}
}

void BlurBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void BlurBrush::applyBlur(const Point& target)
{
	ImpressionistDoc* pDoc = GetDocument();
	int width = pDoc->m_nWidth;
	int height = pDoc->m_nHeight;

	if (target.x < 0 || target.x >= width || target.y < 0 || target.y >= height) {
		return;
	}

	const int kernelSize = 3;
	const int halfKernelSize = kernelSize / 2;
	const float kernelSum = kernelSize * kernelSize;

	float red = 0.0f, green = 0.0f, blue = 0.0f;

	for (int ky = -halfKernelSize; ky <= halfKernelSize; ++ky) {
		for (int kx = -halfKernelSize; kx <= halfKernelSize; ++kx) {
			int sampleX = min(max(target.x + kx, 0), width - 1);
			int sampleY = min(max(target.y + ky, 0), height - 1);

			GLubyte* pixel = pDoc->GetPaintingPixel(sampleX, sampleY);

			red += pixel[0];
			green += pixel[1];
			blue += pixel[2];
		}
	}

	red /= kernelSum;
	green /= kernelSum;
	blue /= kernelSum;

	glColor3f(red / 255.0f, green / 255.0f, blue / 255.0f);
	glBegin(GL_POINTS);
	glVertex2d(target.x, target.y);
	glEnd();
}
//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

extern float frand();

#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;
static Point	startDrag;
static Point	endDrag;
static bool		dragging = false;
static float	dx, dy, angle, gradx, grady;
static int		X, Y;


PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}

void PaintView::convertToGrayscale(GLubyte* src, GLubyte* dst, int width, int height)
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			GLubyte* pixel = m_pDoc->GetPixel(src, x, y, m_pDoc->m_nWidth, m_pDoc->m_nHeight);
			dst[y * width + x] = static_cast<GLubyte>(0.299 * pixel[0] +
															0.587 * pixel[1] +
															0.144 * pixel[2]);
		}
	}
}

void PaintView::boxBlurImage(GLubyte* src, GLubyte* dest, int width, int height)
{
	const int kernelSize = 3;
	const int halfKernelSize = kernelSize / 2;
	const float kernelSum = kernelSize * kernelSize;

	for (int y = halfKernelSize; y < height - halfKernelSize; ++y)
	{
		for (int x = halfKernelSize; x < width - halfKernelSize; ++x)
		{
			float sum = 0.0f;
			for (int ky = -halfKernelSize; ky <= halfKernelSize; ++ky)
			{
				for (int kx = -halfKernelSize; kx <= halfKernelSize; ++kx)
				{
					sum += src[(y + ky) * width + (x + kx)];
				}
			}
			dest[y * width + x] = static_cast<GLubyte>(sum / kernelSum);
		}
	}
}

void PaintView::calculateGradient(GLubyte* src, const Point& source)
{
	int x = source.x;
	int y = source.y;
	int width = m_pDoc->m_nWidth;
	int height = m_pDoc->m_nHeight;
	GLubyte* blurredBitmap = src;

	if (x > 0 && x < width - 1 && y > 0 && y < height - 1)
	{
		gradx = (blurredBitmap[(y - 1) * width + (x + 1)] - blurredBitmap[(y - 1) * width + (x - 1)] +
				2 * blurredBitmap[y * width + (x + 1)] - 2 * blurredBitmap[y * width + (x - 1)] +
				blurredBitmap[(y + 1) * width + (x + 1)] - blurredBitmap[(y + 1) * width + (x - 1)]) / 8.0f;

		grady = (blurredBitmap[(y + 1) * width + (x - 1)] - blurredBitmap[(y - 1) * width + (x - 1)] +
				2 * blurredBitmap[(y + 1) * width + x] - 2 * blurredBitmap[(y - 1) * width + x] +
				blurredBitmap[(y + 1) * width + (x + 1)] - blurredBitmap[(y - 1) * width + (x + 1)]) / 8.0f;
	}
	else
	{
		gradx = 0.0f;
		grady = 0.0f;
	}
}

void PaintView::display()
{
	RestoreContent();

	if (dragging) {
		glLineWidth(2);
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex2d(startDrag.x, startDrag.y);
		glVertex2d(endDrag.x, endDrag.y);
		glEnd();
	}

}

void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();
	}

	if ( m_pDoc->m_ucPainting && isAnEvent) //
	{

		// Clear it after processing.
		isAnEvent	= 0;

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		
		

		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			startDrag.x = target.x;
			startDrag.y = target.y;

			if (target.x >= 0 && target.x < m_pDoc->m_nPaintWidth && 
				m_nWindowHeight - target.y >= 0 && m_nWindowHeight - target.y < m_pDoc->m_nPaintHeight)
				m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:

			if (m_pDoc->m_pCurrentDirection == CURSOR) {
				endDrag.x = target.x;
				endDrag.y = target.y;

				dx = endDrag.x - startDrag.x;
				dy = endDrag.y - startDrag.y;
				angle = atan2(dy, dx) * 180.0 / M_PI;
				if (angle < 0) angle += 360;

				m_pDoc->m_pUI->setLineAngle(angle);

				startDrag.x = endDrag.x;
				startDrag.y = endDrag.y;
			}
			else if (m_pDoc->m_pCurrentDirection == GRADIENT) {
				std::vector<GLubyte> grayscaleBitmap(m_nDrawWidth * m_nDrawHeight);
				std::vector<GLubyte> blurredBitmap(m_nDrawWidth * m_nDrawHeight);

				if (m_pDoc->m_pUI->getAnotherGradient()) {
					convertToGrayscale(m_pDoc->m_ucAnotherBitmap, grayscaleBitmap.data(), m_nDrawWidth, m_nDrawHeight);
				}
				else {
					convertToGrayscale(m_pDoc->m_ucBitmap, grayscaleBitmap.data(), m_nDrawWidth, m_nDrawHeight);
				}
				boxBlurImage(grayscaleBitmap.data(), blurredBitmap.data(), m_nDrawWidth, m_nDrawHeight);

				calculateGradient(blurredBitmap.data(), source);

				angle = (atan2(grady, gradx) + M_PI/2) * 180.0f / M_PI;
				if (angle < 0) angle += 360;

				m_pDoc->m_pUI->setLineAngle(angle);
			}

			if (target.x >= 0 && target.x < m_pDoc->m_nPaintWidth && 
				m_nWindowHeight - target.y >= 0 && m_nWindowHeight - target.y < m_pDoc->m_nPaintHeight)
				m_pDoc->m_pCurrentBrush->BrushMove( source, target );

			if (m_pDoc->m_pUI->m_BrushTypeChoice->value() == BRUSH_BLUR) {
				SaveCurrentContent();
				RestoreContent();
			}
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			startDrag.x = target.x;
			startDrag.y = target.y;
			break;
		case RIGHT_MOUSE_DRAG:
			dragging = true;
			endDrag.x = target.x;
			endDrag.y = target.y;
			display();
			break;
		case RIGHT_MOUSE_UP:
			dragging = false; 
			endDrag.x = target.x;
			endDrag.y = target.y;
			display();

			dx = endDrag.x - startDrag.x;
			dy = endDrag.y - startDrag.y;
			angle = atan2(dy, dx) * 180.0 / M_PI;
			if (angle < 0) angle += 360;
			m_pDoc->m_pUI->setLineAngle(angle);
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();

	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();

		m_pDoc->m_pUI->m_origView->drawCursor(coord.x, coord.y);

		break;
	
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;

	case FL_LEAVE:
		m_pDoc->m_pUI->m_origView->drawCursor(-1, -1);

		break;

	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}

void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::RandomBrushOrder()
{

	int sizeArray = m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth;

	order = new int[sizeArray];

	for (int i = 0; i < sizeArray; i++)
	{
		order[i] = i;
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(order, order + sizeArray, g);

}

void PaintView::autoDraw()
{
	make_current();

	if (m_pDoc->m_ucPainting == NULL)
	{
		fl_alert("Please load an image first.");
		return;
	}

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable(GL_DEPTH_TEST);

		ortho();

		glClear(GL_COLOR_BUFFER_BIT);
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y = 0;

	m_nWindowWidth = w();
	m_nWindowHeight = h();

	int drawWidth, drawHeight;
	drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
	drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if (startrow < 0) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting +
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth = drawWidth;
	m_nDrawHeight = drawHeight;

	m_nStartRow = startrow;
	m_nEndRow = startrow + drawHeight;
	m_nStartCol = scrollpos.x;
	m_nEndCol = m_nStartCol + drawWidth;

	int sizeArray = m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth;

	RandomBrushOrder();

	std::vector<GLubyte> grayscaleBitmap(m_nDrawWidth * m_nDrawHeight);
	std::vector<GLubyte> blurredBitmap(m_nDrawWidth * m_nDrawHeight);

	if (m_pDoc->m_pUI->getAnotherGradient()) {
		convertToGrayscale(m_pDoc->m_ucAnotherBitmap, grayscaleBitmap.data(), m_nDrawWidth, m_nDrawHeight);
	}
	else {
		convertToGrayscale(m_pDoc->m_ucBitmap, grayscaleBitmap.data(), m_nDrawWidth, m_nDrawHeight);
	}
	
	boxBlurImage(grayscaleBitmap.data(), blurredBitmap.data(), m_nDrawWidth, m_nDrawHeight);

	int size, lineWidth, lineAngle, randSize, randlineWidth, randlineAngle;
	size = m_pDoc->getSize();
	lineWidth = m_pDoc->getLineWidth();
	lineAngle = m_pDoc->getLineAngle();

	for (int i = 0; i < sizeArray; i++)
	{
		int x = order[i] % m_pDoc->m_nPaintWidth;
		int y = order[i] / m_pDoc->m_nPaintWidth;

		randSize = size + frand() * 4 - frand() * 4;
		randlineWidth = lineWidth + frand() * 4 - frand() * 4;
		randlineAngle = lineAngle + frand() * 10 - frand() * 10;

		//clamping values
		if (randSize < 1) randSize = 1;
		else if (randSize > 40) randSize = 40;
		if (randlineWidth < 1) randlineWidth = 1;
		else if (randlineWidth > 20) randlineWidth = 20;
		if (randlineAngle < 0) randlineAngle = 0;
		else if (randlineAngle > 359) randlineAngle = 359;

		m_pDoc->m_pUI->setSize(randSize);
		m_pDoc->m_pUI->setLineWidth(randlineWidth);
		m_pDoc->m_pUI->setLineAngle(randlineAngle);

		Point source(x + m_nStartCol, m_nEndRow - y);
		Point target(x, m_nWindowHeight - y);

		if (m_pDoc->m_pCurrentDirection == GRADIENT) {

			calculateGradient(blurredBitmap.data(), source);

			angle = (atan2(grady, gradx) + M_PI / 2) * 180.0f / M_PI;
			if (angle < 0) angle += 360;

			m_pDoc->m_pUI->setLineAngle(angle);
		}

		if (frand() > 0.1)
			m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
	}
	
	m_pDoc->m_pUI->setSize(size);
	m_pDoc->m_pUI->setLineWidth(lineWidth);
	m_pDoc->m_pUI->setLineAngle(lineAngle);

	SaveCurrentContent();
	RestoreContent();
	glFlush();
	refresh();
}
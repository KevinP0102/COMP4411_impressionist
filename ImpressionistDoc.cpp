// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredCircleBrush.h"
#include "BlurBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc()
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_pUndoPainting = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_BLUR]
		= new BlurBrush(this, "Blur");

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

void ImpressionistDoc::setDirectionType(int type)
{
	m_pCurrentDirection = type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Returns the line width of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getLineWidth()
{
	return m_pUI->getLineWidth();
}

//---------------------------------------------------------
// Returns the line angle of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getLineAngle()
{
	return m_pUI->getLineAngle();
}

float ImpressionistDoc::getOpacity()
{
	return m_pUI->getOpacity();
}

float ImpressionistDoc::getRedIntensity()
{
	return m_pUI->getRedIntensity();
}

float ImpressionistDoc::getGreenIntensity()
{
	return m_pUI->getGreenIntensity();
}

float ImpressionistDoc::getBlueIntensity()
{
	return m_pUI->getBlueIntensity();
}

GLubyte ImpressionistDoc::clampIntensity(float intensity)
{
	if (intensity > 255)
		return 255;
	if (intensity < 0)
		return 0;
	return static_cast<GLubyte>(intensity);
}

void ImpressionistDoc::applyIntensity(float red, float green, float blue)
{
	for (int i = 0; i < m_nWidth * m_nHeight * 3; i += 3)
	{
		m_ucBitmap[i] = clampIntensity(m_ucBitmap[i] * red);
		m_ucBitmap[i + 1] = clampIntensity(m_ucBitmap[i + 1] * green);
		m_ucBitmap[i + 2] = clampIntensity(m_ucBitmap[i + 2] * blue);
	}

	m_pUI->m_origView->refresh();
	return;
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	if (m_pUndoPainting) delete[] m_pUndoPainting;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUndoPainting = new GLubyte[width * height * 3];
	memset(m_pUndoPainting, 0, width * height * 3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}

	if (m_pUndoPainting)
	{
		delete[] m_pUndoPainting;

		m_pUndoPainting = new GLubyte[m_nPaintWidth * m_nPaintHeight * 3];
		memset(m_pUndoPainting, 0, m_nPaintWidth * m_nPaintHeight * 3);
		
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

GLubyte* ImpressionistDoc::GetPaintingPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nPaintWidth)
		x = m_nPaintWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nPaintHeight)
		y = m_nPaintHeight - 1;

	return (GLubyte*)(m_ucPainting + 3 * (y * m_nPaintWidth + x));
}

GLubyte* ImpressionistDoc::GetPaintingPixel(const Point p)
{
	return GetPaintingPixel(p.x, p.y);
}

void ImpressionistDoc::SwapBitmaps()
{
	unsigned char* temp = m_ucBitmap;
	m_ucBitmap = m_ucPainting;
	m_ucPainting = temp;

	int tempWidth = m_nWidth;
	m_nWidth = m_nPaintWidth;
	m_nPaintWidth = tempWidth;

	int tempHeight = m_nHeight;
	m_nHeight = m_nPaintHeight;
	m_nPaintHeight = tempHeight;

	m_pUI->m_origView->resizeWindow(m_nWidth, m_nHeight);
	m_pUI->m_origView->refresh();

	m_pUI->m_paintView->resizeWindow(m_nPaintWidth, m_nPaintHeight);
	m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::SaveUndoPainting()
{
	int width = m_nPaintWidth;
	int height = m_nPaintHeight;

	if (m_pUndoPainting == NULL)
	{
		m_pUndoPainting = new GLubyte[width * height * 3];
		memset(m_pUndoPainting, 0, width * height * 3);
	}

	if (m_ucPainting == NULL)
	{
		return;
	}

	memcpy(m_pUndoPainting, m_ucPainting, width * height * 3);
}

void ImpressionistDoc::RestoreUndoPainting()
{
	if (m_pUndoPainting == NULL)
	{
		return;
	}

	int width = m_nPaintWidth;
	int height = m_nPaintHeight;

	memcpy(m_ucPainting, m_pUndoPainting, width * height * 3);

	// Redraw the painting
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, m_ucPainting);
	glFlush();

}
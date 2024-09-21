// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	int		getLineWidth();					// get the UI line width
	void	setLineWidth(int lineWidth);	// set the UI line width
	int		getLineAngle();					// get the UI line angle
	void	setLineAngle(int lineAngle);	// set the UI line angle
	float	getOpacity();					// get the UI opacity
	void	setOpacity(float opacity);		// set the UI opacity
	char*	getImageName();					// get the current image name
	float	getRedIntensity();				// get the UI red intensity
	void	setRedIntensity(float redIntensity);	// set the UI red intensity
	float	getGreenIntensity();			// get the UI green intensity
	void	setGreenIntensity(float greenIntensity);	// set the UI green intensity
	float	getBlueIntensity();				// get the UI blue intensity
	void	setBlueIntensity(float blueIntensity);	// set the UI blue intensity


	void	applyIntensity(float red, float green, float blue);
	GLubyte clampIntensity(float intensity);

	void	setDirectionType(int type);

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;

	int					m_pCurrentDirection;
	GLubyte*			m_pUndoPainting;

	// Size of the brush.
	int m_nSize;							


	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	GLubyte* GetPaintingPixel(int x, int y);
	GLubyte* GetPaintingPixel(const Point p);
	void SwapBitmaps();
	void SaveUndoPainting();
	void RestoreUndoPainting();

private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif

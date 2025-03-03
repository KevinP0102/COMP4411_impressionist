//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Window*			m_colorScaleDialog;
	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*			m_DirectionTypeChoice;

	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_LineWidthSlider;
	Fl_Slider*			m_LineAngleSlider;
	Fl_Slider*			m_RedIntensitySlider;
	Fl_Slider*			m_GreenIntensitySlider;
	Fl_Slider*			m_BlueIntensitySlider;
	Fl_Slider*			m_OpacitySlider;
	Fl_Slider*			m_DissolveFactorSlider;
	Fl_Button*          m_ClearCanvasButton;
	Fl_Button*			m_ConfirmIntensityButton;
	Fl_Button*			m_AutoDrawButton;
	Fl_Light_Button*	m_AnotherGradientButton;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
	void				setSize(int size);
	int					getLineWidth();
	void				setLineWidth(int lineWidth);
	int					getLineAngle();
	void				setLineAngle(int lineAngle);
	float				getOpacity();
	void				setOpacity(float opacity);
	float				getRedIntensity();
	void				setRedIntensity(float redIntensity);
	float				getGreenIntensity();
	void				setGreenIntensity(float greenIntensity);
	float				getBlueIntensity();
	void				setBlueIntensity(float blueIntensity);
	float				getDissolveFactor();
	void				setDissolveFactor(float dissolveFactor);

	bool				getAnotherGradient();

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
	int		m_nLineWidth;
	int		m_nLineAngle;
	float	m_nOpacity;
	float   m_nRedIntensity;
	float   m_nGreenIntensity;
	float   m_nBlueIntensity;
	bool	m_bAutoDrawing;
	bool	m_bAnotherGradient;
	float   m_nDissolveFactor;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
	static Fl_Menu_Item		directionTypeMenu[NUM_DIRECTION_TYPE + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void cb_color_scale(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void cb_swap(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);

	static void cb_lineWidthSlides(Fl_Widget* o, void* v);
	static void cb_lineAngleSlides(Fl_Widget* o, void* v);
	static void cb_opacitySlides(Fl_Widget* o, void* v);
	static void cb_redIntensitySlides(Fl_Widget* o, void* v);
	static void cb_greenIntensitySlides(Fl_Widget* o, void* v);
	static void cb_blueIntensitySlides(Fl_Widget* o, void* v);
	static void cb_dissolveFactorSlides(Fl_Widget* o, void* v);

	static void cb_DirectionChoice(Fl_Widget* o, void* v);
	static void cb_confirmIntensityButton(Fl_Widget* o, void* v);
	static void cb_undo(Fl_Menu_* o, void* v);
	static void cb_autoDraw(Fl_Widget* o, void* v);
	static void cb_load_another_image(Fl_Menu_* o, void* v);
	static void cb_display_original(Fl_Menu_* o, void* v);
	static void cb_display_another(Fl_Menu_* o, void* v);
	static void cb_another_gradient(Fl_Widget* o, void* v);
	static void cb_muralImage(Fl_Menu_* o, void* v);
	static void cb_dissolve(Fl_Menu_* o, void* v);
	
};

#endif

//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}

void ImpressionistUI::cb_load_another_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if (pDoc->m_ucBitmap == NULL)
	{
		fl_alert("Please load an original image first.");
		return;
	}

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getAnotherImageName());
	if (newfile != NULL) {
		pDoc->loadAnotherImage(newfile);
	}

}

void ImpressionistUI::cb_muralImage(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if (pDoc->m_ucBitmap == NULL)
	{
		fl_alert("Please load an original image first.");
		return;
	}
	
	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getAnotherImageName());
	if (newfile != NULL) {
		pDoc->loadMuralImage(newfile);
	}
}

void ImpressionistUI::cb_dissolve(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if (pDoc->m_ucBitmap == NULL || pDoc->m_ucAnotherBitmap == NULL)
	{
		fl_alert("Please load both images first.");
		return;
	}

	pDoc->DissolveImages(pDoc->getDissolveFactor());
}

//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

void ImpressionistUI::cb_color_scale(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if (pDoc->m_ucBitmap == NULL)
	{
		fl_alert("Please load an image first.");
	}
	else
	{
		whoami(o)->m_colorScaleDialog->show();
	}

	return;
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

void ImpressionistUI::cb_swap(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if (pDoc->m_ucBitmap == NULL)
	{
		fl_alert("Please load an image first.");
	}
	else
	{
		pDoc->SwapBitmaps();
	}

	return;
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;

	if (type == BRUSH_LINES || type == BRUSH_SCATTERED_LINES)
	{
		pUI->m_LineWidthSlider->activate();
		pUI->m_LineAngleSlider->activate();
		pUI->m_DirectionTypeChoice->activate();
	}
	else
	{
		pUI->m_LineWidthSlider->deactivate();
		pUI->m_LineAngleSlider->deactivate();
		pUI->m_DirectionTypeChoice->deactivate();
	}

	if (type == BRUSH_BLUR)
		pUI->m_OpacitySlider->deactivate();
	else
		pUI->m_OpacitySlider->activate();

	pDoc->SaveUndoPainting();
	pDoc->setBrushType(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}

//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize= int( ((Fl_Slider *)o)->value() );
}

void ImpressionistUI::cb_lineWidthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nLineWidth = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_lineAngleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nLineAngle = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_opacitySlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nOpacity = float(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_DirectionChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setDirectionType(type);
}

void ImpressionistUI::cb_redIntensitySlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nRedIntensity = float(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_greenIntensitySlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nGreenIntensity = float(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_blueIntensitySlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nBlueIntensity = float(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_dissolveFactorSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nDissolveFactor = float(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_confirmIntensityButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	pDoc->applyIntensity(pUI->getRedIntensity(), pUI->getGreenIntensity(), pUI->getBlueIntensity());

	pUI->setRedIntensity(1.0f);
	pUI->setGreenIntensity(1.0f);
	pUI->setBlueIntensity(1.0f);

	pUI->m_colorScaleDialog->hide();
}

void ImpressionistUI::cb_undo(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	pDoc->RestoreUndoPainting();

	whoami(o)->m_paintView->refresh();
}

void ImpressionistUI::cb_autoDraw(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	pUI->m_bAutoDrawing = true;

	pUI->m_paintView->autoDraw();

	pUI->m_bAutoDrawing = false;

}

void ImpressionistUI::cb_display_original(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if (pDoc->m_ucBitmap == NULL)
	{
		fl_alert("Please load an image first.");
	}
	else
	{
		pDoc->m_pCurrentImageType = ORIGINAL_IMAGE;
	}

	whoami(o)->m_origView->refresh();
}

void ImpressionistUI::cb_display_another(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if (pDoc->m_ucAnotherBitmap == NULL)
	{
		fl_alert("Please load another image first.");
	}
	else
	{
		pDoc->m_pCurrentImageType = ANOTHER_IMAGE;
	}

	whoami(o)->m_origView->refresh();
}

void ImpressionistUI::cb_another_gradient(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	if (pDoc->m_ucAnotherBitmap == NULL)
	{
		fl_alert("Please load another image first.");
		pUI->m_AnotherGradientButton->value(0);
	}
	else
	{
		pUI->m_bAnotherGradient = !pUI->m_bAnotherGradient;
	}
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40 && !m_bAutoDrawing) 
		m_BrushSizeSlider->value(m_nSize);
}

int ImpressionistUI::getLineWidth()
{
	return m_nLineWidth;
}

void ImpressionistUI::setLineWidth(int lineWidth)
{
	m_nLineWidth = lineWidth;

	if (lineWidth <= 20 && !m_bAutoDrawing)
		m_LineWidthSlider->value(m_nLineWidth);
}

int ImpressionistUI::getLineAngle()
{
	return m_nLineAngle;
}

void ImpressionistUI::setLineAngle(int lineAngle)
{
	m_nLineAngle = lineAngle;

	if (m_pDoc->m_pCurrentDirection != CURSOR && 
		m_pDoc->m_pCurrentDirection != GRADIENT &&
		!m_bAutoDrawing &&
		lineAngle <= 359)
		m_LineAngleSlider->value(m_nLineAngle);
}

float ImpressionistUI::getOpacity()
{
	return m_nOpacity;
}

void ImpressionistUI::setOpacity(float opacity)
{
	m_nOpacity = opacity;

	if (opacity <= 1)
		m_OpacitySlider->value(m_nOpacity);
}

float ImpressionistUI::getRedIntensity()
{
	return m_nRedIntensity;
}

void ImpressionistUI::setRedIntensity(float redIntensity)
{
	m_nRedIntensity = redIntensity;

	if (redIntensity <= 1.5f)
		m_RedIntensitySlider->value(m_nRedIntensity);
}

float ImpressionistUI::getGreenIntensity()
{
	return m_nGreenIntensity;
}

void ImpressionistUI::setGreenIntensity(float greenIntensity)
{
	m_nGreenIntensity = greenIntensity;

	if (greenIntensity <= 1.5f)
		m_GreenIntensitySlider->value(m_nGreenIntensity);
}

float ImpressionistUI::getBlueIntensity()
{
	return m_nBlueIntensity;
}

void ImpressionistUI::setBlueIntensity(float blueIntensity)
{
	m_nBlueIntensity = blueIntensity;

	if (blueIntensity <= 1.5f)
		m_BlueIntensitySlider->value(m_nBlueIntensity);
}

bool ImpressionistUI::getAnotherGradient()
{
	return m_bAnotherGradient;
}

float ImpressionistUI::getDissolveFactor()
{
	return m_nDissolveFactor;
}

void ImpressionistUI::setDissolveFactor(float dissolveFactor)
{
	m_nDissolveFactor = dissolveFactor;

	if (dissolveFactor <= 1.0f)
		m_DissolveFactorSlider->value(m_nDissolveFactor);
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Clear Canvas",	FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		
		{ "&Load Another Image", FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_load_another_image },
		{ "New Mural Image", FL_ALT + 'm', (Fl_Callback*)ImpressionistUI::cb_muralImage},
		{"&Dissolve", FL_ALT + 'd', (Fl_Callback*)ImpressionistUI::cb_dissolve},
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },
	
	{ "&Display",	0, 0, 0, FL_SUBMENU },
		{ "&Original Image", FL_ALT + 'o', (Fl_Callback*)ImpressionistUI::cb_display_original },
		//{ "&Edge Image", FL_ALT + 'e', (Fl_Callback*)ImpressionistUI::cb_display_edge },
		{ "&Another Image", FL_ALT + 'n', (Fl_Callback*)ImpressionistUI::cb_display_another },
		{ 0 },


	{ "&Option",		0, 0, 0, FL_SUBMENU },
		{ "&Swap Views",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_swap },
		{ "&Color Scale",	FL_ALT + 'o', (Fl_Callback*)ImpressionistUI::cb_color_scale },
		{ "&Undo",			FL_CTRL + 'z', (Fl_Callback*)ImpressionistUI::cb_undo },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",			FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT + 'd', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_SCATTERED_CIRCLES},
  {"Filter - Blur",		FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_BLUR},
  {0}
};

Fl_Menu_Item ImpressionistUI::directionTypeMenu[NUM_DIRECTION_TYPE + 1] = {
	{"Slider/Right Mouse", FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_DirectionChoice, (void*)SLIDER},
	{"Gradient", FL_ALT + 'g', (Fl_Callback*)ImpressionistUI::cb_DirectionChoice, (void*)GRADIENT},
	{"Cursor", FL_ALT + 'c', (Fl_Callback*)ImpressionistUI::cb_DirectionChoice, (void*)CURSOR},
	{0}
};

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_nLineWidth = 5;
	m_nLineAngle = 0;
	m_nOpacity = 1.00f;
	m_nRedIntensity = 1.0f;
	m_nGreenIntensity = 1.0f;
	m_nBlueIntensity = 1.0f;
	m_nDissolveFactor = 1.0f;
	m_bAutoDrawing = false;
	m_bAnotherGradient = false;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");

		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_DirectionTypeChoice = new Fl_Choice(115, 45, 150, 25, "&Stroke Direction");
		m_DirectionTypeChoice->user_data((void*)(this));	 // record self to be used by static callback functions
		m_DirectionTypeChoice->menu(directionTypeMenu);
		m_DirectionTypeChoice->callback(cb_DirectionChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		m_LineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
		m_LineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_LineWidthSlider->labelfont(FL_COURIER);
		m_LineWidthSlider->labelsize(12);
		m_LineWidthSlider->bounds(1, 20);
		m_LineWidthSlider->step(1);
		m_LineWidthSlider->value(m_nLineWidth);
		m_LineWidthSlider->align(FL_ALIGN_RIGHT);
		m_LineWidthSlider->callback(cb_lineWidthSlides);

		m_LineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
		m_LineAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_LineAngleSlider->labelfont(FL_COURIER);
		m_LineAngleSlider->labelsize(12);
		m_LineAngleSlider->bounds(0, 359);
		m_LineAngleSlider->step(1);
		m_LineAngleSlider->value(m_nLineAngle);
		m_LineAngleSlider->align(FL_ALIGN_RIGHT);
		m_LineAngleSlider->callback(cb_lineAngleSlides);

		m_OpacitySlider = new Fl_Value_Slider(10, 170, 300, 20, "Opacity");
		m_OpacitySlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_OpacitySlider->type(FL_HOR_NICE_SLIDER);
		m_OpacitySlider->labelfont(FL_COURIER);
		m_OpacitySlider->labelsize(12);
		m_OpacitySlider->bounds(0.01f, 1.00f);
		m_OpacitySlider->step(0.01f);
		m_OpacitySlider->value(m_nOpacity);
		m_OpacitySlider->align(FL_ALIGN_RIGHT);
		m_OpacitySlider->callback(cb_opacitySlides);

		m_DissolveFactorSlider = new Fl_Value_Slider(10, 200, 300, 20, "Dissolve");
		m_DissolveFactorSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_DissolveFactorSlider->type(FL_HOR_NICE_SLIDER);
		m_DissolveFactorSlider->labelfont(FL_COURIER);
		m_DissolveFactorSlider->labelsize(12);
		m_DissolveFactorSlider->bounds(0.01f, 1.00f);
		m_DissolveFactorSlider->step(0.01f);
		m_DissolveFactorSlider->value(m_nDissolveFactor);
		m_DissolveFactorSlider->align(FL_ALIGN_RIGHT);
		m_DissolveFactorSlider->callback(cb_dissolveFactorSlides);

		m_AutoDrawButton = new Fl_Button(320, 300, 70, 20, "&Paint");
		m_AutoDrawButton->user_data((void*)(this));
		m_AutoDrawButton->callback(cb_autoDraw);

		m_AnotherGradientButton = new Fl_Light_Button(10, 300, 150, 20, "&Another Gradient");
		m_AnotherGradientButton->user_data((void*)(this));
		m_AnotherGradientButton->callback(cb_another_gradient);

		if (m_BrushTypeChoice->value() == BRUSH_LINES || m_BrushTypeChoice->value() == BRUSH_SCATTERED_LINES)
		{
			m_LineWidthSlider->activate();
			m_LineAngleSlider->activate();
			m_DirectionTypeChoice->activate();
		}
		else
		{
			m_LineWidthSlider->deactivate();
			m_LineAngleSlider->deactivate();
			m_DirectionTypeChoice->deactivate();
		}

		if (m_BrushTypeChoice->value() == BRUSH_BLUR)
			m_OpacitySlider->deactivate();
		else
			m_OpacitySlider->activate();

    m_brushDialog->end();	

	m_colorScaleDialog = new Fl_Window(450, 125, "Color Intensity Dialog");

		m_RedIntensitySlider = new Fl_Value_Slider(10, 10, 300, 20, "Red Intensity");
		m_RedIntensitySlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_RedIntensitySlider->type(FL_HOR_NICE_SLIDER);
		m_RedIntensitySlider->labelfont(FL_COURIER);
		m_RedIntensitySlider->labelsize(12);
		m_RedIntensitySlider->bounds(0.5f, 1.5f);
		m_RedIntensitySlider->step(0.01f);
		m_RedIntensitySlider->value(m_nRedIntensity);
		m_RedIntensitySlider->align(FL_ALIGN_RIGHT);
		m_RedIntensitySlider->callback(cb_redIntensitySlides);

		m_GreenIntensitySlider = new Fl_Value_Slider(10, 40, 300, 20, "Green Intensity");
		m_GreenIntensitySlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_GreenIntensitySlider->type(FL_HOR_NICE_SLIDER);
		m_GreenIntensitySlider->labelfont(FL_COURIER);
		m_GreenIntensitySlider->labelsize(12);
		m_GreenIntensitySlider->bounds(0.5f, 1.5f);
		m_GreenIntensitySlider->step(0.01f);
		m_GreenIntensitySlider->value(m_nGreenIntensity);
		m_GreenIntensitySlider->align(FL_ALIGN_RIGHT);
		m_GreenIntensitySlider->callback(cb_greenIntensitySlides);

		m_BlueIntensitySlider = new Fl_Value_Slider(10, 70, 300, 20, "Blue Intensity");
		m_BlueIntensitySlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BlueIntensitySlider->type(FL_HOR_NICE_SLIDER);
		m_BlueIntensitySlider->labelfont(FL_COURIER);
		m_BlueIntensitySlider->labelsize(12);
		m_BlueIntensitySlider->bounds(0.5f, 1.5f);
		m_BlueIntensitySlider->step(0.01f);
		m_BlueIntensitySlider->value(m_nBlueIntensity);
		m_BlueIntensitySlider->align(FL_ALIGN_RIGHT);
		m_BlueIntensitySlider->callback(cb_blueIntensitySlides);

		m_ConfirmIntensityButton = new Fl_Button(150, 100, 150, 20, "&Confirm Intensity");
		m_ConfirmIntensityButton->user_data((void*)(this));
		m_ConfirmIntensityButton->callback(cb_confirmIntensityButton);

	m_colorScaleDialog->end();

}

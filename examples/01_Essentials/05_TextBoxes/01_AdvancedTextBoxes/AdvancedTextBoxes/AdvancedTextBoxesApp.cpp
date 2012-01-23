/////////////////////////////////////////
//
// pocode : Advanced TextBoxes
//
/////////////////////////////////////////

#include "AdvancedTextBoxesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"							// include poTextBox.h to be able to use poTextBox


// APP CONSTRUCTOR.
// Create all objects here.
AdvancedTextBoxesApp::AdvancedTextBoxesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. poTextBox with alignment ///////////////////////
	
	poTextBox* A = new poTextBox(220, 120);
	A->setText("This text is is set in 18 point Helvetica. It is red and aligned center.");
	A->setFont( poGetFont("Helvetica", "Regular") );
	A->setTextSize(18);
	A->textColor = poColor::red;
	A->setTextAlignment(PO_ALIGN_TOP_CENTER);
	A->doLayout();
	A->drawBounds = true;
	A->position.set(75, 185, 0);
	addChild( A );
	
	
	// B. poTextBox with  ///////////////////////
	
	poTextBox* B = new poTextBox(220, 120);
	B->setText("This is a <b>rich text</b> example...");
//	B->setRichText( true );
	B->setFont( poGetFont("Helvetica", "Regular") );
//	B->setFont( poGetFont("Helvetica", "Bold"), PO_TEXT_BOLD );
	B->setTextSize(18);									// Set the text size
	B->textColor = poColor::blue;
	B->doLayout();
	B->drawBounds= true;
	B->position.set(445, 185, 0);
	addChild( B );
}

 
// APP DESTRUCTOR
// Delete all objects here. (optional)
AdvancedTextBoxesApp::~AdvancedTextBoxesApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void AdvancedTextBoxesApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void AdvancedTextBoxesApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void AdvancedTextBoxesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}

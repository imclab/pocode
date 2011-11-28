/////////////////////////////////////////
//
// LESSON : Update function
//
/////////////////////////////////////////


#include "UpdateApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"


// APP CONSTRUCTOR.
// Create all objects here.
UpdateApp::UpdateApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
   	addChild( BG );
	
	
	A = new poRectShape(5, 5);
	A->fillColor.set(0.6, 0.8, 0.4);
	A->position.set(125, 300, 0);
	A->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild(A);
	
	isScalingA = true;
	
	
	B = new poOvalShape(30, 30, 30);
	B->fillColor.set(0.6, 0.8, 0.4);
	B->position.set(310, 300, 0);
	B->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild(B);
	
	velocityB = poPoint(0.5, 1, 0);
	
	
	C = new poRectShape(4, 70);
	C->position.set(490, 300, 0);
	C->setAlignment(PO_ALIGN_BOTTOM_CENTER);
	C->fillColor.set(0.6, 0.8, 0.4);
	addChild(C);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
UpdateApp::~UpdateApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void UpdateApp::update() {
	
	
	// A. Update scale manually ///////////////////////
	
	if(isScalingA)
		A->scale += poPoint(0.1, 0.1, 0);
	else
		A->scale -= poPoint(0.1, 0.1, 0);
	
	if(isScalingA && A->scale.x >= 30) isScalingA = false;
	if(!isScalingA && A->scale.x <= 0) isScalingA = true;
	
	
	// B. Update position manually ///////////////////////
	
	B->position += velocityB;
	
	if (B->position.x > 385 - 15 || B->position.x < 235 + 15) velocityB.x *= -1;
	if (B->position.y > 375 - 15 || B->position.y < 225 + 15) velocityB.y *= -1;
	
	
	// C. Update rotation manually ///////////////////////
	
	poTime currentTime = poGetCurrentTime();
	int currentSecond = currentTime.seconds;
	
	C->rotation = 360/60 * currentSecond;
}


// EVENT HANDLER
// Respond to user events here.
void UpdateApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void UpdateApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}

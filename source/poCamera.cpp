//
//  poCamera.cpp
//  poFramework4
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poCamera.h"
#include "poApplication.h"
#include "poMatrixStack.h"
#include <glm/gtc/matrix_transform.hpp>

// camera base class
poCamera::poCamera()
:	reset(true)
,	clears_background(true)
,	background_color(poColor::black)
,	is_fixed_size(false)
{}

poCamera::poCamera(poColor color) 
:	reset(true)
,	clears_background(true)
,	background_color(color)
,	is_fixed_size(false)
{}

void poCamera::doSetUp( poObject* obj ) {
	saveAndUpdateGLSettings();

	poMatrixStack *stack = poMatrixStack::get();
	
	if(fixedSize())
		stack->pushViewport(poRect(poPoint(),fixed_size));
	else
		stack->pushViewport(poRect(poPoint(),getWindowDimensions()));

	if(clears_background) {
        glClearColor(background_color.R, background_color.G, background_color.B, background_color.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

	setProjection();
	setModelview();
}

void poCamera::doSetDown( poObject* obj ) {
	restoreGLSettings();
	
	poMatrixStack *stack = poMatrixStack::get();
	stack->popProjection();
	stack->popModelview();
	stack->popViewport();
}

void poCamera::setProjection() {
	poMatrixStack::get()->pushProjection(glm::mat4());
}

void poCamera::setModelview() {
	poMatrixStack::get()->pushModelview(glm::mat4());
}

void poCamera::saveAndUpdateGLSettings() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void poCamera::restoreGLSettings() {
}

bool poCamera::clearsBackground() const {return clears_background;}
poCamera* poCamera::clearsBackground(bool b) {clears_background = b; return this;}

poColor poCamera::backgroundColor() const {return background_color;}
poCamera* poCamera::backgroundColor(poColor color) {background_color = color; return this;}
poCamera* poCamera::backgroundColor(float r, float g, float b, float a) {background_color.set(r,g,b,a); return this;}

bool poCamera::resetsModelview() const {return reset;}
poCamera* poCamera::resetsModelview(bool b) {reset = b; return this;}

bool poCamera::fixedSize() const {return is_fixed_size;}
poCamera* poCamera::fixedSize(bool b, poPoint p) {is_fixed_size=b; fixed_size=p; return this;}


// window camera
poCamera2D::poCamera2D() {}

poCamera2D::poCamera2D(poColor clear)
:	poCamera(clear)
{}

void poCamera2D::setProjection() {
	poMatrixStack *stack = poMatrixStack::get();
	poRect viewp = stack->getViewport();
	stack->pushProjection(glm::ortho(viewp.x, viewp.width + viewp.x, viewp.height + viewp.y, viewp.y));
    
    std::cout << viewp << std::endl;
}

// orthographic camera
poOrthoCamera::poOrthoCamera()
:	x1(-1)
,	y1(-1)
,	x2(1)
,	y2(1)
,	near(-1)
,	far(1)
{}

poOrthoCamera::poOrthoCamera(float w, float h, float n, float f)
:	x1(0)
,	y1(0)
,	x2(w)
,	y2(h)
,	near(n)
,	far(f)
{}

poOrthoCamera::poOrthoCamera(float x1, float y1, float x2, float y2, float n, float f)
:	x1(x1)
,	y1(y1)
,	x2(x2)
,	y2(y2)
,	near(n)
,	far(f)
{}

void poOrthoCamera::set(poRect r, float n, float f) {
	set(r.x, r.y, r.x + r.width, r.y + r.height, n, f);
}

void poOrthoCamera::set(float x1, float y1, float x2, float y2, float n, float f) {
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	near = n;
	far = f;
}

poRect poOrthoCamera::get() const {
	return poRect(x1, y1, x2-x1, y2-y1);
}

void poOrthoCamera::setProjection() {
	poMatrixStack::get()->pushProjection(glm::ortho(x1,x2,y2,y1,near,far));
}



poPerspectiveCamera::poPerspectiveCamera(float fov, float near, float far)
:	fov(fov)
,	near(near)
,	far(far)
{}

poPoint poPerspectiveCamera::lookAtPosition() const {return look_at_pos;}
poPerspectiveCamera *poPerspectiveCamera::lookAtPosition(poPoint p) {look_at_pos = p; return this;}
	
poPoint                 poPerspectiveCamera::cameraPosition() const  {return camera_pos;}
poPerspectiveCamera*    poPerspectiveCamera::cameraPosition(poPoint p) {camera_pos = p; return this;}

void poPerspectiveCamera::doSetUp(poObject *obj) {
	poCamera::doSetUp(obj);
}

void poPerspectiveCamera::setProjection() {
	poMatrixStack *stack = poMatrixStack::get();
    float aspect = getWindowAspect();
	stack->pushProjection(glm::perspective(fov, aspect, near, far));
}

void poPerspectiveCamera::setModelview() {
	using namespace glm;
	
	poMatrixStack *stack = poMatrixStack::get();
	
	vec3 eye(camera_pos.x, camera_pos.y, camera_pos.z);
	vec3 center(look_at_pos.x, look_at_pos.y, look_at_pos.z);
	vec3 up(0,1,0);
	stack->pushModelview(glm::lookAt(eye,center,up));
	stack->scale(poPoint(-1,-1,1));
}





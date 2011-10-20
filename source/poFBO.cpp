//
//  poFBO.cpp
//  potionCode
//
//  Created by Joshua Fisher on 10/17/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include "poFBO.h"
#include "poOpenGLState.h"

#ifdef OPENGL_ES
	#ifdef POTION_IOS
	#define GL_READ_FRAMEBUFFER GL_READ_FRAMEBUFFER_APPLE
	#define GL_DRAW_FRAMEBUFFER GL_DRAW_FRAMEBUFFER_APPLE
	#endif
#endif

poFBOConfig::poFBOConfig()
:	numMultisamples(0)
{}

poFBOConfig &poFBOConfig::setNumMultisamples(uint nm) {
	numMultisamples = nm;
	return *this;
}

poFBO::poFBO(uint w, uint h) 
:	width(w)
,	height(h)
,	cam(new poCamera2D())
,	multisampling(false)
{
	cam->fixedSize(true, poPoint(w,h));
	setup();
}

poFBO::poFBO(uint w, uint h, const poFBOConfig &c)
:	width(w)
,	height(h)
,	config(c)
,	cam(new poCamera2D())
,	multisampling(false)
{
	cam->fixedSize(true, poPoint(w,h));
	setup();
}

poFBO::~poFBO() {
	if(isValid())
		cleanup();
}

bool poFBO::isValid() const {
	return !framebuffers.empty() && framebuffers[0] > 0;
}

void poFBO::setCamera(poCamera *camera) {
	delete cam;
	cam = camera->copy();
}

void poFBO::reset(uint w, uint h, const poFBOConfig &c) {
	if(isValid())
		cleanup();
	
	width = w;
	height = h;
	config = c;
	setup();
}

// retrieve this texture to draw the FBO
poTexture poFBO::colorTexture() const {
	return colorTex;
}

poTexture poFBO::depthTexture() const {
	
}

void poFBO::doSetUp(poObject* obj) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
	cam->setUp(obj);
}

void poFBO::doSetDown(poObject* obj) {
	cam->setDown(obj);
#ifndef OPENGL_ES
	if(multisampling) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffers[0]);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffers[1]);
		#ifdef POTION_IOS
			glResolveMultisampleFramebufferAPPLE();
		#else
			glBlitFramebuffer(0,0,width,height, 0,0,width,height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		#endif
	}
#endif
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void poFBO::setup() {
	int maxSamples =  poOpenGLState::get()->maxFBOSamples();
	if(config.numMultisamples && maxSamples) {
		if(config.numMultisamples > maxSamples)
			config.numMultisamples = maxSamples;
		
		multisampling = true;

		renderbuffers.resize(1);
		glGenRenderbuffers(1, &renderbuffers[0]);
		
		// this is the multisample render buffer
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[0]);
#ifdef POTION_IOS
		glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, config.numMultisamples, GL_RGBA8_OES, width, height);
#else
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, config.numMultisamples, GL_RGBA8, width, height);
#endif
		// we need 2 different framebuffers
		framebuffers.resize(2);
		glGenFramebuffers(2, &framebuffers[0]);
		// the first is the multisample buffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffers[0]);

		// make the texture
		colorTex = poTexture(width,height,NULL,poTextureConfig(GL_RGBA));
		// and attach it to the second framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[1]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex.getUid(), 0);
		
		GLenum ok = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
		if(config.numMultisamples)
			printf("unable to do framebuffer multisampling\n");
		
		// make the texture, which is also the color render buffer
		colorTex = poTexture(width,height,NULL,poTextureConfig(GL_RGBA));

		// we only need the one framebuffer
		framebuffers.resize(1);
		glGenFramebuffers(1, &framebuffers[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex.getUid(), 0);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void poFBO::cleanup() {
	// make sure none of this is bound right now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(framebuffers.size(), &framebuffers[0]);
	glDeleteRenderbuffers(renderbuffers.size(), &renderbuffers[0]);
	framebuffers[0] = 0;
}





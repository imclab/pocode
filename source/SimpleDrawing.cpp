#include "SimpleDrawing.h"

#include "Helpers.h"
#include "poTexture.h"
#include "poBitmapFont.h"
#include "poOpenGLState.h"
#include "poMatrixStack.h"

#include <cfloat>
#include <utf8.h>
#include <boost/assign/list_of.hpp>

static poColor theColor(1.f, 1.f, 1.f, 1.f);

void applyColor(poColor color) {
	theColor = color;
}

void applyColor(poColor c, float a) {
	theColor = poColor(c,a);
}

poColor currentColor() {
    return theColor;
}

void drawQuad(GLenum type, float x, float y, float w, float h) {
	x = floor(x) + .5f;
	y = floor(y) + .5f;
	w = floor(w);
	h = floor(h);
	
	GLfloat quad[4*3] = {
		x, y, 0, 
		x, y+h, 0, 
		x+w, y, 0, 
		x+w, y+h, 0 
	};
	
	if(type == GL_LINE_STRIP || type == GL_LINE_LOOP)
		std::swap(quad[7], quad[10]);

	poOpenGLState::get()->bindShader(basicProgram1());
	basicProgram1()->uniform4v("color", &theColor.R, 1);
	basicProgram1()->uniformMat4("mvp", glm::value_ptr(poMatrixStack::get()->transformation()));
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
	glDrawArrays(type, 0, 4);
	
	poOpenGLState::get()->unbindShader();
}

void drawStroke(poRect rect) {
	drawStroke(rect.x, rect.y, rect.width, rect.height);
}

void drawStroke(float x, float y, float w, float h) {
	drawQuad(GL_LINE_LOOP,x,y,w,h);
}

void drawLine(poPoint a, poPoint b) {
	a = floor(a) + poPoint(0.5f, 0.5f);
	b = floor(b) + poPoint(0.5f, 0.5f);
	
	GLfloat points[2*3] = {
		a.x, a.y, a.z, 
		b.x, b.y, b.z
	};
	
	poOpenGLState::get()->bindShader(basicProgram1());
	basicProgram1()->uniform4v("color", &theColor.R, 1);
	basicProgram1()->uniformMat4("mvp", glm::value_ptr(poMatrixStack::get()->transformation()));
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, points);
	glDrawArrays(GL_LINES, 0, 2);
	
	poOpenGLState::get()->unbindShader();
}

void drawRect(poRect rect) {
	drawRect(rect.x, rect.y, rect.width, rect.height);
}

void drawRect(float x, float y, float w, float h) {
	drawQuad(GL_TRIANGLE_STRIP, x,y,w,h); 
}

void drawRect(poRect rect, poTexture* texture, bool flip) {
	drawRect(rect, poRect(0,0,1,1), texture, flip);
}

void drawRect(float x, float y, float w, float h, poTexture* texture, bool flip) {
	drawRect(poRect(x,y,w,h), texture, flip);
}

void drawRect(poRect rect, poTexture *tex, poTextureFitOption fit) {
	std::vector<poPoint> coords = textureFit(rect, tex, fit, PO_ALIGN_TOP_LEFT);
	poRect coords_rect(coords[0], coords[2]);
	drawRect(rect, coords_rect, tex);
}

void drawRect(poRect rect, poRect coords, poTexture *texture, bool flip) {
    rect.setPosition(floor(rect.getPosition()) + poPoint(.5f, .5f));
	
	GLfloat quad[4*3] = { 
		rect.x,  rect.y, 0, 
		rect.x,  rect.y+rect.height, 0, 
		rect.x+rect.width, rect.y, 0, 
		rect.x+rect.width, rect.y+rect.height, 0 
	};
	
	GLfloat tcoords[4*2] = {
		coords.x, coords.y,
		coords.x, coords.y+coords.height,
		coords.x+coords.width, coords.y,
		coords.x+coords.width, coords.y+coords.height
	};
	
	if(flip) {
//		int row_sz = 2*sizeof(GLfloat);
//		
//		GLfloat tmp[2];
//		memcpy(tmp, tcoords+row_sz, row_sz);
//		memcpy(tcoords+row_sz, tcoords+row_sz*2, row_sz);
//		memcpy(tcoords+row_sz*2, tcoords+row_sz*3, row_sz);
//		memcpy(tcoords+row_sz*3, tmp, row_sz);
	}
	
	poOpenGLState::get()->bindTexture(texture);
	poOpenGLState::get()->bindShader(basicProgram2());
	basicProgram1()->uniform4v("color", &theColor.R, 1);
	basicProgram2()->uniformMat4("mvp", glm::value_ptr(poMatrixStack::get()->transformation()));
	basicProgram2()->uniform1("tex", (int)texture->uid);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, tcoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	poOpenGLState::get()->unbindTexture();
}

void drawRect(poTexture* tex, bool flip) {
	drawRect(0,0,tex->width(),tex->height(),tex, flip);
}

void drawPoints(GLenum type, const std::vector<poPoint> &points) {
	poOpenGLState::get()->bindShader(basicProgram1());
	basicProgram1()->uniform4v("color", &theColor.R, 1);
	basicProgram1()->uniformMat4("mvp", glm::value_ptr(poMatrixStack::get()->transformation()));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &points[0]);
	glDrawArrays(type, 0, points.size());
	poOpenGLState::get()->unbindShader();
}

void textureFitExact(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.x / (float)rect.width;
	float yoff = rect.y / (float)rect.height;
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width - xoff;
		float t = points[i].y / rect.height - yoff;
		coords[i].set(s,t,0.f);
	}
}


void textureFitNone(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.x / (float)rect.width;
	float yoff = rect.y / (float)rect.height;
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / tex->width() - xoff;
		float t = points[i].y / tex->height() - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i] -= offset;
	}
}


void textureFitHorizontal(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.x / (float)rect.width;
	float yoff = rect.y / (float)rect.height;
	
	float new_w = rect.width;
	float new_h = new_w / (tex->width() / (float)tex->height());
	
	poPoint max(FLT_MIN, FLT_MIN);
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width - xoff;
		float t = points[i].y / new_h - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}
	
	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i] -= offset;
	}
}

void textureFitVertical(poRect rect, poTexture *tex, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float xoff = rect.x / (float)rect.width;
	float yoff = rect.y / (float)rect.height;
	
	float new_h = rect.height;
	float new_w = new_h / (tex->height() / (float)tex->width());
	
	poPoint max(FLT_MIN, FLT_MIN);

	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / new_w - xoff;
		float t = points[i].y / rect.height - yoff;
		
		max.x = std::max(s, max.x);
		max.y = std::max(t, max.y);
		
		coords[i].set(s,t,0.f);
	}

	poPoint offset = alignInRect(max, poRect(0,0,1,1), align);
	
	for(int i=0; i<coords.size(); i++) {
		coords[i] -= offset;
	}
}

std::vector<poPoint> textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align) {
	std::vector<poPoint> coords(4);
	std::vector<poPoint> points = rect.getCorners();
	textureFit(rect, tex, fit, align, coords, points);
	return coords;
}

void textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	switch(fit) {
		case PO_TEX_FIT_NONE:
			textureFitNone(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_EXACT:
			textureFitExact(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_H:
			textureFitHorizontal(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_V:
			textureFitVertical(rect, tex, align, coords, points);
			break;
			
		case PO_TEX_FIT_INSIDE: 
		{
			float new_h = rect.width * (tex->height() / (float)tex->width());
			if(new_h > rect.height)
				textureFitVertical(rect, tex, align, coords, points);
			else
				textureFitHorizontal(rect, tex, align, coords, points);
			break;
		}
		default:
			;
	}
}

void drawString(const std::string &str, poFont *font, poPoint pos, int ptSize, float tracking) {
	if(ptSize > 0)
		font->pointSize(ptSize);
	
	poBitmapFont *bitmapFont = getBitmapFont(font);

	font->glyph(' ');
	float spacer = font->glyphAdvance().x * tracking;

	bitmapFont->setUpFont();
	
	std::string::const_iterator ch = str.begin();
	while(ch != str.end()) {
		uint codepoint = utf8::next(ch, str.end());
		font->glyph(codepoint);
		
		poPoint adv = font->glyphAdvance();
		poPoint org = round(pos+font->glyphBearing());
		
		bitmapFont->drawGlyph( codepoint, pos+font->glyphBearing() );
		
		pos.x += adv.x * tracking;
	}
	
	bitmapFont->setDownFont();
}



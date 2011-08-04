//
//  poFont.h
//  poFramework4
//
//  Created by Joshua Fisher on 3/23/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poRect.h"
#include "poImage.h"
#include "poTexture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class poShape2D;
class poFontLoader;
class poBitmapFontAtlas;

// glyphs each have some padding built into the image
// it has to be subtracted from the glyph origin before drawing
const static int GLYPH_PADDING = 5;

class poFont;
typedef std::map<std::string, poFont*> poFontMap;

class poFont
{
	friend class poFontLoader;
	
public:
	virtual ~poFont();
	
	bool isValid() const;
	
	std::string familyName() const;
	std::string styleName() const;
	std::string url() const;
	bool hasKerning() const;
	
	int pointSize() const;
	void pointSize(int size);

	float lineHeight() const;
	float ascender() const;
	float descender() const;
	float underlinePosition() const;
	float underlineThickness() const;

	int glyph() const;
	void glyph(int g);
	// uses the previously set glyph
	poRect glyphBounds();
	poPoint glyphBearing();
	poPoint glyphAdvance();
	// you have to delete the results
	poImage *glyphImage();
	poShape2D *glyphOutline();

	poPoint kernGlyphs(int glyph1, int glyph2);
	
	std::string toString() const;

private:
	poFont();
	poFont(const std::string &family_or_url, int pointSize, const std::string &style);

	void init();
	void loadGlyph(int g);
	
	static FT_Library lib;
	boost::shared_ptr<FT_FaceRec_> face;

	std::string _url;
	int size, _glyph;
};

bool fontExists(const std::string &family_or_url);

std::ostream &operator<<(std::ostream &o, const poFont &f);


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
class poBitmapFont;

class poFont;
typedef std::map<std::string, poFont*> poFontMap;

// CLASS NOTES
//
// A poFont implements general font functionality. A single poFont object represents a single
// font face in a single font size.
//
// You cannot construct a new font directly. You can load a new font as follows:
//
//      poFont* font = getFont("Courier", 20);
//
// Using getFont() ensures that the same font is not loaded and constructed multiple times.
//
// 

class poFont
{
	friend class        poFontLoader;
	
public:
	virtual             ~poFont();
	
    // FONT LOADING
	bool                isValid() const;
	
    // FONT PROPERTIES
	std::string         familyName() const;
	std::string         styleName() const;
	std::string         url() const;
	bool                hasKerning() const;
	
	int                 pointSize() const;
	void                pointSize(int size);

	float               lineHeight() const;
	float               ascender() const;
	float               descender() const;
    
    // UNDERLINE
	// maximum bbox for this font face at this size
	float               underlinePosition() const;
	float               underlineThickness() const;

    // CURRENT GLYPH
    // Get and set the current glyph.
	int                 glyph() const;
	void                glyph(int g);
    
	// These functions (starting with 'glyph') return info about the current codepoint.
	poRect              glyphBounds() const;
	poRect              glyphFrame() const;
	float               glyphDescender() const;
	poPoint             glyphBearing() const;
	poPoint             glyphAdvance() const;
	poImage             *glyphImage() const;
	poShape2D           *glyphOutline() const;

	poPoint             kernGlyphs(int glyph1, int glyph2) const;

	std::string         toString() const;

private:
	poFont();
	poFont(const std::string &family_or_url, int pointSize, const std::string &style);

	void                init();
	void                loadGlyph(int g);
	
	boost::shared_ptr<FT_FaceRec_> face;

	std::string         _url;
	int size,           _glyph;
    
    static FT_Library   lib;
};

// glyphs each have some padding built into the image
// it has to be subtracted from the glyph origin before drawing
const static int GLYPH_PADDING = 5;

bool fontExists(const std::string &family_or_url);

std::ostream &operator<<(std::ostream &o, const poFont &f);

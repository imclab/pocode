//
//  poCamera.h
//  poFramework4
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poObject.h"
#include "poColor.h"


class poCamera2D : public poObjectModifier
{
public:
    poCamera2D();
    
    virtual void setUp( poObject* obj );
    virtual void setDown( poObject* obj );
    
    bool       clearBackground;
    poColor    backgroundColor;
};
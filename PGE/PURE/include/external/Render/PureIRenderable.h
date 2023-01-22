#pragma once

/*
    ###################################################################################
    PureIRenderable.h
    This file is part of PURE.
    External header.
    Renderable object interface.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include "PureIRenderer.h"


/**
    Renderable object interface.

*/

class PureIRenderable
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureIRenderable is included")
#endif

public:

    /**
        Render this renderable object using the specified renderer.

        @param renderer  Renderer to be used for rendering.
        @param bLighting Should lighting be applied on the renderable object or not.
    */
    virtual void Draw(PureIRenderer& renderer, bool bLighting) = 0;

};
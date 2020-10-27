#pragma once

/*
    ###################################################################################
    PRREIRenderable.h
    This file is part of PRRE.
    External header.
    Renderable object interface.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "../PRREtypes.h"
#include "PRREIRenderer.h"


/**
    Renderable object interface.

*/

class PRREIRenderable
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREIRenderable is included")
#endif

public:

    /**
        Render this renderable object using the specified renderer.

        @param renderer  Renderer to be used for rendering.
        @param bLighting Should lighting be applied on the renderable object or not.
    */
    virtual void Draw(PRREIRenderer& renderer, bool bLighting) = 0;

};
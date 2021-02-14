#pragma once

/*
    ###################################################################################
    PGEWorld.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine World class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"
#include "PGEWorldTime.h"
#include "PGEWorldWeather.h"


/**
    PR00F's Game Engine World class.
*/
class PGEWorld
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEWorld is included")
#endif

public:

    /** Creates and gets the singleton instance. */
    static PGEWorld& createAndGet();

    // ---------------------------------------------------------------------------

    /**
        Initializes the world.
    */
    virtual bool initialize() = 0;
    virtual void Shutdown() = 0;

    virtual PGEWorldTime& getWorldTime() const = 0;
    virtual PGEWorldWeather& getWorldWeather() const = 0;

};


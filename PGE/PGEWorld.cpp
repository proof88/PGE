#pragma once

/*
    ###################################################################################
    PGEWorld.cpp
    This file is part of PGE.
    PR00F's Game Engine World class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGEWorld.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"


/*
   PGEWorldImpl
   ###########################################################################
*/


class PGEWorldImpl :
    public PGEWorld
{

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PGEWorldImpl();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    bool initialize();
    void Shutdown();

    PGEWorldTime& getWorldTime() const;
    PGEWorldWeather& getWorldWeather() const;

protected:

    // ---------------------------------------------------------------------------

private:

    PGEWorldTime& worldTime;
    PGEWorldWeather& worldWeather;

    // ---------------------------------------------------------------------------

    PGEWorldImpl();

    PGEWorldImpl(const PGEWorldImpl&);   
    PGEWorldImpl& operator=(const PGEWorldImpl&);

    friend class PGEWorld;

};


// ############################### PUBLIC ################################


PGEWorldImpl::~PGEWorldImpl()
{

}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEWorldImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEWorldImpl::getLoggerModuleName()
{
    return "PGEWorld";
} // getLoggerModuleName()


/**
    Initializes the world.
*/
bool PGEWorldImpl::initialize()
{
    getConsole().OLnOI("PGEWorld::initialize()");
    if ( !worldTime.initialize() )
    {
        getConsole().OIEOLnOO("failed to initialize WorldTime!");
        getConsole().OO();
        Shutdown();
        return false;
    }

    if ( !worldWeather.initialize(10, 10, 10) )
    {
        getConsole().OIEOLnOO("failed to initialize WorldWeather!");
        getConsole().OO();
        Shutdown();
        return false;
    }
        
    getConsole().SOLnOO("> World initialized!");
    getConsole().OLn("");
    return true;
}


void PGEWorldImpl::Shutdown()
{
    getConsole().OLnOI("PGEWorld::Shutdown()");
    worldWeather.Shutdown();
    worldTime.Shutdown();
    getConsole().OO();
}


PGEWorldTime& PGEWorldImpl::getWorldTime() const
{
    return worldTime;
}

PGEWorldWeather& PGEWorldImpl::getWorldWeather() const
{
    return worldWeather;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGEWorldImpl::PGEWorldImpl() :
    worldTime( PGEWorldTime::createAndGet() ),
    worldWeather( PGEWorldWeather::createAndGet() )
{

}

PGEWorldImpl::PGEWorldImpl(const PGEWorldImpl&) :
    worldTime( PGEWorldTime::createAndGet() ),
    worldWeather( PGEWorldWeather::createAndGet() )
{

}     

PGEWorldImpl& PGEWorldImpl::operator=(const PGEWorldImpl&)
{
    return *this;
}




/*
   PGEWorld
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PGEWorld& PGEWorld::createAndGet()
{
    static PGEWorldImpl worldInstance;
    return worldInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
CConsole& PGEWorld::getConsole() const
{
    return createAndGet().getConsole();
    #pragma warning(default:4717)
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
const char* PGEWorld::getLoggerModuleName()
{
    return createAndGet().getLoggerModuleName();
    #pragma warning(default:4717)
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################



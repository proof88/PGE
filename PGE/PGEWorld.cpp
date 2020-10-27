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

    virtual ~PGEWorldImpl();

    bool initialize();
    void Shutdown();

    PGEWorldTime& getWorldTime() const;
    PGEWorldWeather& getWorldWeather() const;

protected:

    // ---------------------------------------------------------------------------

private:

    CConsole& con;
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
    Initializes the world.
*/
bool PGEWorldImpl::initialize()
{
    con.OLnOI("PGEWorld::initialize()");
    if ( !worldTime.initialize() )
    {
        con.OIEOLnOO("failed to initialize WorldTime!");
        con.OO();
        Shutdown();
        return false;
    }

    if ( !worldWeather.initialize(10, 10, 10) )
    {
        con.OIEOLnOO("failed to initialize WorldWeather!");
        con.OO();
        Shutdown();
        return false;
    }
        
    con.SOLnOO("> World initialized!");
    con.OLn("");
    return true;
}


void PGEWorldImpl::Shutdown()
{
    con.OLnOI("PGEWorld::Shutdown()");
    worldWeather.Shutdown();
    worldTime.Shutdown();
    con.OO();
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
    con( CConsole::getConsoleInstance() ),
    worldTime( PGEWorldTime::createAndGet() ),
    worldWeather( PGEWorldWeather::createAndGet() )
{

}

PGEWorldImpl::PGEWorldImpl(const PGEWorldImpl&) :
    con( CConsole::getConsoleInstance() ),
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


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################



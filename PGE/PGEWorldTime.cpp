/*
    ###################################################################################
    PGEWorldTime.cpp
    This file is part of PGE.
    PR00F's Game Engine World Time class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGEWorldTime.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"
#include "../../CConsole/CConsole/src/CConsole.h"

using namespace std;



/*
   PGEWorldTimeImpl
   ###########################################################################
*/


class PGEWorldTimeImpl :
    public PGEWorldTime
{

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PGEWorldTimeImpl();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    bool initialize(int days = 0, int hours = 0, int mins = 0, int secs = 0);   
    bool isInitialized() const;  
    void Shutdown();

    int  getDays() const;     
    int  getTime() const;      
    int  getHours() const;    
    int  getMins() const;    
    int  getSecs() const;      
    int  getMilliSecs() const;  

    void SetTimeAbsolute(
        int days, int hours = 0, int mins = 0, int secs = 0, int millisecs = 0); 
    void SetTimeRelative(
        int hours, int mins = 0, int secs = 0, int millisecs = 0); 

    void IncDays();     
    void DecDays();     
    void IncHours();    
    void DecHours();    
    void IncMins();     
    void DecMins();     
    void IncSecs();    
    void DecSecs();   
    void IncMilliSecs(); 
    void DecMilliSecs(); 
    
    int  getRealMillisecondsPerVirtualSecond() const;             
    void SetRealMillisecondsPerVirtualSecond(int realMillisecs);  
    void AdvanceByRealMilliseconds(int millisecs);                

protected:

    // ---------------------------------------------------------------------------

private:

    static const int PGE_WORLD_TIME_DEF_REAL_MSECS_PER_VIRTUAL_SEC;   /**< How many real milliseconds equal to 1 virtual second by default. */

    static const int PGE_WORLD_TIME_MILLISECS_PER_SEC;   /**< How many virtual milliseconds equal to 1 virtual second by default. */
    static const int PGE_WORLD_TIME_SECS_PER_MIN;        /**< How many virtual seconds equal to 1 virtual minute by default. */
    static const int PGE_WORLD_TIME_MINS_PER_HOUR;       /**< How many virtual minutes equal to 1 virtual hour by default. */
    static const int PGE_WORLD_TIME_HOURS_PER_DAY;       /**< How many virtual hours equal to 1 virtual day by default. */

    // ---------------------------------------------------------------------------

    bool bInitialized;

    int nCurrentDayMilliSecs;                /**< Current time of the day in millisecs. */
    int nCurrentDay;                         /**< Current day. */
    int nRealMillisecsPerVirtualSec;         /**< How many real milliseconds equal to 1 virtual second. */

    // ---------------------------------------------------------------------------

    PGEWorldTimeImpl();

    PGEWorldTimeImpl(const PGEWorldTimeImpl&);    
    PGEWorldTimeImpl& operator=(const PGEWorldTimeImpl&);

    bool checkValues(
        int days, int hours = 0, int mins = 0, int secs = 0, int millisecs = 0);  /**< Checks if the given values are correct. */


    friend class PGEWorldTime;

};


// ############################### PUBLIC ################################


PGEWorldTimeImpl::~PGEWorldTimeImpl()
{
    Shutdown();
}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEWorldTimeImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEWorldTimeImpl::getLoggerModuleName()
{
    return "PGEWorldTime";
} // getLoggerModuleName()


/**
    Initializes the virtual time.
*/
bool PGEWorldTimeImpl::initialize(int days, int hours, int mins, int secs)
{
    getConsole().OLn("PGEWorldTime::initialize(%d, %d, %d, %d)", days, hours, mins, secs);
    SetTimeAbsolute(days, hours, mins, secs);
    bInitialized = true;
    return true;
}


bool PGEWorldTimeImpl::isInitialized() const
{
    return bInitialized;
}


void PGEWorldTimeImpl::Shutdown()
{
    getConsole().OLn("PGEWorldTime::Shutdown()");
    bInitialized = false;
    SetTimeAbsolute(0, 0, 0, 0);
}


/**
    Gets the current virtual day.
    Example: for current time of day 4, 05:35:22, it will return 4.
*/
int  PGEWorldTimeImpl::getDays() const
{
    return nCurrentDay;
}


/**
    Gets the time of the current virtual day in millisecs.
    Example: for current time of day 4, 05:35:22.335, it will return 20122335 (22.335*1000 + 35*60*1000 + 5*3600*1000).
*/
int  PGEWorldTimeImpl::getTime() const
{
    return nCurrentDayMilliSecs;
}


/**
    Gets the hour of the current virtual day.
    Example: for current time of day 4, 05:35:22.335, it will return 5.
*/
int  PGEWorldTimeImpl::getHours() const
{
    return (int) floor( nCurrentDayMilliSecs / (float)PGE_WORLD_TIME_MILLISECS_PER_SEC / PGE_WORLD_TIME_SECS_PER_MIN / PGE_WORLD_TIME_MINS_PER_HOUR );
}


/**
    Gets the minutes of the current virtual hour.
    Example: for current time of day 4, 05:35:22.335, it will return 35.
*/
int  PGEWorldTimeImpl::getMins() const
{
    return ((int) floor( nCurrentDayMilliSecs / (float)PGE_WORLD_TIME_MILLISECS_PER_SEC / PGE_WORLD_TIME_SECS_PER_MIN )) % PGE_WORLD_TIME_MINS_PER_HOUR;
}


/**
    Gets the seconds of the current virtual minute.
    Example: for current time of day 4, 05:35:22.335, it will return 22.
*/
int  PGEWorldTimeImpl::getSecs() const
{
    return ((int) floor( nCurrentDayMilliSecs / (float)PGE_WORLD_TIME_MILLISECS_PER_SEC)) % PGE_WORLD_TIME_SECS_PER_MIN;
}


/**
    Gets the milliseconds of the current virtual second.
    Example: for current time of day 4, 05:35:22.335, it will return 335.
*/
int  PGEWorldTimeImpl::getMilliSecs() const
{
    return nCurrentDayMilliSecs % PGE_WORLD_TIME_MILLISECS_PER_SEC;
}


/**
    Sets the virtual day and time.
*/
void PGEWorldTimeImpl::SetTimeAbsolute(int days, int hours, int mins, int secs, int millisecs)
{
    if ( !checkValues(days, hours, mins, secs, millisecs) )
        return;

    nCurrentDay = days;
    SetTimeRelative(hours, mins, secs, millisecs);
}


/**
    Sets the virtual time within the current virtual day.
*/
void PGEWorldTimeImpl::SetTimeRelative(int hours, int mins, int secs, int millisecs)
{
    if ( !checkValues(getDays(), hours, mins, secs, millisecs) )
        return;

    nCurrentDayMilliSecs =
        millisecs +
        secs * PGE_WORLD_TIME_MILLISECS_PER_SEC +
        mins * PGE_WORLD_TIME_SECS_PER_MIN * PGE_WORLD_TIME_MILLISECS_PER_SEC +
        hours * PGE_WORLD_TIME_SECS_PER_MIN * PGE_WORLD_TIME_MINS_PER_HOUR * PGE_WORLD_TIME_MILLISECS_PER_SEC;
}


/**
    Increments the current virtual day by 1.
*/
void PGEWorldTimeImpl::IncDays()
{
    nCurrentDay++;
}


/**
    Decrements the current virtual day by 1.
*/
void PGEWorldTimeImpl::DecDays()
{
    if ( nCurrentDay > 0 )
        nCurrentDay--;
}


/**
    Increments the current virtual hour by 1.
*/
void PGEWorldTimeImpl::IncHours()
{
    if ( checkValues(getDays(), getHours()+1) )
        nCurrentDayMilliSecs += PGE_WORLD_TIME_SECS_PER_MIN * PGE_WORLD_TIME_MINS_PER_HOUR * PGE_WORLD_TIME_MILLISECS_PER_SEC;
    else // wrap
        SetTimeAbsolute( getDays()+1, 0, getMins(), getSecs(), getMilliSecs() );
}


/**
    Decrements the current virtual hour by 1.
*/
void PGEWorldTimeImpl::DecHours()
{
    if ( checkValues(getDays(), getHours()-1) )
        nCurrentDayMilliSecs -= PGE_WORLD_TIME_SECS_PER_MIN * PGE_WORLD_TIME_MINS_PER_HOUR * PGE_WORLD_TIME_MILLISECS_PER_SEC;
    else  // wrap
        if ( getDays() > 0 )
            SetTimeAbsolute( getDays()-1, PGE_WORLD_TIME_HOURS_PER_DAY-1, getMins(), getSecs(), getMilliSecs() );
} 


/**
    Increments the current virtual minutes by 1.
*/
void PGEWorldTimeImpl::IncMins()
{
    nCurrentDayMilliSecs += PGE_WORLD_TIME_SECS_PER_MIN * PGE_WORLD_TIME_MILLISECS_PER_SEC;

    // check if hours should be wrapped
    if ( !checkValues(0, getHours()) )
        SetTimeAbsolute( getDays()+1, 0, getMins(), getSecs(), getMilliSecs() );
} 


/**
    Decrements the current virtual minutes by 1.
*/
void PGEWorldTimeImpl::DecMins()
{
    const int CHANGE = PGE_WORLD_TIME_SECS_PER_MIN * PGE_WORLD_TIME_MILLISECS_PER_SEC;

    if ( nCurrentDayMilliSecs < CHANGE )
        return;

    nCurrentDayMilliSecs -= CHANGE;

    // check if hours should be wrapped
    if ( !checkValues(0, getHours()) )
        if ( getDays() > 0 )
            SetTimeAbsolute( getDays()-1, PGE_WORLD_TIME_HOURS_PER_DAY-1, getMins(), getSecs(), getMilliSecs() );
} 


/**
    Increments the current virtual seconds by 1.
*/
void PGEWorldTimeImpl::IncSecs()
{
    nCurrentDayMilliSecs += PGE_WORLD_TIME_MILLISECS_PER_SEC;

    // check if hours should be wrapped
    if ( !checkValues(0, getHours()) )
        SetTimeAbsolute( getDays()+1, 0, getMins(), getSecs(), getMilliSecs() );
}  


/**
    Decrements the current virtual seconds by 1.
*/
void PGEWorldTimeImpl::DecSecs()
{
    if ( nCurrentDayMilliSecs < PGE_WORLD_TIME_MILLISECS_PER_SEC )
        return;

    nCurrentDayMilliSecs -= PGE_WORLD_TIME_MILLISECS_PER_SEC;

    // check if hours should be wrapped
    if ( !checkValues(0, getHours()) )
        if ( getDays() > 0 )
            SetTimeAbsolute( getDays()-1, PGE_WORLD_TIME_HOURS_PER_DAY-1, getMins(), getSecs(), getMilliSecs() );
}    


/**
    Increments the current virtual milliseconds by 1.
*/
void PGEWorldTimeImpl::IncMilliSecs()
{
    nCurrentDayMilliSecs += 1;

    // check if hours should be wrapped
    if ( !checkValues(0, getHours()) )
        SetTimeAbsolute( getDays()+1, 0, getMins(), getSecs(), getMilliSecs() );
}


/**
    Decrements the current virtual milliseconds by 1.
*/
void PGEWorldTimeImpl::DecMilliSecs()
{
    if ( nCurrentDayMilliSecs == 0 )
        return;

    nCurrentDayMilliSecs -= 1;

    // check if hours should be wrapped
    if ( !checkValues(0, getHours()) )
        if ( getDays() > 0 )
            SetTimeAbsolute( getDays()-1, PGE_WORLD_TIME_HOURS_PER_DAY-1, getMins(), getSecs(), getMilliSecs() );
}


/**
    Gets how many real milliseconds equal to 1 virtual second.
*/
int  PGEWorldTimeImpl::getRealMillisecondsPerVirtualSecond() const
{
    return nRealMillisecsPerVirtualSec;
}


/**
    Sets how many real milliseconds equal to 1 virtual second.
*/
void PGEWorldTimeImpl::SetRealMillisecondsPerVirtualSecond(int realMillisecs)
{
    nRealMillisecsPerVirtualSec = realMillisecs;
}


/**
    Advances virtual time by the given real milliseconds.
    It is recommended to continuously update the virtual time at least a few times per a real-world second.
    Examples:
    current virtual time is 05:35:22.335
        1.) RealMillisecsPerVirtualSec is 1000 (default)
            by AdvanceByRealMilliseconds( 1000 ),
                new virtual time will be: 05:35:23.335
        2.) RealMillisecsPerVirtualSec is 500 (2x virtual time acceleration)
            by AdvanceByRealMilliseconds( 1000 ),
                new virtual time will be: 05:35:24.335
        3.) RealMillisecsPerVirtualSec is 2000 (2x virtual time deceleration)
            by AdvanceByRealMilliseconds( 1000 ),
                new virtual time will be: 05:35:23.835
*/
void PGEWorldTimeImpl::AdvanceByRealMilliseconds(int millisecs)
{
    nCurrentDayMilliSecs += int( floor( (millisecs / (float)nRealMillisecsPerVirtualSec) * PGE_WORLD_TIME_MILLISECS_PER_SEC ) );

    // check if hours should be wrapped
    if ( !checkValues(0, getHours()) )
        SetTimeAbsolute( getDays()+1, 0, getMins(), getSecs(), getMilliSecs() );
}
 

// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


const int PGEWorldTimeImpl::PGE_WORLD_TIME_DEF_REAL_MSECS_PER_VIRTUAL_SEC = 1000;

const int PGEWorldTimeImpl::PGE_WORLD_TIME_MILLISECS_PER_SEC = 1000;
const int PGEWorldTimeImpl::PGE_WORLD_TIME_SECS_PER_MIN = 60;
const int PGEWorldTimeImpl::PGE_WORLD_TIME_MINS_PER_HOUR = 60;
const int PGEWorldTimeImpl::PGE_WORLD_TIME_HOURS_PER_DAY = 24;



PGEWorldTimeImpl::PGEWorldTimeImpl() :
    bInitialized( false ),
    nCurrentDayMilliSecs( 0 ),
    nCurrentDay( 0 ),
    nRealMillisecsPerVirtualSec( PGE_WORLD_TIME_DEF_REAL_MSECS_PER_VIRTUAL_SEC )
{

}

PGEWorldTimeImpl::PGEWorldTimeImpl(const PGEWorldTimeImpl&) :
    bInitialized( false ),
    nCurrentDayMilliSecs( 0 ),
    nCurrentDay( 0 ),
    nRealMillisecsPerVirtualSec( PGE_WORLD_TIME_DEF_REAL_MSECS_PER_VIRTUAL_SEC )
{

}     

PGEWorldTimeImpl& PGEWorldTimeImpl::operator=(const PGEWorldTimeImpl&)
{
    return *this;
}


/**
    Checks if the given values are correct.
*/
bool PGEWorldTimeImpl::checkValues(int days, int hours, int mins, int secs, int millisecs)
{
    if ( (days < 0) || (hours < 0) || (mins < 0) || (secs < 0) || (millisecs < 0) )
        return false;

    if ( hours >= PGE_WORLD_TIME_HOURS_PER_DAY )
        return false;

    if ( mins >= PGE_WORLD_TIME_MINS_PER_HOUR )
        return false;

    if ( secs >= PGE_WORLD_TIME_SECS_PER_MIN )
        return false;

    if ( millisecs >= PGE_WORLD_TIME_MILLISECS_PER_SEC )
        return false;

    return true;
}



/*
   PGEWorldTime
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PGEWorldTime& PGEWorldTime::createAndGet()
{
    static PGEWorldTimeImpl worldTimeInstance;
    return worldTimeInstance;
}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
CConsole& PGEWorldTime::getConsole() const
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
const char* PGEWorldTime::getLoggerModuleName()
{
    return createAndGet().getLoggerModuleName();
    #pragma warning(default:4717)
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################








/*
    ###################################################################################
    PGEWorldWeather.cpp
    This file is part of PGE.
    PR00F's Game Engine World Weather class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "PGEWorldWeather.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"
#include "PURE/include/external/Math/PureVector.h"

using namespace std;



/*

    weather in games:
    http://gamestudies.org/0801/articles/barton
    http://www.3dweather.org/?p=51
    http://www.3dweather.org/?p=133
    http://www.3dweather.org/?p=120
    http://www.3dweather.org/?p=107
    http://www.3dweather.org/?p=70
    http://kotaku.com/5853995/what-game-handled-weather-changes-the-best/all

*/



/*
   WorldWeatherCell::WorldWeatherCellImpl
   ###########################################################################
*/


class WorldWeatherCell::WorldWeatherCellImpl
{

public:

    // ---------------------------------------------------------------------------

    WorldWeatherCellImpl();

    virtual ~WorldWeatherCellImpl();

protected:

    // ---------------------------------------------------------------------------

    WorldWeatherCellImpl(const WorldWeatherCellImpl&);
    WorldWeatherCellImpl& operator=(const WorldWeatherCellImpl&);

private:

    static float nSize;   /**< Size of a cell in meters. */
    
    // ---------------------------------------------------------------------------

    float      nTemperature;    /**< Temperature in °C. */
    float      nWindSpeed;      /**< Wind speed in m/s. */
    PureVector vWindDir;        /**< Wind direction. */
    float      nPrecipitation;  /**< Precipitation amount in mm/h (~ equals to liter/m^2). */

    // ---------------------------------------------------------------------------

    friend class WorldWeatherCell;

};


// ############################### PUBLIC ################################


WorldWeatherCell::WorldWeatherCellImpl::WorldWeatherCellImpl()
{

}


WorldWeatherCell::WorldWeatherCellImpl::~WorldWeatherCellImpl()
{

}


// ############################## PROTECTED ##############################


WorldWeatherCell::WorldWeatherCellImpl::WorldWeatherCellImpl(const WorldWeatherCell::WorldWeatherCellImpl&)
{

}     


WorldWeatherCell::WorldWeatherCellImpl& WorldWeatherCell::WorldWeatherCellImpl::operator=(const WorldWeatherCell::WorldWeatherCellImpl&)
{
    return *this;
}


// ############################### PRIVATE ###############################


float WorldWeatherCell::WorldWeatherCellImpl::nSize = WorldWeatherCell::PGE_WORLD_WEATHER_DEF_CELL_SIZE;
 


/*
   WorldWeatherCell
   ###########################################################################
*/


// ############################### PUBLIC ################################


const float WorldWeatherCell::PGE_WORLD_WEATHER_DEF_CELL_SIZE = 1.0f;


/**
    Gets the cell size in meters.
    The default is 1.
*/
float WorldWeatherCell::getCellSize()
{
    return WorldWeatherCellImpl::nSize;
}

/**
    Sets the cell size in meters.

    @param size The desired cell size in meters.
*/
void WorldWeatherCell::SetCellSize(float size)
{
    WorldWeatherCellImpl::nSize = size;
}


WorldWeatherCell::WorldWeatherCell()
{
    pImpl = new WorldWeatherCellImpl();
}


WorldWeatherCell::~WorldWeatherCell()
{
    delete pImpl;
    pImpl = NULL;
}


// ############################## PROTECTED ##############################


WorldWeatherCell::WorldWeatherCell(const WorldWeatherCell&)
{
    pImpl = NULL;
};     


WorldWeatherCell& WorldWeatherCell::operator=(const WorldWeatherCell&)
{
    pImpl = NULL;
    return *this;
}


// ############################### PRIVATE ###############################




/*
   PGEWorldWeatherImpl
   ###########################################################################
*/


class PGEWorldWeatherImpl :
    public PGEWorldWeather
{

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PGEWorldWeatherImpl();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    bool initialize(
        int numCellsX, int numCellsY, int numCellsZ,
        int cellSize = WorldWeatherCell::PGE_WORLD_WEATHER_DEF_CELL_SIZE);
    bool isInitialized() const;  
    void Shutdown();

protected:

    // ---------------------------------------------------------------------------

private:

    bool bInitialized;
    WorldWeatherCell* cells;          /**< Cells array. */
    int nCellsX, nCellsY, nCellsZ;    /**< Number of cells horizontally, vertically, and over Z. */

    // ---------------------------------------------------------------------------

    PGEWorldWeatherImpl();

    PGEWorldWeatherImpl(const PGEWorldWeatherImpl&);   
    PGEWorldWeatherImpl& operator=(const PGEWorldWeatherImpl&);

    
    friend class PGEWorldWeather;

};


// ############################### PUBLIC ################################


PGEWorldWeatherImpl::~PGEWorldWeatherImpl()
{
    Shutdown();
}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEWorldWeatherImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEWorldWeatherImpl::getLoggerModuleName()
{
    return "PGEWorldWeather";
} // getLoggerModuleName()


/**
    Initializes the weather.
*/
bool PGEWorldWeatherImpl::initialize(int numCellsX, int numCellsY, int numCellsZ, int cellSize)   
{
    getConsole().OLnOI("PGEWorldWeather::initialize(%d, %d, %d, %d)", numCellsX, numCellsY, numCellsZ, cellSize);

    if ( bInitialized )
    {
        getConsole().SOLnOO("  > already initialized!");
        getConsole().OLn("");
        return true;
    }

    try
    {
        WorldWeatherCell::SetCellSize((float)cellSize);
        nCellsX = numCellsX;
        nCellsY = numCellsY;
        nCellsZ = numCellsZ;
        cells = new WorldWeatherCell[nCellsX * nCellsY * nCellsZ];
        bInitialized = true;
        getConsole().OLn("allocated %d Bytes for weather data", sizeof(WorldWeatherCell) * nCellsX * nCellsY * nCellsZ);
        getConsole().SOLnOO("> initialized!");
        getConsole().OLn("");
        return true;
    }
    catch (const std::bad_alloc&)
    {
        getConsole().EOLnOO("ERROR: memory allocation failure!");
        getConsole().OLn("");
        return false;
    }
}


bool PGEWorldWeatherImpl::isInitialized() const
{
    return bInitialized;
}


void PGEWorldWeatherImpl::Shutdown()
{
    getConsole().OLn("PGEWorldWeather::Shutdown()");
    bInitialized = false;
    delete[] cells;
    cells = NULL;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGEWorldWeatherImpl::PGEWorldWeatherImpl() :
    bInitialized( false ),
    cells( NULL ),
    nCellsX( 0 ), nCellsY( 0 ), nCellsZ( 0 )
{

};


PGEWorldWeatherImpl::PGEWorldWeatherImpl(const PGEWorldWeatherImpl&) :
    bInitialized( false ),
    cells( NULL ),
    nCellsX( 0 ), nCellsY( 0 ), nCellsZ( 0 )
{

};   


PGEWorldWeatherImpl& PGEWorldWeatherImpl::operator=(const PGEWorldWeatherImpl&)
{
    return *this;
}



/*
   PGEWorldWeather
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PGEWorldWeather& PGEWorldWeather::createAndGet()
{
    static PGEWorldWeatherImpl worldWeatherInstance;
    return worldWeatherInstance;
}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
CConsole& PGEWorldWeather::getConsole() const
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
const char* PGEWorldWeather::getLoggerModuleName()
{
    return createAndGet().getLoggerModuleName();
    #pragma warning(default:4717)
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################



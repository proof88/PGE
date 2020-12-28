/*
    ###################################################################################
    PGEWorldWeather.cpp
    This file is part of PGE.
    PR00F's Game Engine World Weather class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGEWorldWeather.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"
#include "PRRE/include/external/Math/PRREVector.h"

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
    PRREVector vWindDir;        /**< Wind direction. */
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

    virtual ~PGEWorldWeatherImpl();

    bool initialize(
        int numCellsX, int numCellsY, int numCellsZ,
        int cellSize = WorldWeatherCell::PGE_WORLD_WEATHER_DEF_CELL_SIZE);
    bool isInitialized() const;  
    void Shutdown();

protected:

    // ---------------------------------------------------------------------------

private:

    CConsole& con;
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
    Initializes the weather.
*/
bool PGEWorldWeatherImpl::initialize(int numCellsX, int numCellsY, int numCellsZ, int cellSize)   
{
    con.OLnOI("PGEWorldWeather::initialize(%d, %d, %d, %d)", numCellsX, numCellsY, numCellsZ, cellSize);

    if ( bInitialized )
    {
        con.SOLnOO("  > already initialized!");
        con.OLn("");
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
        con.OLn("allocated %d Bytes for weather data", sizeof(WorldWeatherCell) * nCellsX * nCellsY * nCellsZ);
        con.SOLnOO("> initialized!");
        con.OLn("");
        return true;
    }
    catch (const std::bad_alloc&)
    {
        con.EOLnOO("ERROR: memory allocation failure!");
        con.OLn("");
        return false;
    }
}


bool PGEWorldWeatherImpl::isInitialized() const
{
    return bInitialized;
}


void PGEWorldWeatherImpl::Shutdown()
{
    con.OLn("PGEWorldWeather::Shutdown()");
    bInitialized = false;
    delete[] cells;
    cells = NULL;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGEWorldWeatherImpl::PGEWorldWeatherImpl() :
    con( CConsole::getConsoleInstance() ),
    bInitialized( false ),
    cells( NULL ),
    nCellsX( 0 ), nCellsY( 0 ), nCellsZ( 0 )
{

};


PGEWorldWeatherImpl::PGEWorldWeatherImpl(const PGEWorldWeatherImpl&) :
    con( CConsole::getConsoleInstance() ),
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


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################



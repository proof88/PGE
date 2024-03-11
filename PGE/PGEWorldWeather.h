#pragma once

/*
    ###################################################################################
    PGEWorldWeather.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine World Weather class
    Made by PR00F88
    ###################################################################################
*/

#include "PGEallHeaders.h"



/**
    World Weather Cell class for PGEWorldWeather class.
*/
class WorldWeatherCell
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  WorldWeatherCell is included")
#endif

public:

    static const float PGE_WORLD_WEATHER_DEF_CELL_SIZE;   /**< Default size of a cell in meters. */

    /**
        Gets the cell size in meters.
        The default is 1.
    */
    static float getCellSize();

    /**
        Sets the cell size in meters.

        @param size The desired cell size in meters.
    */
    static void SetCellSize(float size);

    // ---------------------------------------------------------------------------

    WorldWeatherCell();

    virtual ~WorldWeatherCell();

protected:

    // ---------------------------------------------------------------------------

    WorldWeatherCell(const WorldWeatherCell&);
    WorldWeatherCell& operator=(const WorldWeatherCell&);

private:

    class WorldWeatherCellImpl;
    WorldWeatherCellImpl* pImpl;


};



/**
    PR00F's Game Engine World Weather class.
*/
class PGEWorldWeather
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEWorldWeather is included")
#endif

public:

    static PGEWorldWeather& PGEWorldWeather::createAndGet();   /**< Creates and gets the singleton instance. */

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&   getConsole() const;                   /**< Returns access to console preset with logger module name as this class. */

    virtual bool initialize(
        int numCellsX, int numCellsY, int numCellsZ,
        int cellSize = WorldWeatherCell::PGE_WORLD_WEATHER_DEF_CELL_SIZE) = 0;   /**< Initializes the weather. */
    virtual bool isInitialized() const = 0;
    virtual void Shutdown() = 0;

};
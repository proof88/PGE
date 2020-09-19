#pragma once

/*
    ###################################################################################
    PGEWorldWeather.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine World Weather class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"



/**
    World Weather Cell class for PGEWorldWeather class.

    This class doesn't use any API directly.
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

    This class doesn't use any API directly.
*/
class PGEWorldWeather
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEWorldWeather is included")
#endif

public:

    static PGEWorldWeather& PGEWorldWeather::createAndGet();   /**< Creates and gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual bool initialize(
        int numCellsX, int numCellsY, int numCellsZ,
        int cellSize = WorldWeatherCell::PGE_WORLD_WEATHER_DEF_CELL_SIZE) = 0;   /**< Initializes the weather. */
    virtual bool isInitialized() const = 0;
    virtual void Shutdown() = 0;

};
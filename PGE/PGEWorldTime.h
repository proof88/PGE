#pragma once

/*
    ###################################################################################
    PGEWorldTime.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine World Time class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"

/**
    PR00F's Game Engine World Time class.
*/
class PGEWorldTime
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEWorldTime is included")
#endif

public:

    static PGEWorldTime& PGEWorldTime::createAndGet();   /**< Creates and gets the singleton instance. */

    // ---------------------------------------------------------------------------

    CConsole&   getConsole() const;                   /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    virtual bool initialize(int days = 0, int hours = 0, int mins = 0, int secs = 0) = 0;   /**< Initializes the virtual time. */
    virtual bool isInitialized() const = 0;  
    virtual void Shutdown() = 0;

    virtual int  getDays() const = 0;       /**< Gets the current virtual day. */
    virtual int  getTime() const = 0;       /**< Gets the time of the current virtual day in millisecs. */
    virtual int  getHours() const = 0;      /**< Gets the hour of the current virtual day. */
    virtual int  getMins() const = 0;       /**< Gets the minutes of the current virtual hour. */
    virtual int  getSecs() const = 0;       /**< Gets the seconds of the current virtual minute. */
    virtual int  getMilliSecs() const = 0;  /**< Gets the milliseconds of the current virtual second. */

    virtual void SetTimeAbsolute(
        int days, int hours = 0, int mins = 0, int secs = 0, int millisecs = 0) = 0;  /**< Sets the virtual day and time. */
    virtual void SetTimeRelative(
        int hours, int mins = 0, int secs = 0, int millisecs = 0) = 0;                /**< Sets the virtual time within the current virtual day. */

    virtual void IncDays() = 0;      /**< Increments the current virtual day by 1. */
    virtual void DecDays() = 0;      /**< Decrements the current virtual day by 1. */
    virtual void IncHours() = 0;     /**< Increments the current virtual hour by 1. */
    virtual void DecHours() = 0;     /**< Decrements the current virtual hour by 1. */
    virtual void IncMins() = 0;      /**< Increments the current virtual minutes by 1. */
    virtual void DecMins() = 0;      /**< Decrements the current virtual minutes by 1. */
    virtual void IncSecs() = 0;      /**< Increments the current virtual seconds by 1. */
    virtual void DecSecs() = 0;      /**< Decrements the current virtual seconds by 1. */
    virtual void IncMilliSecs() = 0; /**< Increments the current virtual milliseconds by 1. */
    virtual void DecMilliSecs() = 0; /**< Decrements the current virtual milliseconds by 1. */

    virtual int  getRealMillisecondsPerVirtualSecond() const = 0;              /**< Gets how many real milliseconds equal to 1 virtual second. */
    virtual void SetRealMillisecondsPerVirtualSecond(int realMillisecs) = 0;   /**< Sets how many real milliseconds equal to 1 virtual second. */
    virtual void AdvanceByRealMilliseconds(int millisecs) = 0;                 /**< Advances virtual time by the given real milliseconds. */

};


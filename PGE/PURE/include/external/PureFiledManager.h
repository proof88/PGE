#pragma once

/*
    ###################################################################################
    PureFiledManager.h
    This file is part of PURE.
    External header.
    PureFiledManager and PureFiledManaged classes.
    Manager loading from / saving to a file.
    Managed loaded from / saved to a file.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureAllHeaders.h"
#include "../../../../../CConsole/CConsole/src/CConsole.h"
#include "PureManager.h"
#include <string>

class PureFiledManager;


/**
    Managed loaded from / saved to a file.
*/
class PureFiledManaged :
    public PureManaged
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureFiledManaged is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureFiledManaged(); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureFiledManaged();

    CConsole&   getManagedConsole() const;           /**< Returns access to console preset with logger module name as this class. */

    virtual TPureuint getUsedSystemMemory() const;   /**< Gets the amount of allocated system memory for this managed. */

    const std::string& getFilename() const;          /**< Gets the filename. */

protected:

    // ---------------------------------------------------------------------------

    PureFiledManaged(const PureFiledManaged& other);
    PureFiledManaged& operator=(const PureFiledManaged& other);

    void SetFilename(const std::string& filename);   /**< Sets the filename. */

private:

    // ---------------------------------------------------------------------------

    class PureFiledManagedImpl;
    PureFiledManagedImpl* pImpl;

}; // class PureFiledManaged


/**
    Manager loading from / saving to a file.
*/
class PureFiledManager :
    public PureManager
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureFiledManager is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureFiledManager(); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureFiledManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    PureFiledManaged*
        getByFilename(const char* filename) const;  /**< Gets the FiledManaged by the specified filename. */
    
    virtual PureFiledManaged*
        createFromFile( const char* filename);      /**< Should create a new managed from the given file. */

    void WriteList() const;                         /**< From PureManager, adding logging file names. */

protected:
    PureFiledManager(const PureFiledManager&);
    PureFiledManager& operator=(const PureFiledManager&);

    virtual void WriteListCallback(const PureManaged& mngd) const;  /**< From PureManager, printing filename. */

private:

    class PureFiledManagerImpl;
    PureFiledManagerImpl* pImpl;

}; // class PureFiledManager


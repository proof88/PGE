#pragma once

/*
    ###################################################################################
    PRREFiledManager.h
    This file is part of PRRE.
    External header.
    PRREFiledManager and PRREFiledManaged classes.
    Manager loading from / saving to a file.
    Managed loaded from / saved to a file.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREManager.h"
#include <string>

class PRREFiledManager;


/**
    Managed loaded from / saved to a file.

    This class doesn't use any API directly.
*/
class PRREFiledManaged :
    public PRREManaged
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREFiledManaged is included")
#endif

public:
    PRREFiledManaged();
    virtual ~PRREFiledManaged();

    virtual TPRREuint getUsedSystemMemory() const;   /**< Gets the amount of allocated system memory for this managed. */

    const std::string& getFilename() const;          /**< Gets the filename. */

protected:

    // ---------------------------------------------------------------------------

    PRREFiledManaged(const PRREFiledManaged& other);
    PRREFiledManaged& operator=(const PRREFiledManaged& other);

    void SetFilename(const std::string& filename);   /**< Sets the filename. */

private:

    // ---------------------------------------------------------------------------

    class PRREFiledManagedImpl;
    PRREFiledManagedImpl* pImpl;

}; // class PRREFiledManaged



/**
    Manager loading from / saving to a file.

    This class doesn't use any API directly.
*/
class PRREFiledManager :
    public PRREManager
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREFiledManager is included")
#endif

public:
    PRREFiledManager();
    virtual ~PRREFiledManager();

    PRREFiledManaged*
        getByFilename(const char* filename) const;  /**< Gets the FiledManaged by the specified filename. */
    
    virtual PRREFiledManaged*
        createFromFile( const char* filename);      /**< Should create a new managed from the given file. */

protected:
    PRREFiledManager(const PRREFiledManager&);
    PRREFiledManager& operator=(const PRREFiledManager&);

    virtual void WriteListCallback(const PRREManaged& mngd) const;  /**< From PRREManager, printing filename. */

private:

}; // class PRREFiledManager


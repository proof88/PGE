#pragma once

/*
    ###################################################################################
    PurehwBase.h
    This file is part of PURE.
    External header.
    Base class for Pure hardware classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"


/**
    Base class for Pure hardware classes.
*/
class PurehwBase
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PurehwBase is included")
#endif

public:

    // ---------------------------------------------------------------------------

    void      Initialize();           /**< Initializes the instance. */
    void      Deinitialize();         /**< Deinitializes the instance. */
    TPurebool isInitialized() const;  /**< Returns whether the instance is successfully initialized. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

protected:

    // ---------------------------------------------------------------------------

    PurehwBase();                  /**< Sets members to default values. */

    PurehwBase(const PurehwBase&);
    PurehwBase& operator=(const PurehwBase&);

    virtual ~PurehwBase();

    virtual void      PreInitialize();         /**< Preinitializes members.
                                                    Being called from ctor and Deinitialize(). */
    virtual TPurebool initializeBase() = 0;    /**< Does the real initialization within descendant class.
                                                    Being called from Initialize(). */
    virtual void      DeinitializeBase() = 0;  /**< Does the real deinitialization within descendant class.
                                                    Being called from Deinitialize(). */

private:

    // ---------------------------------------------------------------------------

    class PurehwBaseImpl;
    PurehwBaseImpl* pImpl;

};

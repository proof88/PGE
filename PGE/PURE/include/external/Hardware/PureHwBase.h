#pragma once

/*
    ###################################################################################
    PureHwBase.h
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
class PureHwBase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwBase is included")
#endif

public:

    // ---------------------------------------------------------------------------

    void      Initialize();           /**< Initializes the instance. */
    void      Deinitialize();         /**< Deinitializes the instance. */
    TPureBool isInitialized() const;  /**< Returns whether the instance is successfully initialized. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

protected:

    // ---------------------------------------------------------------------------

    PureHwBase();                  /**< Sets members to default values. */

    PureHwBase(const PureHwBase&);
    PureHwBase& operator=(const PureHwBase&);

    virtual ~PureHwBase();

    virtual void      PreInitialize();         /**< Preinitializes members.
                                                    Being called from ctor and Deinitialize(). */
    virtual TPureBool initializeBase() = 0;    /**< Does the real initialization within descendant class.
                                                    Being called from Initialize(). */
    virtual void      DeinitializeBase() = 0;  /**< Does the real deinitialization within descendant class.
                                                    Being called from Deinitialize(). */

private:

    // ---------------------------------------------------------------------------

    class PurehwBaseImpl;
    PurehwBaseImpl* pImpl;

};

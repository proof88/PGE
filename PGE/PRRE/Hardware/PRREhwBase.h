#pragma once

/*
    ###################################################################################
    PRREhwBase.h
    This file is part of PRRE.
    External header.
    Base class for PRRE hardware classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "../PRREBaseClass.h"


/**
    Base class for PRRE hardware classes.

    This class doesn't use any API directly.
*/
class PRREhwBase :
    public PRREBaseClass
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwBase is included")
#endif

public:

    // ---------------------------------------------------------------------------

    void      Initialize();           /**< Initializes the instance. */
    void      Deinitialize();         /**< Deinitializes the instance. */
    TPRREbool isInitialized() const;  /**< Returns whether the instance is successfully initialized. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

protected:

    // ---------------------------------------------------------------------------

    PRREhwBase();                  /**< Sets members to default values. */

    PRREhwBase(const PRREhwBase&);
    PRREhwBase& operator=(const PRREhwBase&);

    virtual ~PRREhwBase();

    virtual void      PreInitialize();         /**< Preinitializes members.
                                                    Being called from ctor and Deinitialize(). */
    virtual TPRREbool initializeBase() = 0;    /**< Does the real initialization within descendant class.
                                                    Being called from Initialize(). */
    virtual void      DeinitializeBase() = 0;  /**< Does the real deinitialization within descendant class.
                                                    Being called from Deinitialize(). */

private:

    // ---------------------------------------------------------------------------

    class PRREhwBaseImpl;
    PRREhwBaseImpl* p;

};

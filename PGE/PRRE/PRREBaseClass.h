#pragma once

/*
    ###################################################################################
    PRREBaseClass.h
    This file is part of PRRE.
    External header.
    PRRE base class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREallHeaders.h"
#include "PRREtypes.h"

class CConsole;


/**
    PRRE base class.
    Every decent class descends from this.

    This class doesn't use any API directly.
*/
class PRREBaseClass
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREBaseClass is included")
#endif

public:
             PRREBaseClass();
    virtual ~PRREBaseClass();

protected:

    // ---------------------------------------------------------------------------

    PRREBaseClass(const PRREBaseClass&);
    PRREBaseClass& operator=(const PRREBaseClass& other);

    CConsole&    getConsole() const;      /**< Gets console object. */

private:
    class PRREBaseClassImpl;
    PRREBaseClassImpl* p;

    // ---------------------------------------------------------------------------

}; // class PRREBaseClass

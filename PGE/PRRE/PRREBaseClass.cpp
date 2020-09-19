/*
    ###################################################################################
    PRREBaseClass.cpp
    This file is part of PRRE.
    PRRE base class.
    Every decent class descends from this.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREBaseClass.h"


/*
   PRREBaseClass::PRREBaseClassImpl
   ###########################################################################
*/


class PRREBaseClass::PRREBaseClassImpl
{

public:

    virtual ~PRREBaseClassImpl();

    CConsole&    getConsole() const;

protected:

    // ---------------------------------------------------------------------------

    PRREBaseClassImpl(const PRREBaseClassImpl&);
    PRREBaseClassImpl& operator=(const PRREBaseClassImpl&);

private:
    CConsole& con;                 /**< Console, descendants can read it through getConsole(). */

    // ---------------------------------------------------------------------------

    PRREBaseClassImpl();

    friend class PRREBaseClass;

};


// ############################### PUBLIC ################################


PRREBaseClass::PRREBaseClassImpl::~PRREBaseClassImpl()
{

} // ~PRREBaseClassImpl()


/**
    Gets console object.
    @return Console object.
*/
CConsole& PRREBaseClass::PRREBaseClassImpl::getConsole() const
{
    return con;
} // getConsole()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRREBaseClass::PRREBaseClassImpl::PRREBaseClassImpl() :
    con( CConsole::getConsoleInstance() )
{

} // PRREBaseClassImpl(...)


PRREBaseClass::PRREBaseClassImpl::PRREBaseClassImpl(const PRREBaseClassImpl&) :
    con( CConsole::getConsoleInstance() )
{

}


PRREBaseClass::PRREBaseClassImpl& PRREBaseClass::PRREBaseClassImpl::operator=(const PRREBaseClassImpl&)
{
    return *this;
}


/*
   PRREBaseClass
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREBaseClass::PRREBaseClass()
{
    pImpl = new PRREBaseClassImpl();
    getConsole().OLn("PRREBaseClass() ...");
} // PRREBaseClass(...)


PRREBaseClass::~PRREBaseClass()
{
    getConsole().OLn("~PRREBaseClass() ...");
    delete pImpl;
    pImpl = NULL;
} // ~PRREBaseClass()


/**
    Gets console object.
    @return Console object.
*/
CConsole& PRREBaseClass::getConsole() const
{
    return pImpl->con;
} // getConsole()


// ############################## PROTECTED ##############################


PRREBaseClass::PRREBaseClass(const PRREBaseClass&)
{
    pImpl = new PRREBaseClassImpl();
}


PRREBaseClass& PRREBaseClass::operator=(const PRREBaseClass& other)
{
    return *this;
}


// ############################### PRIVATE ###############################



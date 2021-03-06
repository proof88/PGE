/*
    ###################################################################################
    PRREhwCentralProcessor.cpp
    This file is part of PRRE.
    PRRE central processor capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PRREhwCentralProcessor.h"
#include <climits>
#include "../../include/internal/PRREpragmas.h"

using namespace std;

/*
   PRREhwCentralProcessorImpl
   ###########################################################################
*/


class PRREhwCentralProcessorImpl :
    public PRREhwCentralProcessor
{

public:
    static PRREhwCentralProcessorImpl& get();

    // ---------------------------------------------------------------------------

    CConsole& getConsole() const;

    void WriteStats(); 

protected:
    
    // ---------------------------------------------------------------------------

private:
    static PRREhwCentralProcessorImpl hwCentralProcessorInstance; 

    // ---------------------------------------------------------------------------

    PRREhwCentralProcessorImpl();      /**< Sets members to default values. */

    PRREhwCentralProcessorImpl(const PRREhwCentralProcessorImpl&);
    PRREhwCentralProcessorImpl& operator=(const PRREhwCentralProcessorImpl&);

    virtual ~PRREhwCentralProcessorImpl();

    void      PreInitialize();     /**< Preinitializes members. */
    TPRREbool initializeBase();    /**< Does the real initializations. */
    void      DeinitializeBase();  /**< Does the real deinitialization. */

};


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PRREhwCentralProcessorImpl& PRREhwCentralProcessorImpl::get()
{
    return hwCentralProcessorInstance;
} // get()


CConsole& PRREhwCentralProcessorImpl::getConsole() const
{
    return CConsole::getConsoleInstance("PRREhwCentralProcessor");
} // getConsole()


/**
    Writes statistics to the console.
*/
void PRREhwCentralProcessorImpl::WriteStats()
{
    getConsole().OLn("PRREhwCentralProcessor::WriteStats()");
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRREhwCentralProcessorImpl PRREhwCentralProcessorImpl::hwCentralProcessorInstance;


/** 
    Sets members to default values.
*/
PRREhwCentralProcessorImpl::PRREhwCentralProcessorImpl()
{
    PreInitialize();
} // PRREhwCentralProcessor()


PRREhwCentralProcessorImpl::PRREhwCentralProcessorImpl(const PRREhwCentralProcessorImpl&)
{

}


PRREhwCentralProcessorImpl& PRREhwCentralProcessorImpl::operator=(const PRREhwCentralProcessorImpl&)
{
    return *this;
}


PRREhwCentralProcessorImpl::~PRREhwCentralProcessorImpl()
{
    Deinitialize();
} // ~PRREhwCentralProcessor


/**
    Preinitializes members.
*/
void PRREhwCentralProcessorImpl::PreInitialize()
{

} // PreInitialize()


/**
    Does the real initialization.
*/
TPRREbool PRREhwCentralProcessorImpl::initializeBase()
{
    getConsole().OLn("Number of bits in:");
    getConsole().OLn(" - TPRRE(u)byte: %d",  sizeof(TPRREbyte) * CHAR_BIT);
    getConsole().OLn(" - TPRRE(u)short: %d", sizeof(TPRREshort) * CHAR_BIT);
    getConsole().OLn(" - TPRRE(u)int: %d",   sizeof(TPRREint) * CHAR_BIT);
    getConsole().OLn(" - TPRRE(u)long: %d",  sizeof(TPRRElong) * CHAR_BIT);
    getConsole().OLn(" - TPRREfloat: %d",    sizeof(TPRREfloat) * CHAR_BIT);
    getConsole().OLn(" - TPRREbool: %d",     sizeof(TPRREbool) * CHAR_BIT);
    getConsole().OLn("");

    return true;
} // initializeBase()


/**
    Does the real deinitialization.
*/
void PRREhwCentralProcessorImpl::DeinitializeBase()
{

} // deinitializeBase()


/*
   PRREhwCentralProcessor
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PRREhwCentralProcessor& PRREhwCentralProcessor::get()
{
    return PRREhwCentralProcessorImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################

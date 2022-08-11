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
#include <cstdint>
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

    static const char* getLoggerModuleName();         /**< Returns the logger module name of this class. */

    static bool isMachineBigEndian();                 /**< Returns if the current machine is big endian. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

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


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREhwCentralProcessorImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREhwCentralProcessorImpl::getLoggerModuleName()
{
    return "PRREhwCentralProcessor";
} // getLoggerModuleName()


/**
    Returns if the current machine is big endian.

    @return True if machine is big endian, false otherwise.
*/
bool PRREhwCentralProcessorImpl::isMachineBigEndian()
{
    union
    {
        uint32_t i;
        char c[4];
    } bint = { 0x01020304 };

    return bint.c[0] == 1;
}


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
    getConsole().OLn(" - TPRRE(u)byte : %d", CHAR_BIT * sizeof(TPRREbyte));
    getConsole().OLn(" - TPRRE(u)short: %d", CHAR_BIT * sizeof(TPRREshort));
    getConsole().OLn(" - TPRRE(u)int  : %d", CHAR_BIT * sizeof(TPRREint));
    getConsole().OLn(" - TPRRE(u)long : %d", CHAR_BIT * sizeof(TPRRElong));
    getConsole().OLn(" - TPRREfloat   : %d", CHAR_BIT * sizeof(TPRREfloat));
    getConsole().OLn(" - TPRREbool    : %d", CHAR_BIT * sizeof(TPRREbool));
    getConsole().OLn("");
    getConsole().OLn(" - bool     : %d", CHAR_BIT * sizeof(bool));
    getConsole().OLn(" - char     : %d", CHAR_BIT * sizeof(char));
    getConsole().OLn(" - short    : %d", CHAR_BIT * sizeof(short));
    getConsole().OLn(" - int      : %d", CHAR_BIT * sizeof(int));
    getConsole().OLn(" - long     : %d", CHAR_BIT * sizeof(long));
    getConsole().OLn(" - long long: %d", CHAR_BIT * sizeof(long long));
    getConsole().OLn(" - float    : %d", CHAR_BIT * sizeof(float));
    getConsole().OLn(" - double   : %d", CHAR_BIT * sizeof(double));
    getConsole().OLn("");
    getConsole().OLn(" - int8_t   : %d", CHAR_BIT * sizeof(int8_t));
    getConsole().OLn(" - int16_t  : %d", CHAR_BIT * sizeof(int16_t));
    getConsole().OLn(" - int32_t  : %d", CHAR_BIT * sizeof(int32_t));
    getConsole().OLn(" - int64_t  : %d", CHAR_BIT * sizeof(int64_t));
    getConsole().OLn(" - char16_t : %d", CHAR_BIT * sizeof(char16_t));
    getConsole().OLn(" - char32_t : %d", CHAR_BIT * sizeof(char32_t));
    getConsole().OLn("");
    getConsole().OLn("This machine is %s-endian.", isMachineBigEndian() ? "big" : "little");

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

/*
    ###################################################################################
    PurehwCentralProcessor.cpp
    This file is part of Pure.
    Pure central processor capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PurehwCentralProcessor.h"
#include <climits>
#include <cstdint>
#include "../../include/internal/Purepragmas.h"

using namespace std;

/*
   PurehwCentralProcessorImpl
   ###########################################################################
*/


class PurehwCentralProcessorImpl :
    public PurehwCentralProcessor
{

public:
    static PurehwCentralProcessorImpl& get();

    static const char* getLoggerModuleName();         /**< Returns the logger module name of this class. */

    static bool isMachineBigEndian();                 /**< Returns if the current machine is big endian. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    void WriteStats(); 

protected:
    
    // ---------------------------------------------------------------------------

private:
    static PurehwCentralProcessorImpl hwCentralProcessorInstance; 

    // ---------------------------------------------------------------------------

    PurehwCentralProcessorImpl();      /**< Sets members to default values. */

    PurehwCentralProcessorImpl(const PurehwCentralProcessorImpl&);
    PurehwCentralProcessorImpl& operator=(const PurehwCentralProcessorImpl&);

    virtual ~PurehwCentralProcessorImpl();

    void      PreInitialize();     /**< Preinitializes members. */
    TPureBool initializeBase();    /**< Does the real initializations. */
    void      DeinitializeBase();  /**< Does the real deinitialization. */

};


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PurehwCentralProcessorImpl& PurehwCentralProcessorImpl::get()
{
    return hwCentralProcessorInstance;
} // get()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PurehwCentralProcessorImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PurehwCentralProcessorImpl::getLoggerModuleName()
{
    return "PurehwCentralProcessor";
} // getLoggerModuleName()


/**
    Returns if the current machine is big endian.

    @return True if machine is big endian, false otherwise.
*/
bool PurehwCentralProcessorImpl::isMachineBigEndian()
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
void PurehwCentralProcessorImpl::WriteStats()
{
    getConsole().OLn("PurehwCentralProcessor::WriteStats()");
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PurehwCentralProcessorImpl PurehwCentralProcessorImpl::hwCentralProcessorInstance;


/** 
    Sets members to default values.
*/
PurehwCentralProcessorImpl::PurehwCentralProcessorImpl()
{
    PreInitialize();
} // PurehwCentralProcessor()


PurehwCentralProcessorImpl::PurehwCentralProcessorImpl(const PurehwCentralProcessorImpl&)
{

}


PurehwCentralProcessorImpl& PurehwCentralProcessorImpl::operator=(const PurehwCentralProcessorImpl&)
{
    return *this;
}


PurehwCentralProcessorImpl::~PurehwCentralProcessorImpl()
{
    Deinitialize();
} // ~PurehwCentralProcessor


/**
    Preinitializes members.
*/
void PurehwCentralProcessorImpl::PreInitialize()
{

} // PreInitialize()


/**
    Does the real initialization.
*/
TPureBool PurehwCentralProcessorImpl::initializeBase()
{
    getConsole().OLn("Number of bits in:");
    getConsole().OLn(" - TPure(u)byte : %d", CHAR_BIT * sizeof(TPureByte));
    getConsole().OLn(" - TPure(u)short: %d", CHAR_BIT * sizeof(TPureShort));
    getConsole().OLn(" - TPure(u)int  : %d", CHAR_BIT * sizeof(TPureInt));
    getConsole().OLn(" - TPure(u)long : %d", CHAR_BIT * sizeof(TPureLong));
    getConsole().OLn(" - TPureFloat   : %d", CHAR_BIT * sizeof(TPureFloat));
    getConsole().OLn(" - TPureBool    : %d", CHAR_BIT * sizeof(TPureBool));
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
void PurehwCentralProcessorImpl::DeinitializeBase()
{

} // deinitializeBase()


/*
   PurehwCentralProcessor
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PurehwCentralProcessor& PurehwCentralProcessor::get()
{
    return PurehwCentralProcessorImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################

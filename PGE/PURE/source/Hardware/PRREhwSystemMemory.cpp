/*
    ###################################################################################
    PurehwSystemMemory.cpp
    This file is part of Pure.
    Pure system memory info class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PurehwSystemMemory.h"
#include "../../include/internal/Purepragmas.h"


using namespace std;


/*
   PurehwSystemMemoryImpl
   ###########################################################################
*/

class PurehwSystemMemoryImpl :
    public PurehwSystemMemory
{

public:
    static PurehwSystemMemoryImpl& get();

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPureulong getFreeSystemMemory();
    TPureulong getTotalSystemMemory() const;

    void WriteStats();

protected:

    // ---------------------------------------------------------------------------  

private:
    static PurehwSystemMemoryImpl hwMemoryInstance;

    // ---------------------------------------------------------------------------

    MEMORYSTATUS ms;  /**< To query memory. */   

    PurehwSystemMemoryImpl();                /**< Sets members to default values. */

    PurehwSystemMemoryImpl(const PurehwSystemMemoryImpl&);
    PurehwSystemMemoryImpl& operator=(const PurehwSystemMemoryImpl&);

    virtual ~PurehwSystemMemoryImpl();

    void      PreInitialize();     /**< Preinitializes members. */
    TPurebool initializeBase();    /**< Does the real initializations. */
    void      DeinitializeBase();  /**< Does the real deinitialization. */

};


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PurehwSystemMemoryImpl& PurehwSystemMemoryImpl::get()
{
    return hwMemoryInstance;
} // get()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PurehwSystemMemoryImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PurehwSystemMemoryImpl::getLoggerModuleName()
{
    return "PurehwSystemMemory";
} // getLoggerModuleName()


/**
    Gets the amount of free physical memory.
    @return The amount of free physical memory in Bytes.
*/
TPureulong PurehwSystemMemoryImpl::getFreeSystemMemory() 
{
    GlobalMemoryStatus(&ms);
    return ms.dwAvailPhys;
} // getFreeSystemMemory()


/**
    Gets the total amount of physical memory.
    @return The amount of total physical memory in Bytes.
*/
TPureulong PurehwSystemMemoryImpl::getTotalSystemMemory() const
{
    return ms.dwTotalPhys;
} // getTotalSystemMemory()


/**
    Writes statistics to the console.
*/
void PurehwSystemMemoryImpl::WriteStats()
{
    getConsole().OLn("PurehwSystemMemory::WriteStats()");
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PurehwSystemMemoryImpl PurehwSystemMemoryImpl::hwMemoryInstance;


/** 
    Sets members to default values.
*/
PurehwSystemMemoryImpl::PurehwSystemMemoryImpl()
{
    PreInitialize();
} // PurehwSystemMemory()


PurehwSystemMemoryImpl::PurehwSystemMemoryImpl(const PurehwSystemMemoryImpl&)
{

}

    
PurehwSystemMemoryImpl& PurehwSystemMemoryImpl::operator=(const PurehwSystemMemoryImpl&)
{
    return *this;
}


PurehwSystemMemoryImpl::~PurehwSystemMemoryImpl()
{
    Deinitialize();
} // ~PurehwSystemMemory


/**
    Preinitializes members.
*/
void PurehwSystemMemoryImpl::PreInitialize()
{
    memset(&ms, 0, sizeof(ms));
    ms.dwLength = sizeof(ms);
} // PreInitialize()


/**
    Does the real initialization.
*/
TPurebool PurehwSystemMemoryImpl::initializeBase()
{
    getFreeSystemMemory();

    // DO NOT judge system on free/total system memory until it is fixed (e.g. 32bit app gives 0 mib total memory on a 64bit system with much memory)
    getConsole().OLn("");
    getConsole().OLn("total RAM: %i kiByte(s) = %i MiByte(s)", getTotalSystemMemory()/1024, getTotalSystemMemory()/1024/1024);
    getConsole().OLn("free  RAM: %i kiByte(s) = %i MiByte(s)", getFreeSystemMemory()/1024, getFreeSystemMemory()/1024/1024);
    getConsole().OLn("");

    return true;
} // initializeBase()


/**
    Does the real deinitialization.
*/
void PurehwSystemMemoryImpl::DeinitializeBase()
{

} // deinitializeBase()



/*
   PurehwSystemMemory
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PurehwSystemMemory& PurehwSystemMemory::get()
{
    return PurehwSystemMemoryImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################
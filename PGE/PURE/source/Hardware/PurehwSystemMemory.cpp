/*
    ###################################################################################
    PureHwSystemMemory.cpp
    This file is part of PURE.
    Pure system memory info class.
    Made by PR00F88
    ###################################################################################
*/


#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PureHwSystemMemory.h"
#include "../../include/internal/PurePragmas.h"


using namespace std;


/*
   PureHwSystemMemoryImpl
   ###########################################################################
*/

class PureHwSystemMemoryImpl :
    public PureHwSystemMemory
{

public:
    static PureHwSystemMemoryImpl& get();

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPureULong getFreeSystemMemory();
    TPureULong getTotalSystemMemory() const;

    void WriteStats();

protected:

    // ---------------------------------------------------------------------------  

private:
    static PureHwSystemMemoryImpl hwMemoryInstance;

    // ---------------------------------------------------------------------------

    MEMORYSTATUS ms;  /**< To query memory. */   

    PureHwSystemMemoryImpl();                /**< Sets members to default values. */

    PureHwSystemMemoryImpl(const PureHwSystemMemoryImpl&);
    PureHwSystemMemoryImpl& operator=(const PureHwSystemMemoryImpl&);

    virtual ~PureHwSystemMemoryImpl();

    void      PreInitialize();     /**< Preinitializes members. */
    TPureBool initializeBase();    /**< Does the real initializations. */
    void      DeinitializeBase();  /**< Does the real deinitialization. */

};


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PureHwSystemMemoryImpl& PureHwSystemMemoryImpl::get()
{
    return hwMemoryInstance;
} // get()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureHwSystemMemoryImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureHwSystemMemoryImpl::getLoggerModuleName()
{
    return "PureHwSystemMemory";
} // getLoggerModuleName()


/**
    Gets the amount of free physical memory.
    @return The amount of free physical memory in Bytes.
*/
TPureULong PureHwSystemMemoryImpl::getFreeSystemMemory() 
{
    GlobalMemoryStatus(&ms);
    return ms.dwAvailPhys;
} // getFreeSystemMemory()


/**
    Gets the total amount of physical memory.
    @return The amount of total physical memory in Bytes.
*/
TPureULong PureHwSystemMemoryImpl::getTotalSystemMemory() const
{
    return ms.dwTotalPhys;
} // getTotalSystemMemory()


/**
    Writes statistics to the console.
*/
void PureHwSystemMemoryImpl::WriteStats()
{
    getConsole().OLn("PureHwSystemMemory::WriteStats()");
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureHwSystemMemoryImpl PureHwSystemMemoryImpl::hwMemoryInstance;


/** 
    Sets members to default values.
*/
PureHwSystemMemoryImpl::PureHwSystemMemoryImpl()
{
    PreInitialize();
} // PureHwSystemMemory()


PureHwSystemMemoryImpl::PureHwSystemMemoryImpl(const PureHwSystemMemoryImpl&)
{

}

    
PureHwSystemMemoryImpl& PureHwSystemMemoryImpl::operator=(const PureHwSystemMemoryImpl&)
{
    return *this;
}


PureHwSystemMemoryImpl::~PureHwSystemMemoryImpl()
{
    Deinitialize();
} // ~PureHwSystemMemory


/**
    Preinitializes members.
*/
void PureHwSystemMemoryImpl::PreInitialize()
{
    memset(&ms, 0, sizeof(ms));
    ms.dwLength = sizeof(ms);
} // PreInitialize()


/**
    Does the real initialization.
*/
TPureBool PureHwSystemMemoryImpl::initializeBase()
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
void PureHwSystemMemoryImpl::DeinitializeBase()
{

} // deinitializeBase()



/*
   PureHwSystemMemory
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PureHwSystemMemory& PureHwSystemMemory::get()
{
    return PureHwSystemMemoryImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################
/*
    ###################################################################################
    PRREhwSystemMemory.cpp
    This file is part of PRRE.
    PRRE system memory info class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREbaseIncludes.h"  // PCH
#include "../PRREpragmas.h"
#include "PRREhwSystemMemory.h"

using namespace std;


/*
   PRREhwSystemMemoryImpl
   ###########################################################################
*/

class PRREhwSystemMemoryImpl :
    public PRREhwSystemMemory
{

public:
    static PRREhwSystemMemoryImpl& get();

    // ---------------------------------------------------------------------------

    TPRREulong getFreeSystemMemory();
    TPRREulong getTotalSystemMemory() const;

    void WriteStats();

protected:

    // ---------------------------------------------------------------------------  

private:
    static PRREhwSystemMemoryImpl hwMemoryInstance;

    // ---------------------------------------------------------------------------

    MEMORYSTATUS ms;  /**< To query memory. */   

    PRREhwSystemMemoryImpl();                /**< Sets members to default values. */

    PRREhwSystemMemoryImpl(const PRREhwSystemMemoryImpl&);
    PRREhwSystemMemoryImpl& operator=(const PRREhwSystemMemoryImpl&);

    virtual ~PRREhwSystemMemoryImpl();

    void      PreInitialize();     /**< Preinitializes members. */
    TPRREbool initializeBase();    /**< Does the real initializations. */
    void      DeinitializeBase();  /**< Does the real deinitialization. */

};


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PRREhwSystemMemoryImpl& PRREhwSystemMemoryImpl::get()
{
    return hwMemoryInstance;
} // get()


/**
    Gets the amount of free physical memory.
    @return The amount of free physical memory in Bytes.
*/
TPRREulong PRREhwSystemMemoryImpl::getFreeSystemMemory() 
{
    GlobalMemoryStatus(&ms);
    return ms.dwAvailPhys;
} // getFreeSystemMemory()


/**
    Gets the total amount of physical memory.
    @return The amount of total physical memory in Bytes.
*/
TPRREulong PRREhwSystemMemoryImpl::getTotalSystemMemory() const
{
    return ms.dwTotalPhys;
} // getTotalSystemMemory()


/**
    Writes statistics to the console.
*/
void PRREhwSystemMemoryImpl::WriteStats()
{
    getConsole().OLn("PRREhwSystemMemory::WriteStats()");
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRREhwSystemMemoryImpl PRREhwSystemMemoryImpl::hwMemoryInstance;


/** 
    Sets members to default values.
*/
PRREhwSystemMemoryImpl::PRREhwSystemMemoryImpl()
{
    PreInitialize();
} // PRREhwSystemMemory()


PRREhwSystemMemoryImpl::PRREhwSystemMemoryImpl(const PRREhwSystemMemoryImpl&)
{

}

    
PRREhwSystemMemoryImpl& PRREhwSystemMemoryImpl::operator=(const PRREhwSystemMemoryImpl&)
{
    return *this;
}


PRREhwSystemMemoryImpl::~PRREhwSystemMemoryImpl()
{
    Deinitialize();
} // ~PRREhwSystemMemory


/**
    Preinitializes members.
*/
void PRREhwSystemMemoryImpl::PreInitialize()
{
    memset(&ms, 0, sizeof(ms));
    ms.dwLength = sizeof(ms);
} // PreInitialize()


/**
    Does the real initialization.
*/
TPRREbool PRREhwSystemMemoryImpl::initializeBase()
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
void PRREhwSystemMemoryImpl::DeinitializeBase()
{

} // deinitializeBase()



/*
   PRREhwSystemMemory
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PRREhwSystemMemory& PRREhwSystemMemory::get()
{
    return PRREhwSystemMemoryImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################
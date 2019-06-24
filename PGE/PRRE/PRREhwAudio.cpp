/*
    ###################################################################################
    PRREhwAudio.cpp
    This file is part of PRRE.
    PRRE audio capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREhwAudio.h"

using namespace std;


/*
   PRREhwAudioImpl
   ###########################################################################
*/

class PRREhwAudioImpl :
    public PRREhwAudio
{

public:

    static PRREhwAudioImpl& get();

    // ---------------------------------------------------------------------------

    void WriteStats();

protected:

private:
    static PRREhwAudioImpl hwAudioInstance;

    // ---------------------------------------------------------------------------

    PRREhwAudioImpl();                /**< Sets members to default values. */

    PRREhwAudioImpl(const PRREhwAudioImpl&);
    PRREhwAudioImpl& operator=(const PRREhwAudioImpl&);

    virtual ~PRREhwAudioImpl();

    void      PreInitialize();     /**< Preinitializes members. */
    TPRREbool initializeBase();    /**< Does the real initializations. */
    void      DeinitializeBase();  /**< Does the real deinitialization. */

};


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PRREhwAudioImpl& PRREhwAudioImpl::get()
{
    return hwAudioInstance;
} // get()


/**
    Writes statistics to the console.
*/
void PRREhwAudioImpl::WriteStats()
{
    getConsole().OLn("PRREhwAudio::WriteStats()");
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRREhwAudioImpl PRREhwAudioImpl::hwAudioInstance;


/** 
    Sets members to default values.
*/
PRREhwAudioImpl::PRREhwAudioImpl()
{
    PreInitialize();
} // PRREhwInfo()


PRREhwAudioImpl::PRREhwAudioImpl(const PRREhwAudioImpl&)
{

}


PRREhwAudioImpl& PRREhwAudioImpl::operator=(const PRREhwAudioImpl&)
{
    return *this;
}
                                             

PRREhwAudioImpl::~PRREhwAudioImpl()
{
    Deinitialize();
} // ~PRREhwAudio


/**
    Preinitializes members.
*/
void PRREhwAudioImpl::PreInitialize()
{

} // PreInitialize()


/**
    Does the real initialization.
*/
TPRREbool PRREhwAudioImpl::initializeBase()
{
    return true;
} // initializeBase()


/**
    Does the real deinitialization.
*/
void PRREhwAudioImpl::DeinitializeBase()
{

} // deinitializeBase()




/*
   PRREhwAudio
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PRREhwAudio& PRREhwAudio::get()
{
    return PRREhwAudioImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################
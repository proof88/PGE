/*
    ###################################################################################
    PRREMaterialManager.cpp
    This file is part of PRRE.
    PRREMaterialManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Material/PRREMaterialManager.h"
#include "../../include/internal/Material/PRREMaterialImpl.h"
#include "../../include/internal/PRREpragmas.h"
#include "../../include/external/Hardware/PRREhwInfo.h"

using namespace std;

/*
   PRREMaterialManager::PRREMaterialManagerImpl
   ###########################################################################
*/


class PRREMaterialManager::PRREMaterialManagerImpl
{

public:
    virtual ~PRREMaterialManagerImpl();

    TPRREbool isInitialized() const;

    TPRREuint getMaximumLayerCount() const;

protected:

private:
    PRREMaterialManager* _pOwner;         /**< The owner public object who creates this pimpl object. */

    TPRREbool           bInited;          /**< True if successfully inited, false if not functional. */
    TPRREuint           nRunningCounter;  /**< Always increased when creating a new Material instance. */
    TPRREuint           nMaxLayers;       /**< Max number of layers per material based on number of TMUs. */

    // ---------------------------------------------------------------------------

    PRREMaterialManagerImpl();
    PRREMaterialManagerImpl(PRREMaterialManager* owner);

    PRREMaterialManagerImpl(const PRREMaterialManagerImpl&);
    PRREMaterialManagerImpl& operator=(const PRREMaterialManagerImpl&);

    friend class PRREMaterialManager;

};


// ############################### PUBLIC ################################


PRREMaterialManager::PRREMaterialManagerImpl::~PRREMaterialManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PRREMaterialManager() ...");
    _pOwner->DeleteAll();
    nMaxLayers = 1;
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
} // ~PRREObject3DManager()


TPRREbool PRREMaterialManager::PRREMaterialManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


TPRREuint PRREMaterialManager::PRREMaterialManagerImpl::getMaximumLayerCount() const
{
    return nMaxLayers;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRREMaterialManager::PRREMaterialManagerImpl::PRREMaterialManagerImpl()
{
    bInited = false;
}


/**
    @param owner Public class instance owning this pimpl object.
*/
PRREMaterialManager::PRREMaterialManagerImpl::PRREMaterialManagerImpl(PRREMaterialManager* owner)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PRREMaterialManager() ...");
    bInited = false;
    // we always assume at least 1 TMU for the SW renderer case
    nRunningCounter = 0;
    nMaxLayers = max(PRREhwInfo::get().getVideo().getTextureUnitsCount(), 1);
    bInited = PRREhwInfo::get().getVideo().isInitialized();
    _pOwner->getConsole().SOLnOO("Done!");
} // PRREObject3DManager(...)


PRREMaterialManager::PRREMaterialManagerImpl::PRREMaterialManagerImpl(const PRREMaterialManager::PRREMaterialManagerImpl&)
{

}


PRREMaterialManager::PRREMaterialManagerImpl& PRREMaterialManager::PRREMaterialManagerImpl::operator=(const PRREMaterialManager::PRREMaterialManagerImpl&)
{
    return *this;    
}



/*
   PRREMaterialManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREMaterialManager::PRREMaterialManager()
{
    pImpl = new PRREMaterialManagerImpl(this);
} // PRREMaterialManager(...)


PRREMaterialManager::~PRREMaterialManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREMaterialManager()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREMaterialManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREMaterialManager::getLoggerModuleName()
{
    return "PRREMaterialManager";
} // getLoggerModuleName()


/**
    Tells whether the object is correctly initialized or not.
    @return True if the given MaterialManager instance was initialized correctly, false otherwise.
*/
TPRREbool PRREMaterialManager::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


/**
    Tells maximum number of layers allowed per material.
    This depends on number of hardware texture mapping units in current system.
*/
TPRREuint PRREMaterialManager::getMaximumLayerCount() const
{
    return pImpl->getMaximumLayerCount();
}


/**
    Creates a new material.

    @return New material. PGENULL if MaterialManager is not yet initialized.
*/

PRREMaterial* PRREMaterialManager::createMaterial()
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREMaterialManager::createMaterial()");

    PRREMaterial* mat = PGENULL;
    try
    {
        mat = new PRREMaterial(pImpl->nMaxLayers);
        Attach( *mat );

        mat->SetName("Material " + std::to_string(pImpl->nRunningCounter++));

        getConsole().SOLnOO("> Material created successfully!");
    }
    catch (const std::bad_alloc&)
    {
        getConsole().EOLnOO("ERROR: Failed to instantiate new PRREMaterial, name: %s!", mat->getName().c_str());
    }
    getConsole().OLn("");

    return mat;
        
} // createMaterial()


void PRREMaterialManager::WriteList() const
{
    getConsole().OLn("PRREMaterialManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PRREFiledManager::WriteList();
    getConsole().OO();
}



// ############################## PROTECTED ##############################


PRREMaterialManager::PRREMaterialManager(const PRREMaterialManager&)
{

}


PRREMaterialManager& PRREMaterialManager::operator=(const PRREMaterialManager&)
{
    return *this;    
}


void PRREMaterialManager::WriteListCallback(const PRREManaged& mngd) const
{
    const PRREMaterial& mat = (PRREMaterial&) mngd;
    PRREFiledManager::WriteListCallback(mat);
    getConsole().OIOLnOO("textures: %d, indices: %d, colors: %d, texcoords: %d", mat.getTextureCount(), mat.getIndicesCount(), mat.getColorsCount(), mat.getTexcoordsCount());
} // WriteListCallback()


// ############################### PRIVATE ###############################



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
#include "../PRREpragmas.h"
#include "PRREMaterialManager.h"
#include "PRREMaterialImpl.h"
#include "../Hardware/PRREhwInfo.h"

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

    PRREMaterialManager* _pOwner;      /**< The owner public object who creates this pimpl object. */

    TPRREbool           bInited;       /**< True if successfully inited, false if not functional. */
    TPRREuint           nMaxLayers;    /**< Max number of layers per material based on number of TMUs. */

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


PRREMaterialManager::PRREMaterialManager()
{
    pImpl = new PRREMaterialManagerImpl(this);
} // PRREObject3DManager(...)


PRREMaterialManager::~PRREMaterialManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREObject3DManager()


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

    PRREMaterial* const mat = new PRREMaterial(pImpl->nMaxLayers);
    Attach( *mat );

    getConsole().SOLnOO("> Material created successfully!");
    getConsole().OLn("");

    return mat;
        
} // createMaterial()



// ############################## PROTECTED ##############################


PRREMaterialManager::PRREMaterialManager(const PRREMaterialManager&)
{

}


PRREMaterialManager& PRREMaterialManager::operator=(const PRREMaterialManager&)
{
    return *this;    
}


// ############################### PRIVATE ###############################



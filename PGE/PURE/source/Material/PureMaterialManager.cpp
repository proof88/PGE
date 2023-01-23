/*
    ###################################################################################
    PureMaterialManager.cpp
    This file is part of Pure.
    PureMaterialManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Material/PureMaterialManager.h"
#include "../../include/internal/Material/PureMaterialImpl.h"
#include "../../include/internal/Purepragmas.h"
#include "../../include/external/Hardware/PureHwInfo.h"

using namespace std;

/*
   PureMaterialManager::PureMaterialManagerImpl
   ###########################################################################
*/


class PureMaterialManager::PureMaterialManagerImpl
{

public:
    virtual ~PureMaterialManagerImpl();

    TPureBool isInitialized() const;

    TPureUInt getMaximumLayerCount() const;

protected:

private:
    PureMaterialManager* _pOwner;         /**< The owner public object who creates this pimpl object. */

    TPureBool           bInited;          /**< True if successfully inited, false if not functional. */
    TPureUInt           nRunningCounter;  /**< Always increased when creating a new Material instance. */
    TPureUInt           nMaxLayers;       /**< Max number of layers per material based on number of TMUs. */

    // ---------------------------------------------------------------------------

    PureMaterialManagerImpl();
    PureMaterialManagerImpl(PureMaterialManager* owner);

    PureMaterialManagerImpl(const PureMaterialManagerImpl&);
    PureMaterialManagerImpl& operator=(const PureMaterialManagerImpl&);

    friend class PureMaterialManager;

};


// ############################### PUBLIC ################################


PureMaterialManager::PureMaterialManagerImpl::~PureMaterialManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PureMaterialManager() ...");
    _pOwner->DeleteAll();
    nMaxLayers = 1;
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
} // ~PureObject3DManager()


TPureBool PureMaterialManager::PureMaterialManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


TPureUInt PureMaterialManager::PureMaterialManagerImpl::getMaximumLayerCount() const
{
    return nMaxLayers;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureMaterialManager::PureMaterialManagerImpl::PureMaterialManagerImpl()
{
    bInited = false;
}


/**
    @param owner Public class instance owning this pimpl object.
*/
PureMaterialManager::PureMaterialManagerImpl::PureMaterialManagerImpl(PureMaterialManager* owner)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PureMaterialManager() ...");
    bInited = false;
    // we always assume at least 1 TMU for the SW renderer case
    nRunningCounter = 0;
    nMaxLayers = max(PureHwInfo::get().getVideo().getTextureUnitsCount(), 1);
    bInited = PureHwInfo::get().getVideo().isInitialized();
    _pOwner->getConsole().SOLnOO("Done!");
} // PureObject3DManager(...)


PureMaterialManager::PureMaterialManagerImpl::PureMaterialManagerImpl(const PureMaterialManager::PureMaterialManagerImpl&)
{

}


PureMaterialManager::PureMaterialManagerImpl& PureMaterialManager::PureMaterialManagerImpl::operator=(const PureMaterialManager::PureMaterialManagerImpl&)
{
    return *this;    
}



/*
   PureMaterialManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureMaterialManager::PureMaterialManager()
{
    pImpl = new PureMaterialManagerImpl(this);
} // PureMaterialManager(...)


PureMaterialManager::~PureMaterialManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureMaterialManager()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureMaterialManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureMaterialManager::getLoggerModuleName()
{
    return "PureMaterialManager";
} // getLoggerModuleName()


/**
    Tells whether the object is correctly initialized or not.
    @return True if the given MaterialManager instance was initialized correctly, false otherwise.
*/
TPureBool PureMaterialManager::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


/**
    Tells maximum number of layers allowed per material.
    This depends on number of hardware texture mapping units in current system.
*/
TPureUInt PureMaterialManager::getMaximumLayerCount() const
{
    return pImpl->getMaximumLayerCount();
}


/**
    Creates a new material.

    @return New material. PGENULL if MaterialManager is not yet initialized.
*/

PureMaterial* PureMaterialManager::createMaterial()
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PureMaterialManager::createMaterial()");

    PureMaterial* mat = PGENULL;
    try
    {
        mat = new PureMaterial(pImpl->nMaxLayers);
        Attach( *mat );

        mat->SetName("Material " + std::to_string(pImpl->nRunningCounter++));

        getConsole().SOLnOO("> Material created successfully!");
    }
    catch (const std::bad_alloc&)
    {
        getConsole().EOLnOO("ERROR: Failed to instantiate new PureMaterial, name: %s!", mat->getName().c_str());
    }
    getConsole().OLn("");

    return mat;
        
} // createMaterial()


/**
    From PureFiledManager, adding logging material data.
*/
void PureMaterialManager::WriteList() const
{
    getConsole().OLn("PureMaterialManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PureFiledManager::WriteList();
    getConsole().OO();
}



// ############################## PROTECTED ##############################


PureMaterialManager::PureMaterialManager(const PureMaterialManager&)
{

}


PureMaterialManager& PureMaterialManager::operator=(const PureMaterialManager&)
{
    return *this;    
}


void PureMaterialManager::WriteListCallback(const PureManaged& mngd) const
{
    const PureMaterial& mat = (PureMaterial&) mngd;
    PureFiledManager::WriteListCallback(mat);
    getConsole().OIOLnOO("textures: %d, indices: %d, colors: %d, texcoords: %d", mat.getTextureCount(), mat.getIndicesCount(), mat.getColorsCount(), mat.getTexcoordsCount());
} // WriteListCallback()


// ############################### PRIVATE ###############################



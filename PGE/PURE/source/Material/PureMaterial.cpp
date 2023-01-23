/*
    ###################################################################################
    PureMaterial.cpp
    This file is part of Pure.
    PureMaterial class.
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

    Notes
    ***************************

        glMaterialfv(GL_FRONT, GL_AMBIENT , ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE , diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);

        mtl format:
        http://en.wikipedia.org/wiki/Wavefront_.obj_file
        http://web.cse.ohio-state.edu/~hwshen/581/Site/Lab3_files/Labhelp_Obj_parser.htm
        http://casual-effects.blogspot.hu/2012/01/translucency-in-obj-mtl-files.html
        http://paulbourke.net/dataformats/mtl/

*/


/*
   PureMaterial::PureMaterialImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureMaterial::PureMaterialImpl::~PureMaterialImpl()
{
    _pOwner->getManagedConsole().OLnOI("~PureMaterial() ...");

    for (TPureUInt i = 0; i < layers.size(); i++)
    {
        layers[i].nColors_h = layers[i].nTexcoords_h = 0;
        delete[] layers[i].pColors;
        delete[] layers[i].pTexcoords;
        layers[i].pColors = PGENULL;
        layers[i].pTexcoords = PGENULL;
    }
    nIndices = 0;
    m_nIndexSize = 0;

    _pOwner->getManagedConsole().SOLnOO("Done!");
} // ~PureTexture()


TPureBool PureMaterial::PureMaterialImpl::allocateArrays(TPureUInt nColorCount, TPureUInt nTexcoordCount, TPureUInt nIndexCount, TPureUInt nIndexSize)
{
    if ( nIndexSize == 0 )
    {
        _pOwner->getManagedConsole().EOLn("PureMaterial()::allocateArrays(%d, %d, %d, %d) fail due to nIndexSize!", nColorCount, nTexcoordCount, nIndexCount, nIndexSize);
        return false;
    }

    if ( getTexcoordsCount(0) > 0 )
    {
        _pOwner->getManagedConsole().SOLn("PureMaterial()::allocateArrays(%d, %d, %d, %d) skip, already allocated!", nColorCount, nTexcoordCount, nIndexCount, nIndexSize);
        return true;
    }

    nIndices   = nIndexCount;
    m_nIndexSize = nIndexSize;

    try
    {
        for (TPureUInt i = 0; i < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount(); i++)
        {
            layers[i].nColors_h          = nColorCount;
            layers[i].pColors            = new TRGBAFLOAT[nColorCount];
            layers[i].nTexcoords_h       = nTexcoordCount;
            layers[i].pTexcoords         = new TUVW[nTexcoordCount];
        }
        return true;
    }
    catch (const std::bad_alloc&)
    {
        _pOwner->getManagedConsole().EOLn("PureMaterial()::allocateArrays(%d, %d, %d, %d) fail due to out of memory!", nColorCount, nTexcoordCount, nIndexCount, nIndexSize);
        for (TPureUInt i = 0; i < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount(); i++)
        {
            layers[i].nColors_h = 0;
            delete[] layers[i].pColors;
            layers[i].pColors = PGENULL;
            layers[i].nTexcoords_h = 0;
            delete[] layers[i].pTexcoords;
            layers[i].pTexcoords = PGENULL;
        }
        return false;
    }
}


TPureUInt PureMaterial::PureMaterialImpl::getColorsCount(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].nColors_h;
    else
    {
        // TODO: set lasterr
        return 0;
    }
} // getTexcoordsCount()


TRGBAFLOAT* PureMaterial::PureMaterialImpl::getColors(TPureUInt level)
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pColors;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
} // getTexcoords()


const TRGBAFLOAT* PureMaterial::PureMaterialImpl::getColors(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pColors;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
} // getTexcoords()


TPureUInt PureMaterial::PureMaterialImpl::getIndicesCount() const
{
    return nIndices;
}


TPureUInt PureMaterial::PureMaterialImpl::getTexcoordsCount(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].nTexcoords_h;
    else
    {
        // TODO: set lasterr
        return 0;
    }
} // getTexcoordsCount()


TUVW* PureMaterial::PureMaterialImpl::getTexcoords(TPureUInt level)
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pTexcoords;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
} // getTexcoords()


const TUVW* PureMaterial::PureMaterialImpl::getTexcoords(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pTexcoords;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
} // getTexcoords()


PureTexture* PureMaterial::PureMaterialImpl::getTexture(TPureUInt level)
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].tex;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
}


const PureTexture* PureMaterial::PureMaterialImpl::getTexture(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].tex;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
}


TPureBool PureMaterial::PureMaterialImpl::setTexture(PureTexture* tex, TPureUInt level)
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
    {
        layers[level].tex = tex;
        return true;
    }
    return false;
}


TPureUInt PureMaterial::PureMaterialImpl::getTextureCount() const
{
    TPureUInt nFound = 0;
    for (TPureUInt i = 0; i < layers.size(); i++)
        if ( layers[i].tex != PGENULL )
            nFound++;

    return nFound;
}


TPureBool PureMaterial::PureMaterialImpl::isTextured() const
{
    return getTextureCount() > 0;
}


TPureBool PureMaterial::PureMaterialImpl::isSingleTextured() const
{
    return getTextureCount() == 1;
}


TPureBool PureMaterial::PureMaterialImpl::isMultiTextured() const
{
    return getTextureCount() > 1;
}


PureColor& PureMaterial::PureMaterialImpl::getTextureEnvColor(TPureUInt level)
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].clrTexEnv;
    else
    {
        // TODO: set lasterr
        return layers[0].clrTexEnv;
    } 
}


const PureColor& PureMaterial::PureMaterialImpl::getTextureEnvColor(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].clrTexEnv;
    else
    {
        // TODO: set lasterr
        return layers[0].clrTexEnv;
    }
}


TPURE_BLENDFACTOR PureMaterial::PureMaterialImpl::getSourceBlendFunc(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].blendFactorSource;
    else
    {
        // TODO: set lasterr
        return layers[0].blendFactorSource;
    }
} // getSourceBlendFunc()


TPureBool PureMaterial::PureMaterialImpl::setSourceBlendFunc(TPURE_BLENDFACTOR value, TPureUInt level)
{
    if ( !_pOwner->getUtiliser() || !_pOwner->getUtiliser()->getManager() )
    {
        // this way we permit the user setting blending again when the utiliser Object3D is not attached to its manager,
        // so it can be assured that in case of reattach, it is still non-blended, without even checking blending state!
        _pOwner->getManagedConsole().EOLn("ERROR: %s either no utiliser or utiliser has no parent!", __FUNCTION__);
        return false;
    }

    if ( (value != PURE_SRC_COLOR) && (value != PURE_ONE_MINUS_SRC_COLOR) )
    {
        if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        {
            layers[level].blendFactorSource = value;

            // remember: Object3D is also a manager, so it could also implement its own HandleManagedPropertyChanged(),
            // to handle changes in the material of its subobjects! Since Object3D doesnt implement such,
            // the PureManager's base HandleManagedPropertyChanged() is invoked when subobject's material is changed here!
            _pOwner->getUtiliser()->getManager()->HandleManagedPropertyChanged( *(_pOwner->getUtiliser()) );

            return true;
        }
    }
    return false;
} // setSourceBlendFunc()


TPURE_BLENDFACTOR PureMaterial::PureMaterialImpl::getDestinationBlendFunc(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].blendFactorDestination;
    else
    {
        // TODO: set lasterr
        return layers[0].blendFactorDestination;
    }
} // getDestinationBlendFunc()


TPureBool PureMaterial::PureMaterialImpl::setDestinationBlendFunc(TPURE_BLENDFACTOR value, TPureUInt level)
{
    if ( !_pOwner->getUtiliser() || !_pOwner->getUtiliser()->getManager() )
    {
        // this way we permit the user setting blending again when the utiliser Object3D is not attached to its manager,
        // so it can be assured that in case of reattach, it is still non-blended, without even checking blending state!
        _pOwner->getManagedConsole().EOLn("ERROR: %s either no utiliser or utiliser has no parent!", __FUNCTION__);
        return false;
    }

    if ( (value != PURE_DST_COLOR) && (value != PURE_ONE_MINUS_DST_COLOR) && (value != PURE_SRC_ALPHA_SATURATE) )
    {
        if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        {
            layers[level].blendFactorDestination = value;

            // remember: Object3D is also a manager, so it could also implement its own HandleManagedPropertyChanged(),
            // to handle changes in the material of its subobjects! Since Object3D doesnt implement such,
            // the PureManager's base HandleManagedPropertyChanged() is invoked when subobject's material is changed here!
            _pOwner->getUtiliser()->getManager()->HandleManagedPropertyChanged( *(_pOwner->getUtiliser()) );

            return true;
        }
    }
    return false;
} // setDestinationBlendFunc()


TPureBool PureMaterial::PureMaterialImpl::setBlendFuncs(TPURE_BLENDFACTOR src, TPURE_BLENDFACTOR dst, TPureUInt level)
{
    return setSourceBlendFunc( src, level ) &&
        setDestinationBlendFunc( dst, level );
} // setBlendFuncs()


TPURE_BLENDMODE PureMaterial::PureMaterialImpl::getBlendMode(TPureUInt level) const
{
    if ( level < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
    {
        if ( (layers[level].blendFactorSource == PURE_ONE) && (layers[level].blendFactorDestination == PURE_ZERO) )
            return PURE_BM_NONE;
        else if ( (layers[level].blendFactorSource == PURE_SRC_ALPHA) && (layers[level].blendFactorDestination == PURE_ONE_MINUS_SRC_ALPHA) )
            return PURE_BM_STANDARD_TRANSPARENCY;
        else
            return PURE_BM_MANUAL;
    }
    else
    {
        // TODO: set lasterr
        return PURE_BM_NONE;
    }
    
} // getBlendMode()


TPureBool PureMaterial::PureMaterialImpl::setBlendMode(TPURE_BLENDMODE mode, TPureUInt level)
{
    switch (mode)
    {
    case PURE_BM_NONE:
        return setBlendFuncs(PURE_ONE, PURE_ZERO, level);
    case PURE_BM_STANDARD_TRANSPARENCY:
        return setBlendFuncs(PURE_SRC_ALPHA, PURE_ONE_MINUS_SRC_ALPHA, level);
    default:
        return false;
    }
} // setBlendMode()


TPureBool PureMaterial::PureMaterialImpl::copyFromMaterial(PureMaterial& srcMat, TPureUInt dstLevel, TPureUInt srcLevel)
{
    if ( (dstLevel < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount()) && 
         (srcLevel < ((PureMaterialManager*)_pOwner->getManager())->getMaximumLayerCount()) )
    {
        // we also want to make sure that we have the same memory layout of texcoords and indices
        // so we can safely copy data into our preallocated arrays
        if ( (layers[dstLevel].nTexcoords_h == srcMat.pImpl->layers[srcLevel].nTexcoords_h) &&
             (layers[dstLevel].nColors_h == srcMat.pImpl->layers[srcLevel].nColors_h) &&
             (nIndices == srcMat.getIndicesCount()) &&
             (m_nIndexSize == srcMat.pImpl->m_nIndexSize) )
        {
            if ( srcMat.getTexture(srcLevel) != NULL )
            {
                if ( !setTexture( srcMat.getTexture(srcLevel), dstLevel ) )
                {
                    _pOwner->getManagedConsole().EOLn("ERROR: %s setTexture() failed for dstLevel %d!", __FUNCTION__, dstLevel);
                    return false;
                }
            }

            memcpy( layers[dstLevel].pTexcoords, srcMat.pImpl->layers[srcLevel].pTexcoords, sizeof(TUVW) * layers[dstLevel].nTexcoords_h );
            memcpy( layers[dstLevel].pColors, srcMat.pImpl->layers[srcLevel].pColors, sizeof(TRGBAFLOAT) * layers[dstLevel].nColors_h );

            return true;
        }
        else
            ; // TODO: set lasterr
    }
    else
        ; // TODO: set lasterr

    return false;
}


TPureUInt PureMaterial::PureMaterialImpl::getUsedSystemMemory() const
{
    return (
        sizeof(*this) + layers.capacity() * sizeof(TPURE_MATERIAL_LAYER) );
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    @exception Any exception that might be thrown by std::vector::push_back(), in case of failure the exception is not handled but propagated to caller.
*/
PureMaterial::PureMaterialImpl::PureMaterialImpl(
    PureMaterial* owner, TPureUInt nLayers)
{
    _pOwner = owner;
    _pOwner->getManagedConsole().OLnOI("PureMaterial() ...");

    // sanity check
    if ( nLayers < 1 )
        nLayers = 1;

    for (TPureUInt i = 0; i < nLayers; i++)
    {
        layers.push_back( TPURE_MATERIAL_LAYER() );
        layers[ layers.size()-1 ].nColors_h = 0;
        layers[ layers.size()-1 ].pColors = PGENULL;
        layers[ layers.size()-1 ].nTexcoords_h = 0;
        layers[ layers.size()-1 ].pTexcoords = PGENULL;
        layers[ layers.size()-1 ].tex = PGENULL;
        layers[ layers.size()-1 ].texEnvMode = PURE_TEX_FUNC_MODULATE;
        layers[ layers.size()-1 ].fTransparancy = 1.0f;
        layers[ layers.size()-1 ].clrTexEnv.Set(255, 255, 255, 255);
        layers[ layers.size()-1 ].blendFactorSource = PURE_ONE;
        layers[ layers.size()-1 ].blendFactorDestination = PURE_ZERO;
    }

    nIndices = 0;
    fShininessBackFace = fShininessFrontFace = 1.0f;

    // we dont store nLayers because from now layers.size() is fix until this instance is deleted

    _pOwner->getManagedConsole().SOLnOO("Done!");
} // PureObject3D()


PureMaterial::PureMaterialImpl::PureMaterialImpl(const PureMaterialImpl&)
{

}


PureMaterial::PureMaterialImpl& PureMaterial::PureMaterialImpl::operator=(const PureMaterialImpl&)
{
    return *this;
}


// ############################### PRIVATE ###############################


/*
   PureMaterial
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureMaterial::~PureMaterial()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureTexture()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureMaterial::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureMaterial::getLoggerModuleName()
{
    return "PureMaterial";
} // getLoggerModuleName()


/**
    Gets whether the given source and destination blend factors really mean blending or not.
    If source factor is PURE_ONE and destination factor is PURE_ZERO, there's no blending even if blending is enabled.

    @return True if the given blend factors really mean blending, false otherwise.
*/
TPureBool PureMaterial::isBlendFuncReallyBlending(TPURE_BLENDFACTOR sfactor, TPURE_BLENDFACTOR dfactor)
{
    return !( (sfactor == PURE_ONE) && (dfactor == PURE_ZERO) );
} // isBlendFuncReallyBlending()


/**
    Allocate color-, texture coordinate-, and index arrays on all available levels.
    Although the size of color- and texture coordinate arrays may differ, the
    allocated color index arrays and texture coordinate index arrays will have same size since indexing is done per-vertex.

    @param nColorCount    Size of required color array on each level.
    @param nTexcoordCount Size of required texture coordinate array on each level.
    @param nIndexCount    Size of required index array on each level.
    @param nIndexSize     Size of an index in Bytes. Should be greater than 0.

    @return True if allocation succeeded or already allocated, false otherwise.
*/
TPureBool PureMaterial::allocateArrays(
                            TPureUInt nColorCount,
                            TPureUInt nTexcoordCount,
                            TPureUInt nIndexCount,
                            TPureUInt nIndexSize)
{
    return pImpl->allocateArrays(nColorCount, nTexcoordCount, nIndexCount, nIndexSize);
}


/**
    Gets the number of colors on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Number of colors on the specified layer, 0 on error.
*/
TPureUInt PureMaterial::getColorsCount(TPureUInt level) const
{
    return pImpl->getColorsCount(level);
} // getTexcoordsCount()


/**
    Gets the pointer to colors on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to colors on the specified layer, PGENULL on error.
*/
TRGBAFLOAT* PureMaterial::getColors(TPureUInt level)
{
    return pImpl->getColors(level);
} // getTexcoords()


/**
    Gets the pointer to colors on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to colors on the specified layer, PGENULL on error.
*/
const TRGBAFLOAT* PureMaterial::getColors(TPureUInt level) const
{
    return pImpl->getColors(level);
} // getTexcoords()


/**
    Gets the number of indices.

    @return Number of indices.
*/
TPureUInt PureMaterial::getIndicesCount() const
{
    return pImpl->getIndicesCount();
}


/**
    Gets the number of texture coordinates on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Number of texture coordinates on the specified layer, 0 on error.
*/
TPureUInt PureMaterial::getTexcoordsCount(TPureUInt level) const
{
    return pImpl->getTexcoordsCount(level);
} // getTexcoordsCount()


/**
    Gets the pointer to texture coordinates on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture coordinates on the specified layer, PGENULL on error.
*/
TUVW* PureMaterial::getTexcoords(TPureUInt level)
{
    return pImpl->getTexcoords(level);
} // getTexcoords()


/**
    Gets the pointer to texture coordinates on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture coordinates on the specified layer, PGENULL on error.
*/
const TUVW* PureMaterial::getTexcoords(TPureUInt level) const
{
    return pImpl->getTexcoords(level);
} // getTexcoords()


/**
    Gets the texture of the material on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture on the specified level, PGENULL on error or if no texture is assigned to this level.
*/
PureTexture* PureMaterial::getTexture(TPureUInt level)
{
    return pImpl->getTexture(level);
}


/**
    Gets the texture of the material on the specified level.
    0 is the default level, always used for single-textured materials.

    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture on the specified level, PGENULL on error or if no texture is assigned to this level.
*/
const PureTexture* PureMaterial::getTexture(TPureUInt level) const
{
    return pImpl->getTexture(level);
}


/**
    Sets the texture of the material on the specified level.
    0 is the default level, always used for single-textured materials.
    
    @param  tex   Texture we are defining for the specified level. Texture can be unassigned from the specified level by specifying NULL here.
    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
    @return True on success, false otherwise.
*/
TPureBool PureMaterial::setTexture(PureTexture* tex, TPureUInt level)
{
    return pImpl->setTexture(tex, level);
}


/**
    Gets the number of textures assigned to this material.

    @return Number of textures assigned to this material on all levels.
*/
TPureUInt PureMaterial::getTextureCount() const
{
    return pImpl->getTextureCount();
}


/**
    Gets whether the material has at least 1 textured layer.

    @return True if there is at least 1 textured layer, false otherwise.
*/
TPureBool PureMaterial::isTextured() const
{
    return pImpl->isTextured();
}


/**
    Gets whether the material has only 1 textured layer.

    @return True if there is only 1 textured layer, false otherwise.
*/
TPureBool PureMaterial::isSingleTextured() const
{
    return pImpl->isSingleTextured();
}


/**
    Gets whether the material has at least 2 textured layers.

    @return True if there is at least 2 textured layers, false otherwise.
*/
TPureBool PureMaterial::isMultiTextured() const
{
    return pImpl->isMultiTextured();
}


/**
    Gets the texture environment color of the material on the specified level.
    Texture environment color is used by the texture environment mode / function in producing fragment color and alpha values.
    The default texture environment color is white [255, 255, 255, 255].

    Texture environment mode / function is currently cannot be changed, it is PURE_TEX_FUNC_MODULATE by default, hence even setting texture environment color has no effect at all.

    0 is the default level, always used for single-textured materials.
    

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return The texture environment color of the material on the specified level. If level is invalid, then the color on the lowest level is returned.
*/
PureColor& PureMaterial::getTextureEnvColor(TPureUInt level)
{
    return pImpl->getTextureEnvColor(level);
}


/**
    Gets the texture environment color of the material on the specified level.
    Texture environment color is used by the texture environment mode / function in producing fragment color and alpha values.
    The default texture environment color is white [255, 255, 255, 255].

    Texture environment mode / function is currently cannot be changed, it is PURE_TEX_FUNC_MODULATE by default, hence even setting texture environment color has no effect at all.

    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return The texture environment color of the material on the specified level. If level is invalid, then the color on the lowest level is returned.
*/
const PureColor& PureMaterial::getTextureEnvColor(TPureUInt level) const
{
    return pImpl->getTextureEnvColor(level);
}


/**
    Gets the source blend factor on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Source blend factor on the specified level. If level is invalid, then the source blend factor on the lowest level is returned.
*/
TPURE_BLENDFACTOR PureMaterial::getSourceBlendFunc(TPureUInt level) const
{
    return pImpl->getSourceBlendFunc(level);
} // getSourceBlendFunc()


/**
    Sets the source blend factor on the specified level.
    0 is the default level.
    No effect if the given value is PURE_SRC_COLOR or PURE_ONE_MINUS_SRC_COLOR.
    No effect if the material doesn't have an utiliser entity (e.g. PureObject3D instance) or if the utiliser doesn't have its parent set (e.g. PureObject3DManager).

    @param value Source blend factor to be set on the specified level.
    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
    @return True on success, false otherwise.
*/
TPureBool PureMaterial::setSourceBlendFunc(TPURE_BLENDFACTOR value, TPureUInt level)
{
    return pImpl->setSourceBlendFunc(value, level);
} // setSourceBlendFunc()


/**
    Gets the destination blend factor on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Destination blend factor on the specified level. If level is invalid, then the destination blend factor on the lowest level is returned.
*/
TPURE_BLENDFACTOR PureMaterial::getDestinationBlendFunc(TPureUInt level) const
{
    return pImpl->getDestinationBlendFunc(level);
} // getDestinationBlendFunc()


/**
    Sets the destination blend factor on the specified level.
    0 is the default level.
    No effect if the given value is one of the following:
    PURE_DST_COLOR, PURE_ONE_MINUS_DST_COLOR, PURE_SRC_ALPHA_SATURATE. 
    No effect if the material doesn't have an utiliser entity (e.g. PureObject3D instance) or if the utiliser doesn't have its parent set (e.g. PureObject3DManager).

    @param value Destination blend factor to be set on the specified level.
    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
    @return True on success, false otherwise.
*/
TPureBool PureMaterial::setDestinationBlendFunc(TPURE_BLENDFACTOR value, TPureUInt level)
{
    return pImpl->setDestinationBlendFunc(value, level);
} // setDestinationBlendFunc()


/**
    Sets the blend factors on the specified level.
    0 is the default level.
    Equivalent to calling setSourceBlendFunc(src, level) and setDestinationBlendFunc(dst, level), so check their documentation as well for accepted values.
    See the description of those functions for more details about the cases when there is no effect when calling them.
    If you want to utilize transparency of a loaded 32 bit color depth bitmap, you have to make sure the bitmap has proper
    alpha values. Some image editing applications don't save proper alpha values, as explained at PureImageManager::createFromFile().
    For bitmaps not having proper alpha bits use (PURE_SRC_ALPHA, PURE_ONE) for src and dst respectively.
    For bitmaps having proper alpha bits use (PURE_SRC_ALPHA, PURE_ONE_MINUS_SRC_ALPHA) for src and dst respectively to utilize real alpha.

    @param src Source blend factor to be set on the specified level.
    @param dst Destination blend factor to be set on the specified level.
    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
    @return True on success, false otherwise.
*/
TPureBool PureMaterial::setBlendFuncs(TPURE_BLENDFACTOR src, TPURE_BLENDFACTOR dst, TPureUInt level)
{
    return pImpl->setBlendFuncs(src, dst, level);
} // setBlendFuncs()


/**
    Gets the blend mode on the specified level.
    0 is the default level.

    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Blend mode. It is PURE_BM_MANUAL if source and destination blend factor combination doesn't match any predefined blend mode.
            In case of invalid level, PURE_BM_NONE is returned.
*/
TPURE_BLENDMODE PureMaterial::getBlendMode(TPureUInt level) const
{
    return pImpl->getBlendMode(level);
} // getBlendMode()


/**
    Sets the blend mode on the specified level.
    0 is the default level.
    PURE_BM_MANUAL input is ignored, it is set by the engine when the blend factors do not match with a predefined blend mode.
    No effect if the material doesn't have an utiliser entity (e.g. PureObject3D instance) or if the utiliser doesn't have its parent set (e.g. PureObject3DManager).

    @param mode  The requested blend mode.
    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
    @return True on success, false otherwise.
*/
TPureBool PureMaterial::setBlendMode(TPURE_BLENDMODE mode, TPureUInt level)
{
    return pImpl->setBlendMode(mode, level);
} // setBlendMode()


/**
    Copies a layer of a material to a layer of this material.
    The array sizes of the 2 materials should match as array layout won't be changed by this method.

    @param srcMat   Material where we are copying a layer from.
    @param dstLevel Which layer are we copying from the source material.
    @param srcLevel Which layer are we copying to in destination material.

    @return True on success, false otherwise.
*/
TPureBool PureMaterial::copyFromMaterial(PureMaterial& srcMat, TPureUInt dstLevel, TPureUInt srcLevel)
{
    return pImpl->copyFromMaterial(srcMat, dstLevel, srcLevel);
}


/**
    Gets the amount of allocated system memory.

    @return Amount of allocated system memory in Bytes.
*/
TPureUInt PureMaterial::getUsedSystemMemory() const
{
    return PureFiledManaged::getUsedSystemMemory() - sizeof(PureFiledManaged) + sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    Only PureMaterialManager creates it.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
    @exception Any exception that might be thrown by std::vector::push_back(), in case of failure the exception is not handled but propagated to caller.
*/
PureMaterial::PureMaterial(TPureUInt nLayers)
{
    pImpl = new PureMaterialImpl(this, nLayers);
} // PureObject3D()


PureMaterial::PureMaterial(const PureMaterial&)
{
    // UNUSED
}


PureMaterial& PureMaterial::operator=(const PureMaterial&)
{
    // UNUSED
    return *this;
}


// ############################### PRIVATE ###############################



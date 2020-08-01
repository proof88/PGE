/*
    ###################################################################################
    PRREMaterial.cpp
    This file is part of PRRE.
    PRREMaterial class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREMaterialManager.h"
#include "PRREMaterialImpl.h"
#include "PRREhwInfo.h"

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
   PRREMaterial::PRREMaterialImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREMaterial::PRREMaterialImpl::~PRREMaterialImpl()
{
    getConsole().OLnOI("~PRREMaterial() ...");

    for (TPRREuint i = 0; i < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount(); i++)
    {
        layers[i].nColors_h = layers[i].nColorIndices_h = layers[i].nTexcoords_h = layers[i].nTexcoordIndices_h = 0;
        free( layers[i].pColors );
        free( layers[i].pColorIndices );
        free( layers[i].pTexcoords );
        free( layers[i].pTexcoordIndices );
        layers[i].pColors = PGENULL;
        layers[i].pColorIndices = PGENULL;
        layers[i].pTexcoords = PGENULL;
        layers[i].pTexcoordIndices = PGENULL;
        layers[i].nIndexSize = 0;
    }

    getConsole().SOLnOO("Done!");
} // ~PRRETexture()


void PRREMaterial::PRREMaterialImpl::AllocateArrays(TPRREuint nColorCount, TPRREuint nTexcoordCount, TPRREuint nIndexCount, TPRREuint nIndexSize)
{
    if ( nIndexSize == 0 )
    {
        getConsole().EOLn("PRREMaterial()::AllocateArrays(%d, %d, %d, %d) fail!", nColorCount, nTexcoordCount, nIndexCount, nIndexSize);
        return;
    }

    for (TPRREuint i = 0; i < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount(); i++)
    {
        layers[i].nColors_h          = nColorCount;
        layers[i].pColors            = (TRGBAFLOAT*) malloc( sizeof(TRGBAFLOAT) * nColorCount );
        layers[i].nColorIndices_h    = nIndexCount;
        layers[i].pColorIndices      = malloc( nIndexSize * nIndexCount );
        layers[i].nTexcoords_h       = nTexcoordCount;
        layers[i].pTexcoords         = (TUVW*) malloc( sizeof(TUVW) * nTexcoordCount );
        layers[i].nTexcoordIndices_h = nIndexCount;
        layers[i].pTexcoordIndices   = malloc( nIndexSize * nIndexCount );
        layers[i].nIndexSize         = nIndexSize;
    }
}


TPRREuint PRREMaterial::PRREMaterialImpl::getColorsCount(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].nColors_h;
    else
    {
        // TODO: set lasterr
        return 0;
    }
} // getTexcoordsCount()


TRGBAFLOAT* PRREMaterial::PRREMaterialImpl::getColors(TPRREuint level)
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pColors;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
} // getTexcoords()


const TRGBAFLOAT* PRREMaterial::PRREMaterialImpl::getColors(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pColors;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
} // getTexcoords()


TPRREuint PRREMaterial::PRREMaterialImpl::getColorIndicesCount(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].nColorIndices_h;
    else
    {
        // TODO: set lasterr
        return 0;
    }
}


void* PRREMaterial::PRREMaterialImpl::getColorIndices(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pColorIndices;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
}


TPRREuint PRREMaterial::PRREMaterialImpl::getTexcoordsCount(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].nTexcoords_h;
    else
    {
        // TODO: set lasterr
        return 0;
    }
} // getTexcoordsCount()


TUVW* PRREMaterial::PRREMaterialImpl::getTexcoords(TPRREuint level)
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pTexcoords;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
} // getTexcoords()


const TUVW* PRREMaterial::PRREMaterialImpl::getTexcoords(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pTexcoords;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
} // getTexcoords()


TPRREuint PRREMaterial::PRREMaterialImpl::getTexcoordIndicesCount(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].nTexcoordIndices_h;
    else
    {
        // TODO: set lasterr
        return 0;
    }
}


void* PRREMaterial::PRREMaterialImpl::getTexcoordIndices(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].pTexcoordIndices;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
}


PRRETexture* PRREMaterial::PRREMaterialImpl::getTexture(TPRREuint level)
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].tex;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
}


const PRRETexture* PRREMaterial::PRREMaterialImpl::getTexture(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].tex;
    else
    {
        // TODO: set lasterr
        return PGENULL;
    }
}


void PRREMaterial::PRREMaterialImpl::SetTexture(PRRETexture* tex, TPRREuint level)
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        layers[level].tex = tex;
    else
        ; // TODO: set lasterr
}


TPRREuint PRREMaterial::PRREMaterialImpl::getTextureCount() const
{
    TPRREuint nFound = 0;
    for (TPRREuint i = 0; i < layers.size(); i++)
        if ( layers[i].tex != PGENULL )
            nFound++;

    return nFound;
}


TPRREbool PRREMaterial::PRREMaterialImpl::isTextured() const
{
    return getTextureCount() > 0;
}


TPRREbool PRREMaterial::PRREMaterialImpl::isSingleTextured() const
{
    return getTextureCount() == 1;
}


TPRREbool PRREMaterial::PRREMaterialImpl::isMultiTextured() const
{
    return getTextureCount() > 1;
}


PRREColor& PRREMaterial::PRREMaterialImpl::getTextureEnvColor(TPRREuint level)
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].clrTexEnv;
    else
    {
        // TODO: set lasterr
        return layers[0].clrTexEnv;
    } 
}


const PRREColor& PRREMaterial::PRREMaterialImpl::getTextureEnvColor(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].clrTexEnv;
    else
    {
        // TODO: set lasterr
        return layers[0].clrTexEnv;
    }
}


TPRRE_BLENDFACTORS PRREMaterial::PRREMaterialImpl::getSourceBlendFunc(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].blendFactorSource;
    else
    {
        // TODO: set lasterr
        return layers[0].blendFactorSource;
    }
} // getSourceBlendFunc()


void PRREMaterial::PRREMaterialImpl::SetSourceBlendFunc(TPRRE_BLENDFACTORS value, TPRREuint level)
{
    if ( (value != PRRE_SRC_COLOR) && (value != PRRE_ONE_MINUS_SRC_COLOR) )
    {
        if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
            layers[level].blendFactorSource = value;
        else
            ; // TODO: set lasterr
    }
} // SetSourceBlendFunc()


TPRRE_BLENDFACTORS PRREMaterial::PRREMaterialImpl::getDestinationBlendFunc(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
        return layers[level].blendFactorDestination;
    else
    {
        // TODO: set lasterr
        return layers[0].blendFactorDestination;
    }
} // getDestinationBlendFunc()


void PRREMaterial::PRREMaterialImpl::SetDestinationBlendFunc(TPRRE_BLENDFACTORS value, TPRREuint level)
{
    if ( (value != PRRE_DST_COLOR) && (value != PRRE_ONE_MINUS_DST_COLOR) && (value != PRRE_SRC_ALPHA_SATURATE) )
    {
        if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
            layers[level].blendFactorDestination = value;
        else
            ; // TODO: set lasterr
    }
} // SetDestinationBlendFunc()


void PRREMaterial::PRREMaterialImpl::SetBlendFuncs(TPRRE_BLENDFACTORS src, TPRRE_BLENDFACTORS dst, TPRREuint level)
{
    SetSourceBlendFunc( src, level );
    SetDestinationBlendFunc( dst, level );
} // SetBlendFuncs()


TPRRE_BLENDMODES PRREMaterial::PRREMaterialImpl::getBlendMode(TPRREuint level) const
{
    if ( level < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount() )
    {
        if ( (layers[level].blendFactorSource == PRRE_ONE) && (layers[level].blendFactorDestination == PRRE_ZERO) )
            return PRRE_BM_NONE;
        else if ( (layers[level].blendFactorSource == PRRE_SRC_ALPHA) && (layers[level].blendFactorDestination == PRRE_ONE_MINUS_SRC_ALPHA) )
            return PRRE_BM_STANDARD_TRANSPARENCY;
        else
            return PRRE_BM_MANUAL;
    }
    else
    {
        // TODO: set lasterr
        return PRRE_BM_NONE;
    }
    
} // getBlendMode()


void PRREMaterial::PRREMaterialImpl::SetBlendMode(TPRRE_BLENDMODES mode, TPRREuint level)
{
    switch (mode)
    {
    case PRRE_BM_NONE:
        SetBlendFuncs(PRRE_ONE, PRRE_ZERO, level);
        break;
    case PRRE_BM_STANDARD_TRANSPARENCY:
        SetBlendFuncs(PRRE_SRC_ALPHA, PRRE_ONE_MINUS_SRC_ALPHA, level);
        break;
    default:
        break;
    }
} // SetBlendMode()


TPRREbool PRREMaterial::PRREMaterialImpl::copyFromMaterial(PRREMaterial& srcMat, TPRREuint dstLevel, TPRREuint srcLevel)
{
    if ( (dstLevel < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount()) && 
         (srcLevel < ((PRREMaterialManager*)_pOwner->getManager())->getMaximumLayerCount()) )
    {
        // we also want to make sure that we have the same memory layout of texcoords and indices
        // so we can safely copy data into our preallocated arrays
        if ( (layers[dstLevel].nTexcoords_h == srcMat.p->layers[srcLevel].nTexcoords_h) &&
             (layers[dstLevel].nTexcoordIndices_h == srcMat.p->layers[srcLevel].nTexcoordIndices_h) &&
             (layers[dstLevel].nColors_h == srcMat.p->layers[srcLevel].nColors_h) &&
             (layers[dstLevel].nColorIndices_h == srcMat.p->layers[srcLevel].nColorIndices_h) &&
             (layers[dstLevel].nIndexSize == srcMat.p->layers[srcLevel].nIndexSize) )
        {
            if ( srcMat.getTexture(srcLevel) != NULL )
                SetTexture( srcMat.getTexture(srcLevel), dstLevel );

            memcpy( layers[dstLevel].pTexcoords, srcMat.p->layers[srcLevel].pTexcoords, sizeof(TUVW) * layers[dstLevel].nTexcoords_h );
            memcpy( layers[dstLevel].pTexcoordIndices, srcMat.p->layers[srcLevel].pTexcoordIndices, layers[dstLevel].nIndexSize * layers[dstLevel].nTexcoordIndices_h );

            memcpy( layers[dstLevel].pColors, srcMat.p->layers[srcLevel].pColors, sizeof(TRGBAFLOAT) * layers[dstLevel].nColors_h );
            memcpy( layers[dstLevel].pColorIndices, srcMat.p->layers[srcLevel].pColorIndices, layers[dstLevel].nIndexSize * layers[dstLevel].nColorIndices_h );

            return true;
        }
        else
            ; // TODO: set lasterr
    }
    else
        ; // TODO: set lasterr

    return false;
}


TPRREuint PRREMaterial::PRREMaterialImpl::getUsedSystemMemory() const
{
    return (
        sizeof(*this) + layers.capacity() * sizeof(TPRRE_MATERIAL_LAYER) );
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


PRREMaterial::PRREMaterialImpl::PRREMaterialImpl(
    PRREMaterial* owner, TPRREuint nLayers)
{
    _pOwner = owner;
    getConsole().OLnOI("PRREMaterial() ...");

    // sanity check
    if ( nLayers < 1 )
        nLayers = 1;

    for (TPRREuint i = 0; i < nLayers; i++)
    {
        layers.push_back( TPRRE_MATERIAL_LAYER() );
        layers[ layers.size()-1 ].nColors_h = 0;
        layers[ layers.size()-1 ].nColorIndices_h = 0;
        layers[ layers.size()-1 ].pColors = PGENULL;
        layers[ layers.size()-1 ].pColorIndices = PGENULL;
        layers[ layers.size()-1 ].nTexcoords_h = 0;
        layers[ layers.size()-1 ].nTexcoordIndices_h = 0;
        layers[ layers.size()-1 ].pTexcoords = PGENULL;
        layers[ layers.size()-1 ].pTexcoordIndices = PGENULL;
        layers[ layers.size()-1 ].tex = PGENULL;
        layers[ layers.size()-1 ].texEnvMode = PRRE_TEX_FUNC_MODULATE;
        layers[ layers.size()-1 ].fTransparancy = 1.0f;
        layers[ layers.size()-1 ].clrTexEnv.Set(255, 255, 255, 255);
        layers[ layers.size()-1 ].blendFactorSource = PRRE_ONE;
        layers[ layers.size()-1 ].blendFactorDestination = PRRE_ZERO;
    }

    fShininessBackFace = fShininessFrontFace = 1.0f;

    // we dont store nLayers because from now layers.size() is fix until this instance is deleted

    getConsole().SOLnOO("Done!");
} // PRREObject3D()


PRREMaterial::PRREMaterialImpl::PRREMaterialImpl(const PRREMaterialImpl&)
{

}


PRREMaterial::PRREMaterialImpl& PRREMaterial::PRREMaterialImpl::operator=(const PRREMaterialImpl&)
{
    return *this;
}


// ############################### PRIVATE ###############################


/**
    Hack to be able to use CConsole singleton instance instead of owner's protected console instance.
    TODO: maybe the whole getConsole() stuff should be removed from everywhere because CConsole is a singleton class after all.
*/
CConsole& PRREMaterial::PRREMaterialImpl::getConsole() const
{
    return CConsole::getConsoleInstance();
}


/*
   PRREMaterial
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREMaterial::~PRREMaterial()
{
    delete p;
    p = NULL;
} // ~PRRETexture()


/**
    Allocate color-, texture coordinate-, and index arrays on all available levels.
    Although the size of color- and texture coordinate arrays may differ, the
    allocated color index arrays and texture coordinate index arrays will have same size since indexing is done per-vertex.

    @param nColorCount    Size of required color array on each level.
    @param nTexcoordCount Size of required texture coordinate array on each level.
    @param nIndexCount    Size of required index array on each level.
    @param nIndexSize     Size of an index in Bytes. Should be greater than 0.
*/
void PRREMaterial::AllocateArrays(
                            TPRREuint nColorCount,
                            TPRREuint nTexcoordCount,
                            TPRREuint nIndexCount,
                            TPRREuint nIndexSize)
{
    p->AllocateArrays(nColorCount, nTexcoordCount, nIndexCount, nIndexSize);
}


/**
    Gets the number of colors on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Number of colors on the specified layer, 0 on error.
*/
TPRREuint PRREMaterial::getColorsCount(TPRREuint level) const
{
    return p->getColorsCount(level);
} // getTexcoordsCount()


/**
    Gets the pointer to colors on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to colors on the specified layer, PGENULL on error.
*/
TRGBAFLOAT* PRREMaterial::getColors(TPRREuint level)
{
    return p->getColors(level);
} // getTexcoords()


/**
    Gets the pointer to colors on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to colors on the specified layer, PGENULL on error.
*/
const TRGBAFLOAT* PRREMaterial::getColors(TPRREuint level) const
{
    return p->getColors(level);
} // getTexcoords()


/**
    Gets the number of color indices on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Number of color indices on the specified layer, 0 on error.
*/
TPRREuint PRREMaterial::getColorIndicesCount(TPRREuint level) const
{
    return p->getColorIndicesCount(level);
}


/**
    Gets the pointer to color indices on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to color indices on the specified layer, PGENULL on error.
*/
void* PRREMaterial::getColorIndices(TPRREuint level) const
{
    return p->getColorIndices(level);
}


/**
    Gets the number of texture coordinates on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Number of texture coordinates on the specified layer, 0 on error.
*/
TPRREuint PRREMaterial::getTexcoordsCount(TPRREuint level) const
{
    return p->getTexcoordsCount(level);
} // getTexcoordsCount()


/**
    Gets the pointer to texture coordinates on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture coordinates on the specified layer, PGENULL on error.
*/
TUVW* PRREMaterial::getTexcoords(TPRREuint level)
{
    return p->getTexcoords(level);
} // getTexcoords()


/**
    Gets the pointer to texture coordinates on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture coordinates on the specified layer, PGENULL on error.
*/
const TUVW* PRREMaterial::getTexcoords(TPRREuint level) const
{
    return p->getTexcoords(level);
} // getTexcoords()


/**
    Gets the number of texture coordinate indices on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Number of texture coordinate indices on the specified layer, 0 on error.
*/
TPRREuint PRREMaterial::getTexcoordIndicesCount(TPRREuint level) const
{
    return p->getTexcoordIndicesCount(level);
}


/**
    Gets the pointer to texture coordinate indices on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture coordinate indices on the specified layer, PGENULL on error.
*/
void* PRREMaterial::getTexcoordIndices(TPRREuint level) const
{
    return p->getTexcoordIndices(level);
}


/**
    Gets the texture of the material on the specified level.
    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture on the specified level, PGENULL on error or if no texture is assigned to this level.
*/
PRRETexture* PRREMaterial::getTexture(TPRREuint level)
{
    return p->getTexture(level);
}


/**
    Gets the texture of the material on the specified level.
    0 is the default level, always used for single-textured materials.

    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Pointer to texture on the specified level, PGENULL on error or if no texture is assigned to this level.
*/
const PRRETexture* PRREMaterial::getTexture(TPRREuint level) const
{
    return p->getTexture(level);
}


/**
    Sets the texture of the material on the specified level.
    0 is the default level, always used for single-textured materials.
    
    @param  tex   Texture we are defining for the specified level. Texture can be unassigned from the specified level by specifying NULL here.
    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
*/
void PRREMaterial::SetTexture(PRRETexture* tex, TPRREuint level)
{
    p->SetTexture(tex, level);
}


/**
    Gets the number of textures assigned to this material.

    @return Number of textures assigned to this material on all levels.
*/
TPRREuint PRREMaterial::getTextureCount() const
{
    return p->getTextureCount();
}


/**
    Gets whether the material has at least 1 textured layer.

    @return True if there is at least 1 textured layer, false otherwise.
*/
TPRREbool PRREMaterial::isTextured() const
{
    return p->isTextured();
}


/**
    Gets whether the material has only 1 textured layer.

    @return True if there is only 1 textured layer, false otherwise.
*/
TPRREbool PRREMaterial::isSingleTextured() const
{
    return p->isSingleTextured();
}


/**
    Gets whether the material has at least 2 textured layers.

    @return True if there is at least 2 textured layers, false otherwise.
*/
TPRREbool PRREMaterial::isMultiTextured() const
{
    return p->isMultiTextured();
}


/**
    Gets the texture environment color of the material on the specified level.
    Texture environment color is used by the texture environment mode / function in producing fragment color and alpha values.
    The default texture environment color is white [255, 255, 255, 255].

    Texture environment mode / function is currently cannot be changed, it is PRRE_TEX_FUNC_MODULATE by default, hence even setting texture environment color has no effect at all.

    0 is the default level, always used for single-textured materials.
    

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return The texture environment color of the material on the specified level. If level is invalid, then the color on the lowest level is returned.
*/
PRREColor& PRREMaterial::getTextureEnvColor(TPRREuint level)
{
    return p->getTextureEnvColor(level);
}


/**
    Gets the texture environment color of the material on the specified level.
    Texture environment color is used by the texture environment mode / function in producing fragment color and alpha values.
    The default texture environment color is white [255, 255, 255, 255].

    Texture environment mode / function is currently cannot be changed, it is PRRE_TEX_FUNC_MODULATE by default, hence even setting texture environment color has no effect at all.

    0 is the default level, always used for single-textured materials.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return The texture environment color of the material on the specified level. If level is invalid, then the color on the lowest level is returned.
*/
const PRREColor& PRREMaterial::getTextureEnvColor(TPRREuint level) const
{
    return p->getTextureEnvColor(level);
}


/**
    Gets the source blend factor on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Source blend factor on the specified level. If level is invalid, then the source blend factor on the lowest level is returned.
*/
TPRRE_BLENDFACTORS PRREMaterial::getSourceBlendFunc(TPRREuint level) const
{
    return p->getSourceBlendFunc(level);
} // getSourceBlendFunc()


/**
    Sets the source blend factor on the specified level.
    0 is the default level.
    No effect if the given value is PRRE_SRC_COLOR or PRRE_ONE_MINUS_SRC_COLOR.

    @param value Source blend factor to be set on the specified level.
    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
*/
void PRREMaterial::SetSourceBlendFunc(TPRRE_BLENDFACTORS value, TPRREuint level)
{
    p->SetSourceBlendFunc(value, level);
} // SetSourceBlendFunc()


/**
    Gets the destination blend factor on the specified level.
    0 is the default level.

    @param  level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Destination blend factor on the specified level. If level is invalid, then the destination blend factor on the lowest level is returned.
*/
TPRRE_BLENDFACTORS PRREMaterial::getDestinationBlendFunc(TPRREuint level) const
{
    return p->getDestinationBlendFunc(level);
} // getDestinationBlendFunc()


/**
    Sets the destination blend factor on the specified level.
    0 is the default level.
    No effect if the given value is one of the following:
    PRRE_DST_COLOR, PRRE_ONE_MINUS_DST_COLOR, PRRE_SRC_ALPHA_SATURATE. 

    @param value Destination blend factor to be set on the specified level.
    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
*/
void PRREMaterial::SetDestinationBlendFunc(TPRRE_BLENDFACTORS value, TPRREuint level)
{
    p->SetDestinationBlendFunc(value, level);
} // SetDestinationBlendFunc()


/**
    Sets the blend factors on the specified level.
    0 is the default level.
    Equivalent to calling SetSourceBlendFunc(src, level) and SetDestinationBlendFunc(dst, level), so check their documentation as well for accepted values.

    @param src Source blend factor to be set on the specified level.
    @param dst Destination blend factor to be set on the specified level.
    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
*/
void PRREMaterial::SetBlendFuncs(TPRRE_BLENDFACTORS src, TPRRE_BLENDFACTORS dst, TPRREuint level)
{
    p->SetBlendFuncs(src, dst, level);
} // SetBlendFuncs()


/**
    Gets the blend mode on the specified level.
    0 is the default level.

    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().

    @return Blend mode. It is PRRE_BM_MANUAL if source and destination blend factor combination doesn't match any predefined blend mode.
            In case of invalid level, PRRE_BM_NONE is returned.
*/
TPRRE_BLENDMODES PRREMaterial::getBlendMode(TPRREuint level) const
{
    return p->getBlendMode(level);
} // getBlendMode()


/**
    Sets the blend mode on the specified level.
    0 is the default level.
    PRRE_BM_MANUAL input is ignored.

    @param mode  The requested blend mode.
    @param level The material level/layer we are interested in. Should be less than MaterialManager::getMaximumLayerCount().
*/
void PRREMaterial::SetBlendMode(TPRRE_BLENDMODES mode, TPRREuint level)
{
    p->SetBlendMode(mode, level);
} // SetBlendMode()


/**
    Copies a layer of a material to a layer of this material.
    The array sizes of the 2 materials should match as array layout won't be changed by this method.

    @param srcMat   Material where we are copying a layer from.
    @param dstLevel Which layer are we copying from the source material.
    @param srcLevel Which layer are we copying to in destination material.

    @return True on success, false otherwise.
*/
TPRREbool PRREMaterial::copyFromMaterial(PRREMaterial& srcMat, TPRREuint dstLevel, TPRREuint srcLevel)
{
    return p->copyFromMaterial(srcMat, dstLevel, srcLevel);
}


/**
    Gets the amount of allocated system memory.

    @return Amount if allocated system memory in Bytes.
*/
TPRREuint PRREMaterial::getUsedSystemMemory() const
{
    return PRREFiledManaged::getUsedSystemMemory() - sizeof(PRREFiledManaged) + sizeof(*this) + p->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    Only PRREMaterialManager creates it.
*/
PRREMaterial::PRREMaterial(TPRREuint nLayers)
{
    p = new PRREMaterialImpl(this, nLayers);
} // PRREObject3D()


PRREMaterial::PRREMaterial(const PRREMaterial&)
{

}


PRREMaterial& PRREMaterial::operator=(const PRREMaterial&)
{
    return *this;
}


// ############################### PRIVATE ###############################



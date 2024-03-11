#pragma once

/*
    ###################################################################################
    PureMaterialImpl.h
    This file is part of PURE.
    Internal header.
    PureMaterialImpl class.
    Made by PR00F88
    ###################################################################################
*/

#include <vector>
#include "../../external/PureAllHeaders.h"
#include "../../external/Material/PureMaterialManager.h"

/**
    Defines a layer of a material.
    Maximum number of layers per material is limited by the number of TMUs on current hardware.
    Single-textured objects need 1-layer material.
    Multi-textured objects need a material of 2 or more layers.
*/
struct TPURE_MATERIAL_LAYER
{
    TRGBAFLOAT*    pColors;             /**< Pointer to colors. */
    TPureUInt      nColors_h;           /**< Number of colors. Note it is actually the same on all levels. In the future it could be moved from layer to material level. */
    PureTexture*   tex;                 /**< Texture on layer. */
    TUVW*          pTexcoords;          /**< Pointer to texture coordinates. */
    TPureUInt      nTexcoords_h;        /**< Number of texture coordinates. Note it is actually the same on all levels. In the future it could be moved from layer to material level.*/
	PureColor      clrTexEnv;           /**< Texture environment color. */
	TPURE_TEX_FUNC texEnvMode;          /**< Texture environment mode / function. */
	TPureFloat     fTransparancy;       /**< Layer transparency. */
    TPURE_BLENDFACTOR blendFactorSource;       /**< Source blend factor. */
    TPURE_BLENDFACTOR blendFactorDestination;  /**< Destination blend factor.  */
};

class PureMaterial::PureMaterialImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual              ~PureMaterialImpl();

            TPureBool    allocateArrays(
                            TPureUInt nColorCount,
                            TPureUInt nTexcoordCount,
                            TPureUInt nIndexCount,
                            TPureUInt nIndexSize);

            TPureUInt    getColorsCount(TPureUInt level = 0) const;     
            TRGBAFLOAT*  getColors(TPureUInt level = 0);                
    const   TRGBAFLOAT*  getColors(TPureUInt level = 0) const;
            TPureUInt    getIndicesCount() const;

            TPureUInt    getTexcoordsCount(TPureUInt level = 0) const;     
            TUVW*        getTexcoords(TPureUInt level = 0);                
    const   TUVW*        getTexcoords(TPureUInt level = 0) const;

            TPureUInt    getTextureCount() const;
            PureTexture* getTexture(TPureUInt level = 0);
    const   PureTexture* getTexture(TPureUInt level = 0) const;
            TPureBool    setTexture(PureTexture* tex, TPureUInt level = 0);

            TPureBool    isTextured() const;
            TPureBool    isSingleTextured() const;
            TPureBool    isMultiTextured() const;

            PureColor&   getTextureEnvColor(TPureUInt level = 0);
    const   PureColor&   getTextureEnvColor(TPureUInt level = 0) const;

            TPURE_BLENDFACTOR getSourceBlendFunc(TPureUInt level = 0) const;
            TPureBool          setSourceBlendFunc(
                                TPURE_BLENDFACTOR value,
                                TPureUInt level = 0);

            TPURE_BLENDFACTOR getDestinationBlendFunc(TPureUInt level = 0) const;
            TPureBool          setDestinationBlendFunc(
                                TPURE_BLENDFACTOR value,
                                TPureUInt level = 0);

            TPureBool          setBlendFuncs(
                                TPURE_BLENDFACTOR src,
                                TPURE_BLENDFACTOR dst,
                                TPureUInt level = 0);

            TPURE_BLENDMODE   getBlendMode(TPureUInt level = 0) const;
            TPureBool          setBlendMode(
                                TPURE_BLENDMODE mode,
                                TPureUInt level = 0);

            TPureBool    copyFromMaterial(
                            PureMaterial& srcMat,
                            TPureUInt dstLevel,
                            TPureUInt srcLevel);

            TPureUInt    getUsedSystemMemory() const; 
                

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    explicit PureMaterialImpl(
        PureMaterial* owner, TPureUInt nLayers = 1) /* TODO: mark this as noexcept(false) when using newer compiler! */;
    
    PureMaterialImpl(const PureMaterialImpl&);
    PureMaterialImpl& operator=(const PureMaterialImpl&);

private:
    PureMaterial* _pOwner;            /**< The owner public object who creates this pimpl object. */

    TPureUInt   nIndices;             /**< Number of indices. */
    TPureUInt   m_nIndexSize;         /**< Size of an index. */
    PureColor   clrAmbientFrontFace;
    PureColor   clrAmbientBackFace;
    PureColor   clrDiffuseFrontFace;
    PureColor   clrDiffuseBackFace;
    PureColor   clrSpecularFrontFace;
    PureColor   clrSpecularBackFace;
    PureColor   clrEmissionFrontFace;
    PureColor   clrEmissionBackFace;
    TPureFloat  fShininessFrontFace;
    TPureFloat  fShininessBackFace;
    std::vector<TPURE_MATERIAL_LAYER> layers;

    // ---------------------------------------------------------------------------

    friend class PureMaterial;
    friend class PureMaterialManager;

}; 

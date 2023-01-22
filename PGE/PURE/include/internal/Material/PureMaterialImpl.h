#pragma once

/*
    ###################################################################################
    PureMaterialImpl.h
    This file is part of PURE.
    Internal header.
    PureMaterialImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
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
struct TPure_MATERIAL_LAYER
{
    TRGBAFLOAT*    pColors;             /**< Pointer to colors. */
    TPureuint      nColors_h;           /**< Number of colors. Note it is actually the same on all levels. In the future it could be moved from layer to material level. */
    PureTexture*   tex;                 /**< Texture on layer. */
    TUVW*          pTexcoords;          /**< Pointer to texture coordinates. */
    TPureuint      nTexcoords_h;        /**< Number of texture coordinates. Note it is actually the same on all levels. In the future it could be moved from layer to material level.*/
	PureColor      clrTexEnv;           /**< Texture environment color. */
	TPure_TEX_FUNC texEnvMode;          /**< Texture environment mode / function. */
	TPurefloat     fTransparancy;       /**< Layer transparency. */
    TPure_BLENDFACTOR blendFactorSource;       /**< Source blend factor. */
    TPure_BLENDFACTOR blendFactorDestination;  /**< Destination blend factor.  */
};

class PureMaterial::PureMaterialImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual              ~PureMaterialImpl();

            TPurebool    allocateArrays(
                            TPureuint nColorCount,
                            TPureuint nTexcoordCount,
                            TPureuint nIndexCount,
                            TPureuint nIndexSize);

            TPureuint    getColorsCount(TPureuint level = 0) const;     
            TRGBAFLOAT*  getColors(TPureuint level = 0);                
    const   TRGBAFLOAT*  getColors(TPureuint level = 0) const;
            TPureuint    getIndicesCount() const;

            TPureuint    getTexcoordsCount(TPureuint level = 0) const;     
            TUVW*        getTexcoords(TPureuint level = 0);                
    const   TUVW*        getTexcoords(TPureuint level = 0) const;

            TPureuint    getTextureCount() const;
            PureTexture* getTexture(TPureuint level = 0);
    const   PureTexture* getTexture(TPureuint level = 0) const;
            TPurebool    setTexture(PureTexture* tex, TPureuint level = 0);

            TPurebool    isTextured() const;
            TPurebool    isSingleTextured() const;
            TPurebool    isMultiTextured() const;

            PureColor&   getTextureEnvColor(TPureuint level = 0);
    const   PureColor&   getTextureEnvColor(TPureuint level = 0) const;

            TPure_BLENDFACTOR getSourceBlendFunc(TPureuint level = 0) const;
            TPurebool          setSourceBlendFunc(
                                TPure_BLENDFACTOR value,
                                TPureuint level = 0);

            TPure_BLENDFACTOR getDestinationBlendFunc(TPureuint level = 0) const;
            TPurebool          setDestinationBlendFunc(
                                TPure_BLENDFACTOR value,
                                TPureuint level = 0);

            TPurebool          setBlendFuncs(
                                TPure_BLENDFACTOR src,
                                TPure_BLENDFACTOR dst,
                                TPureuint level = 0);

            TPure_BLENDMODE   getBlendMode(TPureuint level = 0) const;
            TPurebool          setBlendMode(
                                TPure_BLENDMODE mode,
                                TPureuint level = 0);

            TPurebool    copyFromMaterial(
                            PureMaterial& srcMat,
                            TPureuint dstLevel,
                            TPureuint srcLevel);

            TPureuint    getUsedSystemMemory() const; 
                

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    explicit PureMaterialImpl(
        PureMaterial* owner, TPureuint nLayers = 1) /* TODO: mark this as noexcept(false) when using newer compiler! */;
    
    PureMaterialImpl(const PureMaterialImpl&);
    PureMaterialImpl& operator=(const PureMaterialImpl&);

private:
    PureMaterial* _pOwner;            /**< The owner public object who creates this pimpl object. */

    TPureuint   nIndices;             /**< Number of indices. */
    TPureuint   m_nIndexSize;         /**< Size of an index. */
    PureColor   clrAmbientFrontFace;
    PureColor   clrAmbientBackFace;
    PureColor   clrDiffuseFrontFace;
    PureColor   clrDiffuseBackFace;
    PureColor   clrSpecularFrontFace;
    PureColor   clrSpecularBackFace;
    PureColor   clrEmissionFrontFace;
    PureColor   clrEmissionBackFace;
    TPurefloat  fShininessFrontFace;
    TPurefloat  fShininessBackFace;
    std::vector<TPure_MATERIAL_LAYER> layers;

    // ---------------------------------------------------------------------------

    friend class PureMaterial;
    friend class PureMaterialManager;

}; 

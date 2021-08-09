#pragma once

/*
    ###################################################################################
    PRREMaterialImpl.h
    This file is part of PRRE.
    Internal header.
    PRREMaterialImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <vector>
#include "../../external/PRREallHeaders.h"
#include "../../external/Material/PRREMaterialManager.h"

/**
    Defines a layer of a material.
    Maximum number of layers per material is limited by the number of TMUs on current hardware.
    Single-textured objects need 1-layer material.
    Multi-textured objects need a material of 2 or more layers.
*/
struct TPRRE_MATERIAL_LAYER
{
    TRGBAFLOAT*    pColors;             /**< Pointer to colors. */
    TPRREuint      nColors_h;           /**< Number of colors. Note it is actually the same on all levels. In the future it could be moved from layer to material level. */
    PRRETexture*   tex;                 /**< Texture on layer. */
    TUVW*          pTexcoords;          /**< Pointer to texture coordinates. */
    TPRREuint      nTexcoords_h;        /**< Number of texture coordinates. Note it is actually the same on all levels. In the future it could be moved from layer to material level.*/
	PRREColor      clrTexEnv;           /**< Texture environment color. */
	TPRRE_TEX_FUNC texEnvMode;          /**< Texture environment mode / function. */
	TPRREfloat     fTransparancy;       /**< Layer transparency. */
    TPRRE_BLENDFACTOR blendFactorSource;       /**< Source blend factor. */
    TPRRE_BLENDFACTOR blendFactorDestination;  /**< Destination blend factor.  */
};

class PRREMaterial::PRREMaterialImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual              ~PRREMaterialImpl();

            TPRREbool    allocateArrays(
                            TPRREuint nColorCount,
                            TPRREuint nTexcoordCount,
                            TPRREuint nIndexCount,
                            TPRREuint nIndexSize);

            TPRREuint    getColorsCount(TPRREuint level = 0) const;     
            TRGBAFLOAT*  getColors(TPRREuint level = 0);                
    const   TRGBAFLOAT*  getColors(TPRREuint level = 0) const;
            TPRREuint    getIndicesCount() const;

            TPRREuint    getTexcoordsCount(TPRREuint level = 0) const;     
            TUVW*        getTexcoords(TPRREuint level = 0);                
    const   TUVW*        getTexcoords(TPRREuint level = 0) const;

            TPRREuint    getTextureCount() const;
            PRRETexture* getTexture(TPRREuint level = 0);
    const   PRRETexture* getTexture(TPRREuint level = 0) const;
            TPRREbool    setTexture(PRRETexture* tex, TPRREuint level = 0);

            TPRREbool    isTextured() const;
            TPRREbool    isSingleTextured() const;
            TPRREbool    isMultiTextured() const;

            PRREColor&   getTextureEnvColor(TPRREuint level = 0);
    const   PRREColor&   getTextureEnvColor(TPRREuint level = 0) const;

            TPRRE_BLENDFACTOR getSourceBlendFunc(TPRREuint level = 0) const;
            TPRREbool          setSourceBlendFunc(
                                TPRRE_BLENDFACTOR value,
                                TPRREuint level = 0);

            TPRRE_BLENDFACTOR getDestinationBlendFunc(TPRREuint level = 0) const;
            TPRREbool          setDestinationBlendFunc(
                                TPRRE_BLENDFACTOR value,
                                TPRREuint level = 0);

            TPRREbool          setBlendFuncs(
                                TPRRE_BLENDFACTOR src,
                                TPRRE_BLENDFACTOR dst,
                                TPRREuint level = 0);

            TPRRE_BLENDMODE   getBlendMode(TPRREuint level = 0) const;
            TPRREbool          setBlendMode(
                                TPRRE_BLENDMODE mode,
                                TPRREuint level = 0);

            TPRREbool    copyFromMaterial(
                            PRREMaterial& srcMat,
                            TPRREuint dstLevel,
                            TPRREuint srcLevel);

            TPRREuint    getUsedSystemMemory() const; 
                

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    explicit PRREMaterialImpl(
        PRREMaterial* owner, TPRREuint nLayers = 1) /* TODO: mark this as noexcept(false) when using newer compiler! */;
    
    PRREMaterialImpl(const PRREMaterialImpl&);
    PRREMaterialImpl& operator=(const PRREMaterialImpl&);

private:
    PRREMaterial* _pOwner;            /**< The owner public object who creates this pimpl object. */

    TPRREuint   nIndices;             /**< Number of indices. */
    TPRREuint   nIndexSize;           /**< Size of an index. */
    PRREColor   clrAmbientFrontFace;
    PRREColor   clrAmbientBackFace;
    PRREColor   clrDiffuseFrontFace;
    PRREColor   clrDiffuseBackFace;
    PRREColor   clrSpecularFrontFace;
    PRREColor   clrSpecularBackFace;
    PRREColor   clrEmissionFrontFace;
    PRREColor   clrEmissionBackFace;
    TPRREfloat  fShininessFrontFace;
    TPRREfloat  fShininessBackFace;
    std::vector<TPRRE_MATERIAL_LAYER> layers;

    // ---------------------------------------------------------------------------

    friend class PRREMaterial;
    friend class PRREMaterialManager;

}; 

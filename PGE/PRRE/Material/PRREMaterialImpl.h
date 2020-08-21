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

#include "../PRREallHeaders.h"
#include "PRREMaterialManager.h"

/**
    Defines a layer of a material.
    Maximum number of layers per material is limited by the number of TMUs on current hardware.
    Single-textured objects need 1-layer material.
    Multi-textured objects need a material of 2 or more layers.
*/
struct TPRRE_MATERIAL_LAYER
{
    TRGBAFLOAT*    pColors;             /**< Pointer to colors. */
    TPRREuint      nColors_h;           /**< Number of colors. TODO: maybe can be removed as nVertices_h is enough. */
    PRRETexture*   tex;                 /**< Texture on layer. */
    TUVW*          pTexcoords;          /**< Pointer to texture coordinates. */
    TPRREuint      nTexcoords_h;        /**< Number of texture coordinates. TODO: maybe can be removed as Object3D::nVertices_h is enough. */
    TPRREuint      nIndexSize;          /**< Size of an index. */
	PRREColor      clrTexEnv;           /**< Texture environment color. */
	TPRRE_TEX_FUNC texEnvMode;          /**< Texture environment mode / function. */
	TPRREfloat     fTransparancy;       /**< Layer transparency. */
    TPRRE_BLENDFACTORS blendFactorSource;       /**< Source blend factor. */
    TPRRE_BLENDFACTORS blendFactorDestination;  /**< Destination blend factor.  */
};

class PRREMaterial::PRREMaterialImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual              ~PRREMaterialImpl();

            void         AllocateArrays(
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
            void         SetTexture(PRRETexture* tex, TPRREuint level = 0);

            TPRREbool    isTextured() const;
            TPRREbool    isSingleTextured() const;
            TPRREbool    isMultiTextured() const;

            PRREColor&   getTextureEnvColor(TPRREuint level = 0);
    const   PRREColor&   getTextureEnvColor(TPRREuint level = 0) const;

            TPRRE_BLENDFACTORS getSourceBlendFunc(TPRREuint level = 0) const;
            void               SetSourceBlendFunc(
                                TPRRE_BLENDFACTORS value,
                                TPRREuint level = 0);

            TPRRE_BLENDFACTORS getDestinationBlendFunc(TPRREuint level = 0) const;
            void               SetDestinationBlendFunc(
                                TPRRE_BLENDFACTORS value,
                                TPRREuint level = 0);

            void               SetBlendFuncs(
                                TPRRE_BLENDFACTORS src,
                                TPRRE_BLENDFACTORS dst,
                                TPRREuint level = 0);

            TPRRE_BLENDMODES   getBlendMode(TPRREuint level = 0) const;
            void               SetBlendMode(
                                TPRRE_BLENDMODES mode,
                                TPRREuint level = 0);

            TPRREbool    copyFromMaterial(
                            PRREMaterial& srcMat,
                            TPRREuint dstLevel,
                            TPRREuint srcLevel);

            TPRREuint    getUsedSystemMemory() const; 
                

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    PRREMaterialImpl(
        PRREMaterial* owner, TPRREuint nLayers = 1);
    
    PRREMaterialImpl(const PRREMaterialImpl&);
    PRREMaterialImpl& operator=(const PRREMaterialImpl&);

private:
    PRREMaterial* _pOwner;            /**< The owner public object who creates this pimpl object. */

    TPRREuint   nIndices;             /**< Number of indices. */
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

    CConsole& getConsole() const;     /**< Hack to be able to use CConsole singleton instance instead of owner's protected console instance. */

    friend class PRREMaterial;
    friend class PRREMaterialManager;

}; 

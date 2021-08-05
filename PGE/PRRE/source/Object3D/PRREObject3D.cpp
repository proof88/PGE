/*
    ###################################################################################
    PRREObject3D.cpp
    This file is part of PRRE.
    PRREObject3D class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Object3D/PRREObject3DManager.h"
#include "../../include/internal/Object3D/PRREObject3DImpl.h"
#include "../../include/internal/Object3D/PRREVertexTransferModeImpl.h"
#include <cassert>
#include "../../include/external/Hardware/PRREhwInfo.h"
#include "../../include/internal/PRREGLextensionFuncs.h"
#include "../../include/internal/PRREGLsafeFuncs.h"
#include "../../include/internal/PRREGLsnippets.h"
#include "../../include/internal/PRREpragmas.h"
#include "../../../../../PFL/PFL/PFL.h"

using namespace std;

/*

    Notes

    ***************************

    There are some online OpenGL capability reports can be used to find out how widely an extension is supported:
    https://www.gpuinfo.org/
     - https://opengl.gpuinfo.org/listextensions.php
     - https://opengles.gpuinfo.org/
    https://feedback.wildfiregames.com/report/opengl/


    Vertex Culling (Object-Space)
    *****************************

    GL_EXT_cull_vertex: https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_cull_vertex.txt
     - 1996
     - "Culling a polygon by examining its vertexes in object space can be more efficient than screen space polygon culling since
       the transformation to screen space (which may include a division by w) can be avoided for culled vertexes. Also, vertex culling
       can be computed before vertexes are assembled into primitives. This is a useful property when drawing meshes with shared vertexes,
       since a vertex can be culled once, and the resulting state can be used for all primitives which share the vertex."
     - This extension was most probably introduced for optimisation before HW T&L spread. Lack of HW T&L was present in the pre-Geforce era,
       i.e. TNT, etc.
       Desktop: was mostly supported by early integrated Intel VGA.
       Mobile: no support at all!
     - Verdict: don't use it if neither 3dfx nor other major vendors supported it before TnL!

*/

/*
   PRREObject3D::PRREObject3DImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Gets the appropriate PRRE blend factor for the given GL enum.

    @return The appropriate PRRE blend factor for the given GL enum.
            PRRE_ZERO for invalid GL enum.
*/
TPRRE_BLENDFACTORS PRREObject3D::PRREObject3DImpl::getPRREblendFromGLblend(GLenum glb)
{
    switch( glb )
    {
    case GL_ZERO                : return PRRE_ZERO;
    case GL_ONE                 : return PRRE_ONE;
    case GL_SRC_COLOR           : return PRRE_SRC_COLOR;
    case GL_ONE_MINUS_SRC_COLOR : return PRRE_ONE_MINUS_SRC_COLOR;
    case GL_DST_COLOR           : return PRRE_DST_COLOR;
    case GL_ONE_MINUS_DST_COLOR : return PRRE_ONE_MINUS_DST_COLOR;
    case GL_SRC_ALPHA           : return PRRE_SRC_ALPHA;
    case GL_ONE_MINUS_SRC_ALPHA : return PRRE_ONE_MINUS_SRC_ALPHA;
    case GL_DST_ALPHA           : return PRRE_DST_ALPHA;         
    case GL_ONE_MINUS_DST_ALPHA : return PRRE_ONE_MINUS_DST_ALPHA;
    case GL_SRC_ALPHA_SATURATE  : return PRRE_SRC_ALPHA_SATURATE;
    default                     : return PRRE_ZERO;
    }
} // getPRREblendFromGLblend()


/**
    Gets the appropriate GL enum for the given PRRE blend factor.

    @return The appropriate GL enum for the given PRRE blend factor.
            GL_ZERO for invalid PRRE blend factor.
*/
GLenum PRREObject3D::PRREObject3DImpl::getGLblendFromPRREblend(TPRRE_BLENDFACTORS bf)
{
    switch( bf )
    {
    case PRRE_ZERO                : return GL_ZERO;
    case PRRE_ONE                 : return GL_ONE;
    case PRRE_SRC_COLOR           : return GL_SRC_COLOR;
    case PRRE_ONE_MINUS_SRC_COLOR : return GL_ONE_MINUS_SRC_COLOR;
    case PRRE_DST_COLOR           : return GL_DST_COLOR;
    case PRRE_ONE_MINUS_DST_COLOR : return GL_ONE_MINUS_DST_COLOR;
    case PRRE_SRC_ALPHA           : return GL_SRC_ALPHA;
    case PRRE_ONE_MINUS_SRC_ALPHA : return GL_ONE_MINUS_SRC_ALPHA;
    case PRRE_DST_ALPHA           : return GL_DST_ALPHA;         
    case PRRE_ONE_MINUS_DST_ALPHA : return GL_ONE_MINUS_DST_ALPHA;
    case PRRE_SRC_ALPHA_SATURATE  : return GL_SRC_ALPHA_SATURATE;
    default                       : return GL_ZERO;
    }
} // getGLblendFromPRREblend()


PRREObject3D::PRREObject3DImpl::~PRREObject3DImpl()
{
    _pOwner->getManagedConsole().OLnOI("~PRREObject3D() ...");

    delete[] pVerticesTransf;
    delete[] pFbBuffer;
    pVerticesTransf = PGENULL;
    pFbBuffer = PGENULL;

    _pOwner->DeleteAll();

    if ( nOcclusionQuery > 0 )
    {
        glDeleteQueriesARB(1, &nOcclusionQuery);
        nOcclusionQuery = 0;
        delete pBoundingBox;
        pBoundingBox = PGENULL;
    }

    // if we are just a cloned object, above code should have done essentially nothing
    if ( getReferredObject() )
    {
        // TODO: probably we should decrement reference count in referred object ...
    }

    _pOwner->getManagedConsole().SOLnOO("Done!");
} // ~PRRETexture()


TPRRE_VERTEX_MODIFYING_HABIT PRREObject3D::PRREObject3DImpl::getVertexModifyingHabit() const
{
    return getReferredObject() ? getReferredObject()->getVertexModifyingHabit() : _pOwner->PRREVertexTransfer::getVertexModifyingHabit();
} // getVertexModifyingHabit()


TPRREbool PRREObject3D::PRREObject3DImpl::setVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    return setVertexTransferMode( PRREVertexTransfer::selectVertexTransferMode(vmod, _pOwner->getVertexReferencingMode(), false) );
} // setVertexModifyingHabit()


TPRRE_VERTEX_REFERENCING_MODE PRREObject3D::PRREObject3DImpl::getVertexReferencingMode() const
{
    return getReferredObject() ? getReferredObject()->getVertexReferencingMode() : _pOwner->PRREVertexTransfer::getVertexReferencingMode();
} // getVertexReferencingMode()


TPRREbool PRREObject3D::PRREObject3DImpl::setVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    return setVertexTransferMode( PRREVertexTransfer::selectVertexTransferMode(_pOwner->getVertexModifyingHabit(), vref, false) );
} // setVertexReferencingMode()


TPRRE_VERTEX_TRANSFER_MODE PRREObject3D::PRREObject3DImpl::getVertexTransferMode() const
{
    return getReferredObject() ? getReferredObject()->getVertexTransferMode() : _pOwner->PRREVertexTransfer::getVertexTransferMode();
} // getVertexTransferMode()


TPRREbool PRREObject3D::PRREObject3DImpl::setVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    _pOwner->getManagedConsole().OLnOI("PRREObject3D::setVertexTransferMode()");

    if ( getReferredObject() )
    {
        _pOwner->getManagedConsole().EOLnOO("PRREObject3D::setVertexTransferMode() ignored because we are cloned object!");
        return false;
    }

    TPRREbool b = _pOwner->PRREVertexTransfer::setVertexTransferMode(vtrans);
    _pOwner->getManagedConsole().OO();
    return b;
}


PRREObject3D* PRREObject3D::PRREObject3DImpl::getReferredObject() const
{
    return pRefersto; 
}


TPRRE_TRANSFORMED_VERTEX* PRREObject3D::PRREObject3DImpl::getTransformedVertices(TPRREbool implicitAccessSubobject)
{
    if ( implicitAccessSubobject && _pOwner->isLevel1() && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getTransformedVertices();
    else
        return pVerticesTransf;
}


PRREVector& PRREObject3D::PRREObject3DImpl::getAngleVec()
{
    return vAngle;
} // getAngleVec()


const PRREVector& PRREObject3D::PRREObject3DImpl::getAngleVec() const
{
    return vAngle;
} // getAngleVec()


PRREVector PRREObject3D::PRREObject3DImpl::getScaledSizeVec() const
{
    return PRREVector(
        _pOwner->getSizeVec().getX() * vScaling.getX(),
        _pOwner->getSizeVec().getY() * vScaling.getY(),
        _pOwner->getSizeVec().getZ() * vScaling.getZ());
} // getScaledSizeVec()


const PRREVector& PRREObject3D::PRREObject3DImpl::getScaling() const
{
    return vScaling;
} // getScaling()


void PRREObject3D::PRREObject3DImpl::SetScaling(TPRREfloat value)
{
    vScaling.Set(value, value, value);
} // SetScaling()


void PRREObject3D::PRREObject3DImpl::SetScaling(const PRREVector& value)
{
    vScaling = value;
} // SetScaling()


void PRREObject3D::PRREObject3DImpl::Scale(TPRREfloat value)
{
    vScaling.Set( vScaling.getX() * value, vScaling.getY() * value, vScaling.getZ() * value );
} // Scale()


void PRREObject3D::PRREObject3DImpl::Scale(const PRREVector& value)
{
    vScaling.Set( vScaling.getX() * value.getX(), vScaling.getY() * value.getY(), vScaling.getZ() * value.getZ() );
} // Scale()


TPRREbool PRREObject3D::PRREObject3DImpl::isVisible() const
{
    return bVisible;
} // isVisible()


void PRREObject3D::PRREObject3DImpl::SetVisible(TPRREbool state)
{
    bVisible = state;
} // SetVisible()


void PRREObject3D::PRREObject3DImpl::Show()
{
    SetVisible(true);
} // Show()


void PRREObject3D::PRREObject3DImpl::Hide()
{
    SetVisible(false);
} // Hide()


TPRREbool PRREObject3D::PRREObject3DImpl::isColliding_TO_BE_REMOVED() const
{
    return bColliding;
}


void PRREObject3D::PRREObject3DImpl::SetColliding_TO_BE_REMOVED(TPRREbool value)
{
    bColliding = value;
}


TPRRE_ROTATION_ORDER PRREObject3D::PRREObject3DImpl::getRotationOrder() const
{
    return rotation;
} // getRotationOrder()


void PRREObject3D::PRREObject3DImpl::SetRotationOrder(TPRRE_ROTATION_ORDER value)
{
    rotation = value;
} // SetRotationOrder()


TPRREbool PRREObject3D::PRREObject3DImpl::isLit() const
{
    return bAffectedByLights;
} // isLit()


void PRREObject3D::PRREObject3DImpl::SetLit(TPRREbool value)
{
    bAffectedByLights = value;
} // SetLit()


TPRREbool PRREObject3D::PRREObject3DImpl::isDoubleSided() const
{
    return bDoubleSided;
} // isDoubleSided()


void PRREObject3D::PRREObject3DImpl::SetDoubleSided(TPRREbool value)
{
    bDoubleSided = value;
} // SetDoubleSided()


TPRREbool PRREObject3D::PRREObject3DImpl::isWireframed() const
{
    return bWireframe;
} // isWireframed()


void PRREObject3D::PRREObject3DImpl::SetWireframed(TPRREbool value)
{
    bWireframe = value;
} // SetWireframed()


TPRREbool PRREObject3D::PRREObject3DImpl::isWireframedCulled() const
{
    return bWireframedCull;
} // isWireframedCulled()


void PRREObject3D::PRREObject3DImpl::SetWireframedCulled(TPRREbool value)
{
    bWireframedCull = value;
} // SetWireframedCulled()


TPRREbool PRREObject3D::PRREObject3DImpl::isAffectingZBuffer() const
{
    return bAffectZBuffer;
} // isAffectingZBuffer()


void PRREObject3D::PRREObject3DImpl::SetAffectingZBuffer(TPRREbool value)
{
    bAffectZBuffer = value;
} // SetAffectingZBuffer()


TPRREbool PRREObject3D::PRREObject3DImpl::isTestingAgainstZBuffer() const
{
    return bAllowZTesting;
} // isTestingAgainstZBuffer()


void PRREObject3D::PRREObject3DImpl::SetTestingAgainstZBuffer(TPRREbool value)
{
    bAllowZTesting = value;
} // SetTestingAgainstZBuffer()


TPRREbool PRREObject3D::PRREObject3DImpl::isStickedToScreen() const
{
    return bStickedToScreen;
} // isStickedToScreen()


void PRREObject3D::PRREObject3DImpl::SetStickedToScreen(TPRREbool value)
{
    bStickedToScreen = value;
    SetDoubleSided(true);
    SetLit(false);
    SetTestingAgainstZBuffer(false);
} // SetStickedToScreen()


TPRREbool PRREObject3D::PRREObject3DImpl::isOccluder() const
{
    return bOccluder;
}


void PRREObject3D::PRREObject3DImpl::SetOccluder(TPRREbool value)
{
    bOccluder = value;
}
                                                                                                                           

const PRREObject3D* PRREObject3D::PRREObject3DImpl::getBoundingBoxObject() const
{
    return pBoundingBox;
}


TPRREuint PRREObject3D::PRREObject3DImpl::getUsedSystemMemory() const
{
    return (
        sizeof(*this) +
        nFbBuffer_h * sizeof(GLfloat) +
        sizeof(TPRRE_TRANSFORMED_VERTEX) * _pOwner->getVertexIndicesCount(false) +
        (pBoundingBox ? pBoundingBox->getUsedSystemMemory() : 0)
    );
} // getUsedSystemMemory()     


void PRREObject3D::PRREObject3DImpl::Draw_NoOcclusionQuery(const TPRRE_RENDER_PASS& /* this render path ignores this parameter */)
{
    // caller renderer is expected to check for GL errors, so we don't check them here

    if ( !bVisible )
        return;

    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        Draw_DrawSW();
        return;
    }

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( _pOwner->isLevel1() || getReferredObject() )
    {
        // Currently 3D objects are 2-level entities:
        // first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.
        
        Draw_ApplyTransformations();
        Draw_PrepareGLbeforeDraw(false);

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = getReferredObject() ? getReferredObject() : _pOwner;
        pWhichParent->pImpl->bParentInitiatedOperation = true;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(PRRE_RPASS_NORMAL);
        pWhichParent->pImpl->bParentInitiatedOperation = false;
        return;
    }

    // if we reach this point then either our parent is drawing us as its subobject, or a cloned object is drawing us on behalf of our parent

    // subobject must ignore draw if its Draw() was not called by its parent level-1 object but someone else from outside ...
    if ( !((PRREObject3D*)_pOwner->getManager())->pImpl->bParentInitiatedOperation )
    {
        _pOwner->getManagedConsole().EOLn("Draw() of subobject called outside of its level-1 parent object, ignoring draw!");
        return;
    }

    /* currently not supporting any vendor-specific mode */
    if ( BITF_READ(_pOwner->getVertexTransferMode(),PRRE_VT_VENDOR_BITS,3) != 0 )
        return;

    Draw_LoadTexturesAndSetBlendState();

    Draw_FeedbackBuffer_Start();
    ((PRREVertexTransfer*)_pOwner)->pImpl->TransferVertices();
    Draw_FeedbackBuffer_Finish(); 
} // Draw_NoOcclusionQuery()


void PRREObject3D::PRREObject3DImpl::Draw_NaiveOcclusionQuery(const TPRRE_RENDER_PASS& renderPass)
{
    // caller renderer is expected to check for GL errors, so we don't check them here

    if ( !bVisible )
        return;

    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        Draw_DrawSW();
        return;
    }

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( _pOwner->isLevel1() || getReferredObject() )
    {
        // Currently 3D objects are 2-level entities:
        // first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.

        Draw_ApplyTransformations();        
        Draw_PrepareGLbeforeDraw(false);

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = getReferredObject() ? getReferredObject() : _pOwner;
        pWhichParent->pImpl->bParentInitiatedOperation = true;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(renderPass);
        pWhichParent->pImpl->bParentInitiatedOperation = false;
        return;
    }

    // if we reach this point then either our parent is drawing us as its subobject, or a cloned object is drawing us on behalf of our parent

    // subobject must ignore draw if its Draw() was not called by its parent level-1 object but someone else from outside ...
    if ( !((PRREObject3D*)_pOwner->getManager())->pImpl->bParentInitiatedOperation )
    {
        _pOwner->getManagedConsole().EOLn("Draw() of subobject called outside of its level-1 parent object, ignoring draw!");
        return;
    }

    /* currently not supporting any vendor-specific mode */
    if ( BITF_READ(_pOwner->getVertexTransferMode(),PRRE_VT_VENDOR_BITS,3) != 0 )
        return;

    Draw_LoadTexturesAndSetBlendState();

    Draw_FeedbackBuffer_Start();
    ((PRREVertexTransfer*)_pOwner)->pImpl->TransferVertices();
    Draw_FeedbackBuffer_Finish(); 
} // Draw_NaiveOcclusionQuery()


// ############################## PROTECTED ##############################


/**
    @param owner                 The public Object3D class instance owning this pimpl object.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
*/
PRREObject3D::PRREObject3DImpl::PRREObject3DImpl(
    PRREObject3D* owner,
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory )
{
    _pOwner = owner;
    _pOwner->getManagedConsole().OLnOI("PRREObject3D() ...");

    pRefersto = NULL;
    bAffectedByLights = bAffectZBuffer = bAllowZTesting = true;
    bVisible = true;
    bParentInitiatedOperation = false;
    bColliding = bDoubleSided = false;
    bWireframe = bWireframedCull = bStickedToScreen = false;
    bOccluder = false;
    bOcclusionQueryStarted = false;
    nOcclusionQuery = 0;
    pBoundingBox = PGENULL;

    vScaling.Set(1.0f, 1.0f, 1.0f);
    rotation = PRRE_YXZ;
    // nTexcoordsVBO to be resized by manager outside

    selectVertexTransferMode(vmod, vref, bForceUseClientMemory);
    pVerticesTransf = PGENULL;
    pFbBuffer = PGENULL;

    nFbBuffer_h = 0;

    _pOwner->getManagedConsole().SOLnOO("Done!");
} // PRREObject3D()


PRREObject3D::PRREObject3DImpl::PRREObject3DImpl(const PRREObject3DImpl&)
{

}


PRREObject3D::PRREObject3DImpl& PRREObject3D::PRREObject3DImpl::operator=(const PRREObject3DImpl&)
{
    return *this;
}


// ############################### PRIVATE ###############################


/**
    Loads the given texture into the texture mapping unit.

    @param tex  Texture to be loaded. If NULL, texturing will be disabled on the specified texture mapping unit.
    @param iTMU Into which TMU we want to load the texture. Currently it must be either 0 or 1.
*/
void PRREObject3D::PRREObject3DImpl::Draw_LoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    // TODO: the PR00FPS-style lasttex method is not present here currently ... later it should be implemented.
    // A general TMU manager should record which TMU holds which texture, so that can avoid unnecessary glBindTexture() calls.

    // currently we support only 2 textured layers ...
    if ( iTMU > 1 )
    {
        _pOwner->getManagedConsole().EOLn("ERROR: Draw_LoadTextureIntoTMU(tex, %d)", iTMU);
        _ASSERT( false );
    }

    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        glActiveTextureARB(GL_TEXTURE0_ARB + iTMU);
        if ( tex != PGENULL )
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex->getInternalNum());
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
    else
    {
        if ( tex != PGENULL )
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex->getInternalNum());
            if ( isStickedToScreen() )
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            else
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
} // Draw_LoadTextureIntoTMU()


/**
    Loads all textures into all texture mapping units and sets blending if needed.
*/
void PRREObject3D::PRREObject3DImpl::Draw_LoadTexturesAndSetBlendState() const
{
    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        if ( _pOwner->getMaterial().isMultiTextured() )
        {
            // enable blending of 2nd layer
            glEnable(GL_BLEND);
	          glBlendFunc(getGLblendFromPRREblend(_pOwner->getMaterial().getSourceBlendFunc(1)), getGLblendFromPRREblend(_pOwner->getMaterial().getDestinationBlendFunc(1)));
            Draw_LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(0), 0 );
            Draw_LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(1), 1 );
        }
        else
        {
            // disable blending only if base layer is not blended ...
            if ( ! PRREObject3DManager::isBlendFuncBlends(_pOwner->getMaterial().getSourceBlendFunc(), _pOwner->getMaterial().getDestinationBlendFunc()) )
                glDisable(GL_BLEND);
            Draw_LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(0), 0 );
            Draw_LoadTextureIntoTMU( PGENULL, 1 );
        }
    }
    else
    {
        // disable blending only if base layer is not blended ...
        if ( ! PRREObject3DManager::isBlendFuncBlends(_pOwner->getMaterial().getSourceBlendFunc(), _pOwner->getMaterial().getDestinationBlendFunc()) )
            glDisable(GL_BLEND);
        Draw_LoadTextureIntoTMU( _pOwner->getMaterial().getTexture() );
    }
} // Draw_LoadTexturesAndSetBlendState()


/**
    Set render mode to feedback and allocate buffer for transformed vertices.
    This is done only once, when the functions is called for the first time.
    Any consecutive call to this function has no effect, so pFbBuffer and nFbBuffer_h variables
    will hold the same values that have been stored during the rendereing of the 1st frame.
    This is for debugging purposes only, for breakpoint, etc.
    But in the future when HW transform and SW rasterization can be combined, this will be controlled by public API.
*/
void PRREObject3D::PRREObject3DImpl::Draw_FeedbackBuffer_Start()
{
    /* We create this buffer only once at first run. */
    /* We intentionally switch to feedback mode only once since now  */
    if ( pFbBuffer != NULL )
        return;

    /* num of values can be stored in this buffer */
    /* we need to store not only the transformed values but also some extra values for grammar */
    /* in this case, the items placed in this array are polygon items (GL_POLYGON_TOKEN) which look like this:
        GL_POLYGON_TOKEN 3 (value value value value (value value value value) (value value value value))
        So for a standard cube where each side has 2 triangles, it is 12 triangles in total, meaning
        12 polygons. GL_POLYGON_TOKEN itself is 1 value, 3 itself is also 1 value, and then 3*12 values for 3 vertices.
        So the total num of values is: numOfTriangles * (2+3*12)
        Example with some real data is in later comment with debugbuffer[]. */
    
    nFbBuffer_h = GLsizei(ceil((_pOwner->getVerticesCount(false) / 3.0f))) * (2+3*12);
    try
    {
        /* TODO: probably in future we should rather use pVerticesTransf here as well since we already have it for that purpose, right? ;) */
        pFbBuffer = new GLfloat[nFbBuffer_h];

        /* unfortunately only the most detailed option GL_4D_COLOR_TEXTURE will give us the w-coord of vertices so we need to use that */
        if ( pglFeedbackBuffer(nFbBuffer_h, GL_4D_COLOR_TEXTURE, pFbBuffer) )
        {
            // If you want all vertices to be transformed and catched in feedback mode then dont forget to disable culling and depth testing (maybe only 1 is needed to be disabled).
            //glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE); /* otherwise only the front facing side of cube would be written to feedback buffer */

            glRenderMode(GL_FEEDBACK);
        }
    }
    catch (const std::bad_alloc&)
    {
        _pOwner->getManagedConsole().EOLn("ERROR: PRREObject3D::PRREObject3DImpl::Draw() failed to allocate pFbBuffer!");
    }
} // Draw_FeedbackBuffer_Start()


/**
    Set render mode to default render.
    This is for debugging purposes only, for breakpoint, etc.
    If a breakpoint is set inside this function, the coordinates of HW-transformed vertices can be examined.
*/
void PRREObject3D::PRREObject3DImpl::Draw_FeedbackBuffer_Finish()
{
    /* The number of values (not vertices) transferred to the feedback buffer. */
    const GLint nFbBufferWritten_h = glRenderMode(GL_RENDER);

    GLfloat debugbuffer[500];

    if ( nFbBufferWritten_h > 0 )
    {
        // we are happy
        for (int i = 0; i < min(500,nFbBufferWritten_h); i++)
            debugbuffer[i] = pFbBuffer[i];
        //Sleep(1); // suitable for placing breakpoint
        /*
            debugbuffer[0] = GL_POLYGON_TOKEN;
            debugbuffer[1] = 3;
            debugbuffer[ 2.. 5] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[ 6.. 9] = r g b a
            debugbuffer[10..13] = texture coords, probably x y u w
            debugbuffer[14..17] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[18..21] = r g b a
            debugbuffer[22..25] = texture coords, probably x y u w
            debugbuffer[26..29] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[30..33] = r g b a
            debugbuffer[34..37] = texture coords, probably x y u w
            debugbuffer[38] = GL_POLYGON_TOKEN;
            debugbuffer[39] = 3;

            example: gamedata\\models\\cube.obj
            assuming that nearplane is 0.1f, farplane is 100.0f, depthrange is [0, 1], fovy is 80
            box
            i=2 (3. vertex) debugbuffer[26]
                model space        -> world space       -> view/eye space    -> clip space                   -> normalized device coords     -> screen coords
            sw: [-0.5, -0.5, -0.5] -> [-0.5, -0.5, 2.5] -> [-0.5, -0.5, 2.5] -> [-0.446, -0.595, 2.348, 2.5] -> [-0.178, -0.238, 0.939, 2.5] -> [328, 228, 0.961, 2.5]
            hw:                                                                                                                                 [328, 228, 0.961, 2.5]

            i=6 (7. vertex) debugbuffer[?]
                model space        -> world space       -> view/eye space    -> clip space                   -> normalized device coords     -> screen coords
            sw: [ 0.5,  0.5,  0.5] -> [ 0.5,  0.5, 3.5] -> [ 0.5,  0.5, 3.5] -> [ 0.446,  0.595, 3.368, 3.5] -> [ 0.127,  0.170, 0.962, 3.5] -> [451, 351, 0.972, 3.5]
            hw:                                                                                                                                 [451, 351, 0.972, 3.5]

            ...
        */
    }
    else
    {
        // we were already in GL_RENDER (0) mode, OR nothing was returned, OR we were in FEEDBACK but buffer was not enough to hold all values (<0) ...
    }
} // Draw_FeedbackBuffer_Finish()


/**
    Applies transformations to the current modelview matrix based on the given object.
*/
void PRREObject3D::PRREObject3DImpl::Draw_ApplyTransformations() const    
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    glTranslatef(_pOwner->getPosVec().getX(), _pOwner->getPosVec().getY(), _pOwner->getPosVec().getZ());
    
    switch ( getRotationOrder() )
    {
    case PRRE_XYZ:
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        break;
    case PRRE_XZY:
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getY(), 0,1,0);
        break;
    case PRRE_YXZ:
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
         break;
    case PRRE_YZX:
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getX(), 1,0,0);
        break;
    case PRRE_ZXY:
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getY(), 0,1,0);
        break;
    case PRRE_ZYX:
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getX(), 1,0,0);
        break;
    } // switch

    glScalef(getScaling().getX(), getScaling().getY(), getScaling().getZ());
    /*
        Well, mirror-transforming an object (scaling by negative value) makes its faces flip:
        https://www.gamedev.net/forums/topic/640616-negative-scaling-flips-face-winding-affects-backface-culling/
        I wanted to X-mirror objects loaded by tmcsgfxlib-to-prre wrapper for legacy projects, because I noticed they appeared mirrored compared to proofps engine behavior.
        So I noticed when setting negative scale on X axis, faces also flipped, so objects became inside-out.
        This is expected since vertices' winding order also flips because we still feed triangle vertices in same order even though triangles become mirrored.
        But I don't treat this as a legacy-project-only issue, since even a new project with the new engine could come up for any reason with negative scaling for any mirroring reason.
        So, this is rather a feature: we need to detect negative scaling factor and change front face winding order on-the-fly.
    */
    if ( getScaling().getX() * getScaling().getY() * getScaling().getZ() <= (0.0f-PFL::E) )
        glFrontFace(GL_CW);
    else
        glFrontFace(GL_CCW);
} // Draw_ApplyTransformations()


void PRREObject3D::PRREObject3DImpl::Draw_PrepareGLbeforeDraw(bool bLighting) const
{
    /*AmbientLightPos[0] =  cam.getX() - obj->getPosVec().getX();
    AmbientLightPos[1] =  cam.getY() - obj->getPosVec().getY();
    AmbientLightPos[2] = -cam.getZ() - obj->getPosVec().getZ();*/
    /*AmbientLightPos[0] = cam.getX();
    AmbientLightPos[1] = cam.getY();
    AmbientLightPos[2] = -cam.getZ()-2.0f;
    AmbientLightPos[3] = 1.0f;
    glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);  */

    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( isDoubleSided() )
    {
        glDisable(GL_CULL_FACE);
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos); 
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
    }
    else
    {
        glEnable(GL_CULL_FACE);
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
    } // isDoubleSided()

    if ( isWireframed() )
    {
        glDisable(GL_LIGHTING);
        if ( !isWireframedCulled() )
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } // isWireframed()

    if ( isTestingAgainstZBuffer() )
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    // TODO: nope, we shouldnt specify color this way because glColor4f() is called per-vertex anyway later
    // getTextureEnvColor() is used by texture environment mode / function, not blending!
    // we should rather use glBlendColor() for this with GL_*_CONSTANT_* blendfunc, or modify the code not to specify color per-vertex
    
    glColor4f(
        _pOwner->getMaterial().getTextureEnvColor().getRedAsFloat(),
        _pOwner->getMaterial().getTextureEnvColor().getGreenAsFloat(),
        _pOwner->getMaterial().getTextureEnvColor().getBlueAsFloat(),
        _pOwner->getMaterial().getTextureEnvColor().getAlphaAsFloat() );
                
                
    if ( PRREObject3DManager::isBlendFuncBlends(_pOwner->getMaterial().getSourceBlendFunc(), _pOwner->getMaterial().getDestinationBlendFunc()) )
    {
        glEnable(GL_BLEND);
        glBlendFunc(getGLblendFromPRREblend(_pOwner->getMaterial().getSourceBlendFunc()), getGLblendFromPRREblend(_pOwner->getMaterial().getDestinationBlendFunc()));
        glAlphaFunc(GL_GREATER, 0.1f);
        glEnable(GL_ALPHA_TEST);
    }
    else
    {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }
} // Draw_PrepareGLbeforeDraw()


void PRREObject3D::PRREObject3DImpl::Draw_PrepareGLbeforeOcclusionQuery() const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    
    if ( PRREhwInfo::get().getVideo().isBooleanOcclusionQuerySupported() )
    {
        glBeginQueryARB(GL_ANY_SAMPLES_PASSED, nOcclusionQuery);
    }
    else
    {
        glBeginQueryARB(GL_SAMPLES_PASSED_ARB, nOcclusionQuery);
    }
} // Draw_PrepareGLbeforeOcclusionQuery()


void PRREObject3D::PRREObject3DImpl::Draw_ResetGLafterOcclusionQuery() const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( PRREhwInfo::get().getVideo().isBooleanOcclusionQuerySupported() )
    {
        glEndQueryARB(GL_ANY_SAMPLES_PASSED);
    }
    else
    {
        glEndQueryARB(GL_SAMPLES_PASSED_ARB);
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glFlush();
} // Draw_ResetGLafterOcclusionQuery()


void PRREObject3D::PRREObject3DImpl::Draw_Sync_OcclusionQuery_Start() const
{
    if ( nOcclusionQuery == 0 )
    {
        // no occlusion test so we return NOT occluded, so it should be rendered normally
        return;
    }

    assert(pBoundingBox != PGENULL);
    assert(pBoundingBox->getCount() > 0);
    Draw_PrepareGLbeforeOcclusionQuery();
    ((PRREVertexTransfer*)pBoundingBox->getAttachedAt(0))->pImpl->TransferVertices();
    Draw_ResetGLafterOcclusionQuery();

    GLint queryResultAvailable;
    do {
        glGetQueryObjectivARB(nOcclusionQuery, GL_QUERY_RESULT_AVAILABLE_ARB, &queryResultAvailable);
    } while (!queryResultAvailable);

    if ( PRREhwInfo::get().getVideo().isBooleanOcclusionQuerySupported() )
    {
        GLint sampleBoolean;
        glGetQueryObjectivARB(nOcclusionQuery, GL_QUERY_RESULT_ARB, &sampleBoolean);
        return/* ( sampleBoolean == GL_FALSE )*/;
    }
    else
    {
        GLuint sampleCount;
        glGetQueryObjectuivARB(nOcclusionQuery, GL_QUERY_RESULT_ARB, &sampleCount);
        return/* ( sampleCount == 0 )*/;
    }
} // Draw_CheckIfOccluded_Sync()


void PRREObject3D::PRREObject3DImpl::Draw_DrawSW()
{
    if ( !bVisible )
        return;

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( _pOwner->isLevel1() || getReferredObject() )
    {
        // Currently 3D objects are 2-level things: first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.

        Draw_ApplyTransformations();

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = getReferredObject() ? getReferredObject() : _pOwner;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(PRRE_RPASS_NORMAL);
        return;
    }

    // actual draw here
    // todo: wtf, we should finally decide if it is renderer's responsibility to actually render an object or object's responsibility?
} // Draw_DrawSW()


/*
   PRREObject3D
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREObject3D::~PRREObject3D()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRRETexture()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREObject3D::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREObject3D::getLoggerModuleName()
{
    return "PRREObject3D";
} // getLoggerModuleName()


/**
    The details of this function are described in PRREVertexTransfer class.
    If this is a cloned object, the returned value is the same as the referred object's value.
*/
TPRRE_VERTEX_MODIFYING_HABIT PRREObject3D::getVertexModifyingHabit() const
{
    return pImpl->getVertexModifyingHabit();
} // getVertexModifyingHabit()


/**
    The details of this function are described in PRREVertexTransfer class.
    Same functionality as defined originally in PRREVertexTransfer::setVertexModifyingHabit() but extended with a check for being a cloned object or not.
    This function has no effect when called for a cloned object.
    @param vmod Vertex modifying habit to be set.
    @return     True on success, false otherwise, including if called for a cloned object.
*/
TPRREbool PRREObject3D::setVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    return pImpl->setVertexModifyingHabit(vmod);
} // setVertexModifyingHabit()


/**
    The details of this function are described in PRREVertexTransfer class.
    If this is a cloned object, the returned value is the same as the referred object's value.
*/
TPRRE_VERTEX_REFERENCING_MODE PRREObject3D::getVertexReferencingMode() const
{
    return pImpl->getVertexReferencingMode();
} // getVertexReferencingMode()

 
/**
    The details of this function are described in PRREVertexTransfer class.
    Same functionality as defined originally in PRREVertexTransfer::setVertexTransferMode() but extended with a check for being a cloned object or not.
    This function has no effect when called for a cloned object.
    @param  vref Vertex referencing mode to be set.
    @return      True on success, false otherwise, including if called for a cloned object.
*/
TPRREbool PRREObject3D::setVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    return pImpl->setVertexReferencingMode(vref);
} // setVertexReferencingMode()


/**
    The details of this function are described in PRREVertexTransfer class.
    If this is a cloned object, the returned value is the same as the referred object's value.
*/
TPRRE_VERTEX_TRANSFER_MODE PRREObject3D::getVertexTransferMode() const
{
    return pImpl->getVertexTransferMode();
} // getVertexTransferMode()


/**
    The details of this function are described in PRREVertexTransfer class.
    Same functionality as defined originally in PRREVertexTransfer::setVertexTransferMode() but extended with a check for being a cloned object or not.
    This function has no effect when called for a cloned object.
    @param  vtrans Vertex referencing mode to be set.
    @return        True on success, false otherwise, including if called for a cloned object.
*/
TPRREbool PRREObject3D::setVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    return pImpl->setVertexTransferMode(vtrans);
} // setVertexTransferMode()


/**
    Gets the original object which was cloned to create this object.
    @return NULL if this is a non-cloned object, otherwise the pointer to the original object which was cloned to create this cloned object.
*/
PRREObject3D* PRREObject3D::getReferredObject() const
{
    return pImpl->getReferredObject();
}


/**
    Gets the pointer to transformed vertices.
    These are generated only during first rendering.
    This value is irrelevant for a level-1 object since the geometry is owned by its level-2 subobjects.
    Still the returned value for a level-1 object can be a non-NULL value, see below.

    @return Pointer to transformed vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's transformed vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const TPRRE_TRANSFORMED_VERTEX* PRREObject3D::getTransformedVertices(TPRREbool implicitAccessSubobject) const
{
    return pImpl->getTransformedVertices(implicitAccessSubobject);
} // getVertices(false)


/**
    Gets the pointer to transformed vertices.
    These are generated only during first rendering.
    This value is irrelevant for a level-1 object since the geometry is owned by its level-2 subobjects.
    Still the returned value for a level-1 object can be a non-NULL value, see below.

    @return Pointer to transformed vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's transformed vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRRE_TRANSFORMED_VERTEX* PRREObject3D::getTransformedVertices(TPRREbool implicitAccessSubobject)
{
    return pImpl->getTransformedVertices(implicitAccessSubobject);
} // getVertices(false)


/**
    Gets the rotation angles.
    Rotation angle is currently ignored for level-2 objects, they are rotated by the same factor as their level-1 parent object using the same pivot point.
    By default this property is (0.0f, 0.0f, 0.0f).
    @return Rotation angles vector.
*/
PRREVector& PRREObject3D::getAngleVec()
{
    return pImpl->getAngleVec();
} // getAngleVec()


/**
    Gets the rotation angles.
    Rotation angle is currently ignored for level-2 objects, they are rotated by the same factor as their level-1 parent object using the same pivot point.
    By default this property is (0.0f, 0.0f, 0.0f).
    @return Rotation angles vector.
*/
const PRREVector& PRREObject3D::getAngleVec() const
{
    return pImpl->getAngleVec();
} // getAngleVec()


/**
    Gets the base sizes.
    The Mesh3D part of a cloned object does not have its own geometry, thus even Mesh3D::RecalculateSize() would calculate it to 0.
    Thus if we are a cloned object, we need to return the size of the object we are referring to.
*/
const PRREVector& PRREObject3D::getSizeVec() const
{
    return (getReferredObject() == PGENULL) ? PRREMesh3D::getSizeVec() : getReferredObject()->getSizeVec();
}


/**
    Gets the real sizes considering the geometry size calculated from vertex data and the current scaling factor.
    @return Real sizes vector.
*/
PRREVector PRREObject3D::getScaledSizeVec() const
{
    return pImpl->getScaledSizeVec();
} // getScaledSizeVec()


/**
    Gets the scaling factor.
    By default this property is (1.0f, 1.0f, 1.0f).
    @return Scaling.
*/
const PRREVector& PRREObject3D::getScaling() const
{
    return pImpl->getScaling();
} // getScaling()


/**
    Sets the scaling factor to given scalar.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
    By default this property is (1.0f, 1.0f, 1.0f).

    @param value The desired scaling factor.
*/
void PRREObject3D::SetScaling(TPRREfloat value)
{
    pImpl->SetScaling(value);
} // SetScaling()


/**
    Sets the scaling factor to given vector.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
    By default this property is (1.0f, 1.0f, 1.0f).

    @param value The desired scaling factor.
*/
void PRREObject3D::SetScaling(const PRREVector& value)
{
    pImpl->SetScaling(value);
} // SetScaling()


/**
    Scales by the given scalar value. 
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
    By default this property is (1.0f, 1.0f, 1.0f).

    @param value The factor to be scalen by.
*/
void PRREObject3D::Scale(TPRREfloat value)
{
    pImpl->Scale(value);
} // Scale()


/**
    Scales by the given vector.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
    By default this property is (1.0f, 1.0f, 1.0f).

    @param value The vector to be scalen by.
*/
void PRREObject3D::Scale(const PRREVector& value)
{
    pImpl->Scale(value);
} // Scale()


/**
    Gets the visibility state.
    If an object is not visible, it is not rendered.
    This property is manual setting, not related to any visibility calculations that might be done by renderer.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
    By default this property is true.
    @return True, if visible, false otherwise.
*/
TPRREbool PRREObject3D::isVisible() const
{
    return pImpl->isVisible();
} // isVisible()


/**
    Sets the visibility state.
    If an object is not visible, it is not rendered.
    This property is manual setting, not related to any visibility calculations that might be done by renderer.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
    By default this property is true.

    @param value The desired state.
*/
void PRREObject3D::SetVisible(TPRREbool state)
{
    pImpl->SetVisible(state);
} // SetVisible()


/**
    Sets the visibility state to true.
    Equivalent to SetVisible(true).
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
*/
void PRREObject3D::Show()
{
    pImpl->Show();
} // Show()


/**
    Sets the visibility state to false.
    Equivalent to SetVisible(false).
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
*/
void PRREObject3D::Hide()
{
    pImpl->Hide();
} // Hide()


/**
    Gets whether colliding is enabled.
    Legacy function unrelated to the purpose of this class, will need to be removed!
    By default this property is false.
    @return True, if colliding is enabled, false otherwise.
*/
TPRREbool PRREObject3D::isColliding_TO_BE_REMOVED() const
{
    return pImpl->isColliding_TO_BE_REMOVED();
}


/**
    Sets whether colliding is enabled.
    Legacy function unrelated to the purpose of this class, will need to be removed!
    By default this property is false.

    @param value The desired state.
*/
void PRREObject3D::SetColliding_TO_BE_REMOVED(TPRREbool value)
{
    pImpl->SetColliding_TO_BE_REMOVED(value);
}


/**
    Gets the rotation order.
    Rotation order is currently ignored for level-2 objects, they are rotated the same way as their level-1 parent object.
    By default this property is PRRE_YXZ.
    @return Rotation order.
*/
TPRRE_ROTATION_ORDER PRREObject3D::getRotationOrder() const
{
    return pImpl->getRotationOrder();
} // getRotationOrder()


/**
    Sets the rotation order.
    Rotation order is currently ignored for level-2 objects, they are rotated the same way as their level-1 parent object.
    By default this property is PRRE_YXZ.

    @param value The desired state.
*/
void PRREObject3D::SetRotationOrder(TPRRE_ROTATION_ORDER value)
{
    pImpl->SetRotationOrder(value);
} // SetRotationOrder()


/**
    Gets the lit state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is true.
    @return True if lit, false otherwise.
*/
TPRREbool PRREObject3D::isLit() const
{
    return pImpl->isLit();
} // isLit()


/**
    Sets the lit state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is true.

    @param value The desired state.
*/
void PRREObject3D::SetLit(TPRREbool value)
{
    pImpl->SetLit(value);
} // SetLit()


/**
    Gets the double sided state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.
    @return True if double sided, false otherwise.
*/
TPRREbool PRREObject3D::isDoubleSided() const
{
    return pImpl->isDoubleSided();
} // isDoubleSided()


/**
    Sets the double sided state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.

    @param value The desired state.
*/
void PRREObject3D::SetDoubleSided(TPRREbool value)
{
    pImpl->SetDoubleSided(value);
} // SetDoubleSided()


/**
    Gets the wireframed state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.
    @return True if wireframed, false otherwise.
*/
TPRREbool PRREObject3D::isWireframed() const
{
    return pImpl->isWireframed();
} // isWireframed()


/**
    Sets the wireframed state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.

    @param value The desired state.
*/
void PRREObject3D::SetWireframed(TPRREbool value)
{
    pImpl->SetWireframed(value);
} // SetWireframed()


/**
    Gets the wireframed culling state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.
    @return True if culling is enabled in wireframed state, false otherwise.
*/
TPRREbool PRREObject3D::isWireframedCulled() const
{
    return pImpl->isWireframedCulled();
} // isWireframedCulled()


/**
    Sets the wireframed culling state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.

    @param value The desired state.
*/
void PRREObject3D::SetWireframedCulled(TPRREbool value)
{
    pImpl->SetWireframedCulled(value);
} // SetWireframedCulled()


/**
    Gets whether we write to the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is true.
    @return True if we write to Z-Buffer while rendering, false otherwise.
*/
TPRREbool PRREObject3D::isAffectingZBuffer() const
{
    return pImpl->isAffectingZBuffer();
} // isAffectingZBuffer()


/**
    Sets whether we write to the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is true.

    @param value The desired state.
*/
void PRREObject3D::SetAffectingZBuffer(TPRREbool value)
{
    pImpl->SetAffectingZBuffer(value);
} // SetAffectingZBuffer()


/**
    Gets whether we test against the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is true.
    @return True if we test against the Z-Buffer while rendering, false otherwise.
*/
TPRREbool PRREObject3D::isTestingAgainstZBuffer() const
{
    return pImpl->isTestingAgainstZBuffer();
} // isTestingAgainstZBuffer()


/**
    Sets whether we test against the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is true.

    @param value The desired state.
*/
void PRREObject3D::SetTestingAgainstZBuffer(TPRREbool value)
{
    pImpl->SetTestingAgainstZBuffer(value);
} // SetTestingAgainstZBuffer()


/**
    Gets the sticked-to-screen state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.
    @return True if sticked to screen, false otherwise.
*/
TPRREbool PRREObject3D::isStickedToScreen() const
{
    return pImpl->isStickedToScreen();
} // isStickedToScreen()


/**
    Sets the sticked-to-screen state.
    Sticked to screen means the object is rendered with orthogonal projection and not overlapped by non-sticked objects.
    Suitable for 2D object rendering such as GUI elements.
    Note that this call not only changes sticked state of the object but also calls the following methods:
     - SetDoubleSided(true);
     - SetLit(false);
     - SetTestingAgainstZBuffer(false).
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.

    @param value The desired state.
*/
void PRREObject3D::SetStickedToScreen(TPRREbool value)
{
    pImpl->SetStickedToScreen(value);
} // SetStickedToScreen()


/**
    Gets whether this object should be considered as an occluder during rendering.
    Occluders are rendered before non-occluder objects.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.

    @return True if considered as occluder, false otherwise (potential occludee).
*/
TPRREbool PRREObject3D::isOccluder() const
{
    return pImpl->isOccluder();
} // isOccluder()


/**
    Sets whether this object should be considered as an occluder during rendering.
    Occluders are rendered before non-occluder objects.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    By default this property is false.

    @param value The desired state.
*/
void PRREObject3D::SetOccluder(TPRREbool value)
{
    pImpl->SetOccluder(value);
} // SetOccluder()


/**
    Gets the bounding box object used for occlusion tests.
    @return The bounding box object used for occlusion tests. It might be PGENULL, if occlusion tests are not done for this object.
*/
const PRREObject3D* PRREObject3D::getBoundingBoxObject() const
{
    return pImpl->getBoundingBoxObject();
} // getBoundingBoxObject()


/**
    Gets the amount of allocated system memory.
    It includes the allocated Material size as well (getMaterial(false)).
    Level-1 (parent) objects summarize the memory usage of their level-2 subobjects and include it in the returned value.

    @return Amount of allocated system memory in Bytes.
*/
TPRREuint PRREObject3D::getUsedSystemMemory() const
{
    TPRREuint sumSubObjectMemoryUsage = 0;
    for (TPRREint i = 0; i < getSize(); i++)
    {
        if ( getAttachedAt(i) == PGENULL )
            continue;
        sumSubObjectMemoryUsage += getAttachedAt(i)->getUsedSystemMemory();
    }
    return sumSubObjectMemoryUsage +
        PRREFiledManaged::getUsedSystemMemory() - sizeof(PRREFiledManaged) +
        PRREMesh3D::getUsedSystemMemory() - sizeof(PRREMesh3D) +
        PRREVertexTransfer::getUsedSystemMemory() - sizeof(PRREVertexTransfer) + 
        sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    The details of this function are described in VertexTransfer class.
    If this is a cloned object, the returned value is 0.
    Small amount of extra video memory might be also used for the bounding box object utilized by this object.

    @return Amount of allocated video memory in Bytes for storing geometry of the underlying mesh, including all subobjects.
*/
TPRREuint PRREObject3D::getUsedVideoMemory() const
{
    TPRREuint vidMemory = 0;

    if ( getBoundingBoxObject() != PGENULL )
        vidMemory += getBoundingBoxObject()->getUsedVideoMemory();

    // cloned object doesnt have underlying mesh geometry
    if ( !getReferredObject() )
        vidMemory += PRREVertexTransfer::getUsedVideoMemory();

    return vidMemory;
} // getUsedVideoMemory()


/**
    Draws the object.
    This is only valid from outside for level-1 objects.
    The call is ignored by level-2 subobjects. Only their level-1 parent object can call on its subobjects.
*/
void PRREObject3D::Draw(const TPRRE_RENDER_PASS& renderPass)
{
    //pImpl->Draw_NaiveOcclusionQuery(renderPass);
    pImpl->Draw_NoOcclusionQuery(renderPass);
} // Draw()


// ############################## PROTECTED ##############################


/**
    Only PRREObject3DManager creates it.

    @param matMgr                A MaterialManager instance to be used for constructing of ancestor class.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.                             
*/
PRREObject3D::PRREObject3D(
    PRREMaterialManager& matMgr,
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory ) : PRREVertexTransfer(matMgr, vmod, vref, bForceUseClientMemory)
{
    getManagedConsole().OLnOI("PRREObject3D() ...");
    pImpl = new PRREObject3DImpl(this, vmod, vref, bForceUseClientMemory);
    getManagedConsole().SOLnOO("Done!");
} // PRREObject3D()


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREObject3D::PRREObject3D(const PRREObject3D& other)
    : PRREVertexTransfer(other)
{
}


PRREObject3D& PRREObject3D::operator=(const PRREObject3D&)
{
    return *this;
}


// ############################### PRIVATE ###############################



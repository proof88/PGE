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
#include "../PRREpragmas.h"
#include "../../../../PFL/PFL/PFL.h"
#include "PRREObject3DManager.h"
#include "PRREObject3DImpl.h"
#include "../PRREGLextensionFuncs.h"
#include "../PRREGLsnippets.h"
#include "../Hardware/PRREhwInfo.h"

using namespace std;

/*

    Notes

    ***************************

    GeForce 2 cards (and probably GeForce 4MX) only supported 16-bit indexing in HW, so
    if you went over 64Ki vertices then it would have to do the indexing in software.

    Some manual say that glDrawElements()' type parameter must be GL_UNSIGNED_BYTE or GL_UNSIGNED_SHORT, while some allow GL_UNSIGNED_INT too.
    Depends on implementation. This also means that we must use the smallest type given to glDrawElements() not only because of
    performance considerations but also in order to make the function work on older implementations.

    glDrawRangeElements() indices and vertices limits:
    http://www.opengl.org/registry/specs/EXT/draw_range_elements.txt
    The specification of glDrawElements does not allow optimal performance for some OpenGL implementations, however. 
    In particular, it has no restrictions on the number of indices given, the number of unique vertices referenced nor a
    direct indication of the set of unique vertices referenced by the given indices.  This forces some OpenGL
    implementations to walk the index data given, building up a separate list of unique vertex references for later use
    in the pipeline. Additionally, since some OpenGL implementations have internal limitations on how many vertices they can
    deal with simultaneously the unbounded nature of glDrawElements requires the implementation to be prepared to segment the
    input data and do multiple passes.  These preprocessing steps can consume a significant amount of time.
    glDrawRangeElementsEXT is designed to avoid the preprocessing steps which may be necessary for glDrawElements.
    As such it does not have the flexibility of glDrawElements but it is sufficiently functional for a large class of applications
    to benefit from its use. glDrawRangeElementsEXT enhances glDrawElements in two ways:
    1. The set of unique vertices referenced by the indices is explicitly indicated via the start and end parameters, removing the
        necessity to determine this through examination of the index data.  The implementation is given a contiguous chunk of vertex
        data that it can immediately begin streaming through the vertex pipeline.
    2. Recommended limits on the amount of data to be processed can be indicated by the implementation through GL_MAX_ELEMENTS_VERTICES_EXT
        and GL_MAX_ELEMENTS_INDICES_EXT. If an application respects these limits it removes the need to split the incoming data into multiple
        chunks since the maximums can be set to the optimal values for the implementation to handle in one pass.

    http://www.opengl.org/discussion_boards/showthread.php/146440-glDrawRangeElements-GL_MAX_ELEMENTS_VERTICES-INDICES
    However the performance effects of these caches has to do with the re-use of indices and
    the relative locality of vertex array contents. These performance effects aren't really
    related to the number of vertices actually processed by a particular glDrawRangeElements as
    the limits above are specified to describe.
    It's tempting to "re-interpret" these limits to be more "relevant" to how modern GPUs work, but
    that is not what the limits are specified to mean.

    http://stackoverflow.com/questions/7549991/questions-about-gldrawrangeelements
    Now that we know how to use glDrawElements, let's talk about glDrawRangeElements.
    When normally using glDrawElements, you specify a location in the source index array to pull from.
    The indices and count parameters tell OpenGL where to find the indices. But the actual indices pulled from
    this array could be anywhere within the boundaries of the source vertex array indices.
    glDrawRangeElements allows you to give OpenGL a range (half-open again) of vertex index values.
    What you are saying is that the indices it gets during this draw call will not exceed that range (half-open).
    This could allow the driver to perform useful optimizations. The start value should be the lowest index value
    that will be gotten from the index array, and the end should be the highest (+1 because it's half-open).
    It should not simply be the index of the first and last vertices.

    http://www.songho.ca/opengl/gl_vertexarray.html
    The additional parameters in glDrawRangeElements() are start and end index, then OpenGL prefetches a limited
    amount of vertices from these values: end - start + 1. And the values in index array must lie in between
    start and end index. Note that not all vertices in range (start, end) must be referenced. But, if you specify a
    sparsely used range, it causes unnecessary process for many unused vertices in that range.

    ***************************

    Regular VAs, compiled VAs, server-side VAs.

    Compiled vertex array: lock-unlock, but only 1 can be locked at a time, so the only use of this may be
    when the same geometry is rendered multiple times like multipass blending if no multitexturing available.
    Some performance gain may be observed also when using indices (drawElements) because shared vertices will be
    transformed and lit only once. This also means there is no such post-TnL cache optimisation with standard drawElements.

    Early vendor-specific "VBO"s:
     - GeForce2: VAR: NV_vertex_array_range, NV_vertex_array_range2
       VRAM or AGP memory. Write is slow. Write is fast only if Fast Writes is supported.
       Rule: allocate static geometry in VRAM, allocate AGP mem for dynamic geometry.
       Modifying AGP memory is slow, so first work in system memory, then block-copy to AGP memory.
     - VAO: ATI_vertex_array_object, ATI_map_object_buffer, ATI_element_array (like ELEMENT_ARRAY_BUFFER_ARB target).
       Can't be AGP mem, only VRAM.

    Usage params:
    - ATI_vertex_array_object: STATIC_ATI, DYNAMIC_ATI
    - NV_vertex_array_range: readFreq, writeFreq, priority
    - ARB_vertex_buffer_object: STATIC, STREAM, DYNAMIC; DRAW, READ, COPY


    BufferObject: a GPU allocated memory buffer
	    Vertex Buffer Object: a BufferObject containing vertices informations (colors, position, custom data used by a shader, ...)
	    Pixel Buffer Object: a BufferObject containing pixel or texel informations. Mainly used to upload textures.
	    Element Buffer Object: a BufferObject containing indices (used by glDrawElements).
    Vertex Array: memory used by gl*Pointer call.
	    Might be host memory or a Vertex Buffer Object if it is bound using glBindBuffer command with GL_ARRAY_BUFFER.
    Element Array: memory used by glDrawElements call.
	    Might be host memory or an Element Buffer Object if it is bound using glBindBuffer command with GL_ELEMENT_ARRAY_BUFFER.


    ***************************

    GL_ATI_pn_triangles: ATI TruForm, Radeon 8500.

    ***************************

*/


/*
   PRREObject3D::PRREObject3DImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


TPRRE_PRIMITIVE_FORMAT PRREObject3D::PRREObject3DImpl::getPRREprimitiveFromGLprimitive(GLenum glprim)
{
    switch (glprim)
    {
    case GL_TRIANGLES      : return PRRE_PM_TRIANGLES;
    case GL_TRIANGLE_STRIP : return PRRE_PM_TRIANGLE_STRIPS;
    case GL_TRIANGLE_FAN   : return PRRE_PM_TRIANGLE_FANS; 
    case GL_QUADS          : return PRRE_PM_QUADS;
    case GL_QUAD_STRIP     : return PRRE_PM_QUAD_STRIPS;
    case GL_LINES          : return PRRE_PM_LINES;
    case GL_LINE_STRIP     : return PRRE_PM_LINE_STRIPS;
    default                : return PRRE_PM_POINTS;    
    }
} // getPRREprimitiveFromGLprimitive()


GLenum PRREObject3D::PRREObject3DImpl::getGLprimitiveFromPRREprimitive(TPRRE_PRIMITIVE_FORMAT pf)
{
    switch (pf)
    {
    case PRRE_PM_TRIANGLES       : return GL_TRIANGLES;
    case PRRE_PM_TRIANGLE_STRIPS : return GL_TRIANGLE_STRIP;
    case PRRE_PM_TRIANGLE_FANS   : return GL_TRIANGLE_FAN; 
    case PRRE_PM_QUADS           : return GL_QUADS;
    case PRRE_PM_QUAD_STRIPS     : return GL_QUAD_STRIP;
    case PRRE_PM_LINES           : return GL_LINES;
    case PRRE_PM_LINE_STRIPS     : return GL_LINE_STRIP;
    default                      : return GL_POINTS;
    }
} // getGLprimitiveFromPRREprimitive()


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
    getConsole().OLnOI("~PRREObject3D() ...");
    
    FreeGLresources();

    free(pVerticesTransf);
    free(pFbBuffer);
    pVerticesTransf = PGENULL;
    pFbBuffer = PGENULL;

    _pOwner->DeleteAll();

    // if we are just a cloned object, above code should have done essentially nothing
    if ( pRefersto )
    {
        // TODO: probably we should decrement reference count in referred object ...
    }

    getConsole().SOLnOO("Done!");
} // ~PRRETexture()


PRREObject3D* PRREObject3D::PRREObject3DImpl::getReferredObject() const
{
    return pRefersto; 
}


TPRRE_VERTEX_MODIFYING_HABIT PRREObject3D::PRREObject3DImpl::getVertexModifyingHabit() const
{
    return BIT(PRRE_VT_VMOD_BIT) & vertexTransferMode;
} // getVertexModifyingHabit()


void PRREObject3D::PRREObject3DImpl::SetVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    SetVertexTransferMode( PRREObject3DManager::selectVertexTransferMode(vmod, getVertexReferencingMode(), false) );
} // SetVertexModifyingHabit()


TPRRE_VERTEX_REFERENCING_MODE PRREObject3D::PRREObject3DImpl::getVertexReferencingMode() const
{
    return BIT(PRRE_VT_VREF_BIT) & vertexTransferMode;
} // getVertexReferencingMode()

 
void PRREObject3D::PRREObject3DImpl::SetVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    SetVertexTransferMode( PRREObject3DManager::selectVertexTransferMode(getVertexModifyingHabit(), vref, false) );
} // SetVertexReferencingMode()


TPRRE_VERTEX_TRANSFER_MODE PRREObject3D::PRREObject3DImpl::getVertexTransferMode() const
{
    return vertexTransferMode;
} // getVertexTransferMode()


void PRREObject3D::PRREObject3DImpl::SetVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    // TODO DO NOT LET THIS FUNCTION DO ANYTHING IF THE OBJECT IS LEVEL-2!!!
    getConsole().OLnOI("PRREObject3D::SetVertexTransferMode()");

    // TODO: DO NOT DO ANYTHING IF WE ARE CLONED OBJECT!

    // Note: we don't check if the selected transfer mode is already selected.
    // Reason: selecting an already selected transfer mode is a way to recreate buffers / refresh geometry.
    // Modify geometry in system RAM and simply issue SetVertexTransferMode( getVertexTransferMode() ) to apply changes.

    if ( PRREObject3DManager::isVertexReferencingIndexed(vertexTransferMode) &&
         !PRREObject3DManager::isVertexReferencingIndexed(vtrans) )
    {
        if ( !isSwitchFromIndexedAllowed() )
        {
            getConsole().EOLnOO("ERROR: failed to switch from indexed to direct referencing mode!");
            return;
        }
    }

    if ( !PRREObject3DManager::isVertexTransferModeSelectable(vtrans) )
    {
        getConsole().EOLnOO("ERROR: given mode %d is unavailable!", (int)vtrans);
        return;
    }

    FreeGLresources();   // now it's safe to delete GL resources

    if ( BIT_READ(vtrans, PRRE_VT_SVA_BIT) )
    {
        /* check if we want VBO? */
        CompileIntoVertexBufferObjects(
            PRREObject3DManager::isVertexReferencingIndexed(vtrans),
            PRREObject3DManager::isVertexModifyingDynamic(vtrans) );
    }
    else
    {
        /* check if we want display list? */
        if ( (vtrans == (PRRE_VMOD_STATIC | PRRE_VREF_DIRECT)) ||
            (vtrans == (PRRE_VMOD_STATIC | PRRE_VREF_INDEXED)) )
            CompileIntoDisplayList(PRREObject3DManager::isVertexReferencingIndexed(vtrans));
    }

    /* no other transfer mode needs further preparation */
    vertexTransferMode = vtrans;

    if ( vertexTransferMode == vtrans )
    {   // if succeeded, apply the new transfer mode to the subobjects too
        getConsole().OLn("Ok, applying for subobjects ...");
        for (TPRREint i = 0; i < _pOwner->getCount(); i++)
            ((PRREObject3D*)(_pOwner->getAttachedAt)(i))->SetVertexTransferMode( vertexTransferMode );
        getConsole().OLn("Done!");
    }

    getConsole().OO();
} // SetVertexTransferMode()


TPRRE_TRANSFORMED_VERTEX* PRREObject3D::PRREObject3DImpl::getTransformedVertices(TPRREbool implicitAccessSubobject)
{
    if ( implicitAccessSubobject && (_pOwner->getVerticesCount(false) == 0) && (_pOwner->getCount() == 1) )
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
    const PRREVector tmp(
        _pOwner->getSizeVec().getX() * vScaling.getX(),
        _pOwner->getSizeVec().getY() * vScaling.getY(),
        _pOwner->getSizeVec().getZ() * vScaling.getZ());
    return tmp;
} // getScaledSizeVec()


const PRREVector& PRREObject3D::PRREObject3DImpl::getScaling() const
{
    return vScaling;
} // getScaling()


void PRREObject3D::PRREObject3DImpl::SetScaling(TPRREfloat value)
{
    // TODO: ignore the setting if this is level-2 object!!!
    vScaling.Set(value, value, value);
} // SetScaling()


void PRREObject3D::PRREObject3DImpl::SetScaling(const PRREVector& value)
{
    // TODO: ignore the setting if this is level-2 object!!!
    vScaling = value;
} // SetScaling()


void PRREObject3D::PRREObject3DImpl::Scale(TPRREfloat value)
{
    // TODO: ignore the setting if this is level-2 object!!!
    vScaling.Set( vScaling.getX() * value, vScaling.getY() * value, vScaling.getZ() * value );
} // Scale()


void PRREObject3D::PRREObject3DImpl::Scale(const PRREVector& value)
{
    // TODO: ignore the setting if this is level-2 object!!!
    vScaling.Set( vScaling.getX() * value.getX(), vScaling.getY() * value.getY(), vScaling.getZ() * value.getZ() );
} // Scale()


TPRREbool PRREObject3D::PRREObject3DImpl::isVisible() const
{
    return bVisible;
} // isVisible()


void PRREObject3D::PRREObject3DImpl::SetVisible(TPRREbool state)
{
    // TODO: ignore the setting if this is level-2 object!!!
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
    // TODO: ignore this if this is a level-2 subobject!
    rotation = value;
} // SetRotationOrder()


TPRREbool PRREObject3D::PRREObject3DImpl::isLit() const
{
    return bAffectedByLights;
} // isLit()


void PRREObject3D::PRREObject3DImpl::SetLit(TPRREbool value)
{
    // TODO: ignore this if this is a level-2 subobject!
    bAffectedByLights = value;
} // SetLit()


TPRREbool PRREObject3D::PRREObject3DImpl::isDoubleSided() const
{
    return bDoubleSided;
} // isDoubleSided()


void PRREObject3D::PRREObject3DImpl::SetDoubleSided(TPRREbool value)
{
    // TODO: ignore this if this is a level-2 subobject!
    bDoubleSided = value;
} // SetDoubleSided()


TPRREbool PRREObject3D::PRREObject3DImpl::isWireframed() const
{
    return bWireframe;
} // isWireframed()


void PRREObject3D::PRREObject3DImpl::SetWireframed(TPRREbool value)
{
    // TODO: ignore this if this is a level-2 subobject!
    bWireframe = value;
} // SetWireframed()


TPRREbool PRREObject3D::PRREObject3DImpl::isWireframedCulled() const
{
    return bWireframedCull;
} // isWireframedCulled()


void PRREObject3D::PRREObject3DImpl::SetWireframedCulled(TPRREbool value)
{
    // TODO: ignore this if this is a level-2 subobject!
    bWireframedCull = value;
} // SetWireframedCulled()


TPRREbool PRREObject3D::PRREObject3DImpl::isAffectingZBuffer() const
{
    return bAffectZBuffer;
} // isAffectingZBuffer()


void PRREObject3D::PRREObject3DImpl::SetAffectingZBuffer(TPRREbool value)
{
    // TODO: ignore this if this is a level-2 subobject!
    bAffectZBuffer = value;
} // SetAffectingZBuffer()


TPRREbool PRREObject3D::PRREObject3DImpl::isTestingAgainstZBuffer() const
{
    return bAllowZTesting;
} // isTestingAgainstZBuffer()


void PRREObject3D::PRREObject3DImpl::SetTestingAgainstZBuffer(TPRREbool value)
{
    // TODO: ignore this if this is a level-2 subobject!
    bAllowZTesting = value;
} // SetTestingAgainstZBuffer()


TPRREbool PRREObject3D::PRREObject3DImpl::isStickedToScreen() const
{
    return bStickedToScreen;
} // isStickedToScreen()


void PRREObject3D::PRREObject3DImpl::SetStickedToScreen(TPRREbool value)
{
    // TODO: ignore this if this is a level-2 subobject!
    bStickedToScreen = value;
    SetDoubleSided(true);
    SetLit(false);
    SetTestingAgainstZBuffer(false);
} // SetStickedToScreen()


TPRREuint PRREObject3D::PRREObject3DImpl::getUsedSystemMemory() const
{
    return (
        sizeof(*this) +
        nFbBuffer_h
    );
} // getUsedSystemMemory()     


void PRREObject3D::PRREObject3DImpl::Draw(bool bLighting)
{
    if ( !bVisible )
        return;

    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        DrawSW();
        return;
    }

     if ( (_pOwner->getSizeVec().getX() == 2.0f) && (_pOwner->getSizeVec().getY() == 2.0f) )
     {
         // for placing breakpoint ...
         getConsole().OLn("");
     }

    // TODO: add a mechanism for ignoring user-triggered draw of subobjects.
    // This can be done if parent maintains a variable that is set to true
    // at the beginning of draw and its subobjects check for the value of this var,
    // and draw only if that is true. At the end, parent resets this to false.
    // TODO: add public comment about this once implemented!

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( (_pOwner->getCount() > 0) || pRefersto )
    {
        // Currently 3D objects are 2-level entities:
        // first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.
        
        ApplyTransformations();
        PrepareGLbeforeDraw(bLighting);

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = pRefersto ? pRefersto : _pOwner;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(bLighting);
    }

    // before trying to draw anything, see if we actually have anything to draw ...
    if ( (_pOwner->getVertexIndicesCount(false) == 0) && (_pOwner->getVerticesCount(false) == 0) && (nVerticesVBO == 0) && (nDispList == 0) )
        return;  // we return here if we are parent or we are cloned object

    // if we reach this point then either our parent is drawing us as its subobject, or a cloned object is drawing us on behalf of our parent
    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        if ( _pOwner->getMaterial().isMultiTextured() )
        {
            // enable blending of 2nd layer
            glEnable(GL_BLEND);
	          glBlendFunc(getGLblendFromPRREblend(_pOwner->getMaterial().getSourceBlendFunc(1)), getGLblendFromPRREblend(_pOwner->getMaterial().getDestinationBlendFunc(1)));
            LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(0), 0 );
            LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(1), 1 );
        }
        else
        {
            // disable blending only if base layer is not blended ...
            if ( ! PRREObject3DManager::isBlendFuncBlends(_pOwner->getMaterial().getSourceBlendFunc(), _pOwner->getMaterial().getDestinationBlendFunc()) )
                glDisable(GL_BLEND);
            LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(0), 0 );
            LoadTextureIntoTMU( PGENULL, 1 );
        }
    }
    else
    {
        // disable blending only if base layer is not blended ...
        if ( ! PRREObject3DManager::isBlendFuncBlends(_pOwner->getMaterial().getSourceBlendFunc(), _pOwner->getMaterial().getDestinationBlendFunc()) )
            glDisable(GL_BLEND);
        LoadTextureIntoTMU( _pOwner->getMaterial().getTexture() );
    }

    /* currently not supporting any vendor-specific mode */
    if ( BITF_READ(getVertexTransferMode(),PRRE_VT_VENDOR_BITS,3) != 0 )
        return;

    if ( pFbBuffer == NULL )
    {
        /* We create this buffer only once at first run. */
        /* We intentionally switch to feedback mode only once since now it is for debugging purposes only, for breakpoint, etc.
           but in the future when HW transform and SW rasterization can be combined, this will be controlled by public API */
       
        /* num of values can be stored in this buffer */
        /* we need to store not only the transformed values but also some extra values for grammar */
        /* in this case, the items placed in this array are polygon items (GL_POLYGON_TOKEN) which look like this:
            GL_POLYGON_TOKEN 3 (value value value value (value value value value) (value value value value))
            So for a standard cube where each side has 2 triangles, it is 12 triangles in total, meaning
            12 polygons. GL_POLYGON_TOKEN itself is 1 value, 3 itself is also 1 value, and then 3*12 values for 3 vertices.
            So the total num of values is: numOfTriangles * (2+3*12)
            Example with some real data is in later comment with debugbuffer[]. */

       
        nFbBuffer_h = GLsizei(ceil((_pOwner->getVerticesCount(false) / 3.0f))) * (2+3*12);
        /* TODO: probably in future we should rather use pVerticesTransf here as well since we already have it for that purpose, right? ;) */
        pFbBuffer = (GLfloat*) realloc(pFbBuffer, nFbBuffer_h * sizeof(GLfloat));

        /* unfortunately only the most detailed option GL_4D_COLOR_TEXTURE will give us the w-coord of vertices so we need to use that */
        glFeedbackBuffer(nFbBuffer_h, GL_4D_COLOR_TEXTURE, pFbBuffer);
        
        // If you want all vertices to be transformed and catched in feedback mode then dont forget to disable culling and depth testing (maybe only 1 is needed to be disabled).
        //glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE); /* otherwise only the front facing side of cube would be written to feedback buffer */

        glRenderMode(GL_FEEDBACK);
    }  

    if ( BIT_READ(getVertexTransferMode(), PRRE_VT_VA_BIT) == 0u )
    {
        /* there is no array for geometry, so we either invoke a display list or pass vertices 1-by-1 */

        if ( PRREObject3DManager::isVertexModifyingDynamic( getVertexTransferMode() ) )
            // PRRE_VT_DYN_..._1_BY_1
            ProcessGeometry( PRREObject3DManager::isVertexReferencingIndexed( getVertexTransferMode() ) );
        else
            // PRRE_VT_STA_..._DL
            glCallList( nDispList );
    } // PRRE_VT_VA_BIT == 0u
    else
    {
        /* vertex array either in client or server */
        const bool bServer = BIT_READ(getVertexTransferMode(), PRRE_VT_SVA_BIT) == 1u;

        SetArrayPointers( bServer );

            // There is not much use of the CVA mode in this form, however sometime later
            // the code could be somehow enhanced to do multitexturing with just 1 TMU
            // by drawing the same geometry again with different texture blended between
            // 1 lock-unlock, in that case vertices would be transformed only once!
            // This would be useful when multitexturing is enabled on 1-TMU VPU/GPU.
            // However even TNT had 2 TMUs, so I'm not planning to do this improvement.
            // Just wanted to emphasize that locking the array and drawing it only once
            // does not make much sense.
            if ( BIT_READ(getVertexTransferMode(), PRRE_VT_CVA_BIT) == 1u )
                glLockArraysEXT(0, _pOwner->getVerticesCount(false));

                // Static and dynamic vmods are not differentiated here but when creating the arrays.
                // That is why we don't check for vmod in this code.

                if ( PRREObject3DManager::isVertexReferencingIndexed( getVertexTransferMode() ) )
                {
                    if ( BIT_READ(getVertexTransferMode(), PRRE_VT_RNG_BIT) )
                    {
                        // TODO: should call this in loop, do multiple batches based on the implementation-dependent values
                        // (GL_MAX_ELEMENTS_VERTICES_EXT, GL_MAX_ELEMENTS_INDICES_EXT).
                        glDrawRangeElementsEXT(
                            getGLprimitiveFromPRREprimitive(_pOwner->getPrimitiveFormat()),
                            _pOwner->getMinIndexValue(false), _pOwner->getMaxIndexValue(false),
                            _pOwner->getVertexIndicesCount(false), _pOwner->getVertexIndicesType(false), bServer ? NULL : _pOwner->getVertexIndices(false) );
                    }
                    else
                    {
                        glDrawElements(
                            getGLprimitiveFromPRREprimitive(_pOwner->getPrimitiveFormat()), 
                            _pOwner->getVertexIndicesCount(false), _pOwner->getVertexIndicesType(false), bServer ? NULL : _pOwner->getVertexIndices(false) );
                    }
                }
                else
                {
                    glDrawArrays( getGLprimitiveFromPRREprimitive(_pOwner->getPrimitiveFormat()), 0, _pOwner->getVerticesCount(false) );
                }

            if ( BIT_READ(getVertexTransferMode(), PRRE_VT_CVA_BIT) == 1u )
                glUnlockArraysEXT();

        ResetArrayPointers( bServer );
    } // PRRE_VT_VA_BIT == 1u

    /* The number of values (not vertices) transferred to the feedback buffer. */
    GLint nFbBufferWritten_h = glRenderMode(GL_RENDER);

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

} // Draw()


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
    getConsole().OLnOI("PRREObject3D() ...");

    pRefersto = NULL;
    bAffectedByLights = bAffectZBuffer = bAllowZTesting = true;
    bReadyToDraw = bVisible = true;
    bColliding = bDoubleSided = false;
    bWireframe = bWireframedCull = bStickedToScreen = false;

    vScaling.Set(1.0f, 1.0f, 1.0f);
    rotation = PRRE_YXZ;
    nDispList = 0;
    nVerticesVBO = nColorsVBO = nNormalsVBO = nIndicesVBO = 0;
    // nTexcoordsVBO to be resized by manager outside

    vertexTransferMode = PRREObject3DManager::selectVertexTransferMode(vmod, vref, bForceUseClientMemory);
    pVerticesTransf = PGENULL;
    pFbBuffer = PGENULL;

    nFbBuffer_h = 0;

    getConsole().SOLnOO("Done!");
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
    Hack to be able to use CConsole singleton instance instead of owner's protected console instance.
    TODO: maybe the whole getConsole() stuff should be removed from everywhere because CConsole is a singleton class after all.
*/
CConsole& PRREObject3D::PRREObject3DImpl::getConsole() const
{
    return CConsole::getConsoleInstance();
}


/**
    Tells whether it is allowed to switch from indexed to non-indexed vertex referencing mode.
    It is allowed when geometry data is stored redundantly so direct referencing is available.
    Geometry data is stored redundantly when multiple vertices are defined with the same position
    for different faces. In this situation, the number of vertex indices is equal to the number of
    defined vertices so the number of defined vertices is the adequate number for rendering the
    whole geometry. So switching to non-indexed is disallowed only when the number of defined
    vertices is less then the number of indices.
*/
TPRREbool PRREObject3D::PRREObject3DImpl::isSwitchFromIndexedAllowed() const
{
    // TODO: ALL subobjects should check if their vertex index count equals to vertex count, 1 false is enough to return false!
    if ( _pOwner->getCount() > 0 )
        return ((PRREObject3D*)_pOwner->getAttachedAt(0))->p->isSwitchFromIndexedAllowed();
    else
        return (_pOwner->getVertexIndicesCount(false) == _pOwner->getVerticesCount(false)) && (_pOwner->getVerticesCount(false) > 0);
}


/**
    Goes thru vertices and feeds them to OpenGL.
    Used by any vertex transfer mode not utilizing vertex arrays, such as:
     - immediate mode;
     - display lists.
    Handles both direct and indexed vertex reference modes.

    @param indexed If true, will go thru vertices by using pVertexIndices for ordering, otherwise won't use it.
*/
void PRREObject3D::PRREObject3DImpl::ProcessGeometry(TPRREbool indexed) const  
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    //const TRGBAFLOAT* const pColors = _pOwner->getMaterial(false).getColors(0);
    //const void* const pColorIndices = _pOwner->getMaterial(false).getColorIndices(0);
    const TUVW* const pTexcoords = _pOwner->getMaterial(false).getTexcoords(0);
    // following can be NULL if material system supports only 1 level
    const TUVW* const pTexcoords2 = _pOwner->getMaterial(false).getTexcoords(1);
    //const void* const pTexcoordIndices2 = _pOwner->getMaterial(false).getTexcoordIndices(1);

    glBegin( getGLprimitiveFromPRREprimitive(_pOwner->getPrimitiveFormat()) );
        for (TPRREuint i = 0; i < _pOwner->getVertexIndicesCount(false); i++)
        {
            const TXYZ&       vertex = indexed ? _pOwner->getVertices(false)[ _pOwner->getIndexFromArray(_pOwner->getVertexIndices(false), i) ]    : _pOwner->getVertices(false)[i];
            const TXYZ&       normal = indexed ? _pOwner->getNormals(false)[ _pOwner->getIndexFromArray(_pOwner->getVertexIndices(false), i) ]     : _pOwner->getNormals(false)[i];
            //const TRGBAFLOAT& color  = indexed ? pColors[ _pOwner->getIndexFromArray(pColorIndices, i) ]       : pColors[i];
            const TUVW&       uvw    = indexed ? pTexcoords[ _pOwner->getIndexFromArray(_pOwner->getVertexIndices(false), i) ] : pTexcoords[i];

            if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
            {
                if ( _pOwner->getMaterial(false).isMultiTextured() )
                {
                    const TUVW& uvw2 = indexed ? pTexcoords2[ _pOwner->getIndexFromArray(_pOwner->getVertexIndices(false), i) ] : pTexcoords2[i];
                    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, uvw.u, uvw.v);
                    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, uvw2.u, uvw2.v);
                }
                else
                {
                    if ( _pOwner->getMaterial(false).getTexture() != PGENULL )
                        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, uvw.u, uvw.v);
                }
            }
            else
            {
                if ( _pOwner->getMaterial(false).getTexture() != PGENULL )
                    glTexCoord2f(uvw.u, uvw.v);
            }
            
            // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere in setarraypointers()
            //glColor4f(color.red, color.green, color.blue, color.alpha);
            if ( bAffectedByLights )
                glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vertex.x, vertex.y, vertex.z);
        } // for k
    glEnd();
} // ProcessGeometry()


/**
    Compiles OpenGL drawing commands into display list.

    @param indexed If true, will issue ProcessGeometry() to go thru geometry data by using indices for order.
*/
void PRREObject3D::PRREObject3DImpl::CompileIntoDisplayList(TPRREbool indexed)
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( _pOwner->getVertices(false) == NULL )
        return;

    nDispList = glGenLists(1);
    glNewList(nDispList, GL_COMPILE);
        ProcessGeometry( indexed );
    glEndList();
} // CompileIntoDisplayList()


 /**
    Compiles geometry into VBOs.

    @param indexed If true, will go thru geometry data by using indices for order. This will generate an element array buffer.
                   Ignored if vertex indices array is NULL.
    @param dynamic If true, the usage hint when building vertex buffer objects will be dynamic, otherwise static.
 */
void PRREObject3D::PRREObject3DImpl::CompileIntoVertexBufferObjects(TPRREbool indexed, TPRREbool dynamic)
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( indexed && (nIndicesVBO == 0) && (_pOwner->getVertexIndices(false) != NULL) )
    {
        glGenBuffersARB(1, &nIndicesVBO);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nIndicesVBO);
        glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _pOwner->getVertexIndicesCount(false) * PRREGLsnippets::getSizeofIndexType(_pOwner->getVertexIndicesType()), _pOwner->getVertexIndices(false), GL_STATIC_DRAW_ARB);
        // Note: we always store indices in static buffer but could be in client memory, too.
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }

    if ( (nVerticesVBO == 0) && (_pOwner->getVertices(false) != NULL) )
    {
        GLenum usage = dynamic ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB;
        glGenBuffersARB(1, &nVerticesVBO);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nVerticesVBO);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,  _pOwner->getVerticesCount(false) * sizeof(TXYZ), _pOwner->getVertices(false), usage);
 
        glGenBuffersARB(1, &nColorsVBO);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nColorsVBO);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, _pOwner->getMaterial(false).getColorsCount() * sizeof(TRGBAFLOAT), _pOwner->getMaterial(false).getColors(), usage);

        glGenBuffersARB(1, &nTexcoordsVBO[0]);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[0]);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, _pOwner->getMaterial(false).getTexcoordsCount() * sizeof(TUVW), _pOwner->getMaterial(false).getTexcoords(), usage);

        if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
        {
            if ( _pOwner->getMaterial(false).isMultiTextured() )
            {
                glGenBuffersARB(1, &nTexcoordsVBO[1]);
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[1]);
                glBufferDataARB(GL_ARRAY_BUFFER_ARB, _pOwner->getMaterial(false).getTexcoordsCount(1) * sizeof(TUVW), _pOwner->getMaterial(false).getTexcoords(1), usage);
            }
        }

        glGenBuffersARB(1, &nNormalsVBO);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nNormalsVBO);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, _pOwner->getNormalsCount(false) * sizeof(TXYZ), _pOwner->getNormals(false), usage);

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    }

} // CompileIntoVertexBufferObjects()


/**
    Frees up allocated resources from OpenGL.
    Should be called before setting a new Vertex Transfer mode and when deleting the object.
*/
void PRREObject3D::PRREObject3DImpl::FreeGLresources()
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    for (TPRREint i = 0; i < _pOwner->getCount(); i++)
        ((PRREObject3D*)_pOwner->getAttachedAt(i))->p->FreeGLresources();

    if ( nDispList != 0 )
    {
        glDeleteLists(nDispList, 1);
        nDispList = 0;
    }
    if ( nVerticesVBO != 0 )
    {
        glDeleteBuffersARB(1, &nVerticesVBO);
        nVerticesVBO = 0;
    }
    if ( nColorsVBO != 0 )
    {
        glDeleteBuffersARB(1, &nColorsVBO);
        nColorsVBO = 0;
    }
    if ( nNormalsVBO != 0 )
    {
        glDeleteBuffersARB(1, &nNormalsVBO);
        nNormalsVBO = 0;
    }
    if ( nIndicesVBO != 0 )
    {
        glDeleteBuffersARB(1, &nIndicesVBO);
        nIndicesVBO = 0;
    }

    for (std::vector<GLuint>::size_type i = 0; i < nTexcoordsVBO.size(); i++)
    {
        if ( nTexcoordsVBO[i] != 0 )
        {
            glDeleteBuffersARB(1, &nTexcoordsVBO[i]);
            nTexcoordsVBO[i] = 0;
        }
    }
}


/**
    Sets vertex data pointers prior to drawing.
    Can be used for both vertex buffer objects and legacy vertex arrays.

    @param redirectToServer Specify true if geometry data is located in server memory.
*/
void PRREObject3D::PRREObject3DImpl::SetArrayPointers(TPRREbool redirectToServer) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( (PRREObject3DManager::isVertexReferencingIndexed(vertexTransferMode)) && redirectToServer )
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nIndicesVBO);

    glEnableClientState(GL_VERTEX_ARRAY);
    // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason:
    // as tmcsgfxlib uses a single vertex color for object color key when object is textured,
    // we shouldn't specify any further per-vertex color, otherwise alphablending would be bad with legacy projects.
    // In future, we should rather use glBlendColor() for such objects and then per-vertex colors could be re-enabled again.
    // glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    if ( redirectToServer )
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nVerticesVBO);
        glVertexPointer(3, GL_FLOAT, 0, NULL);

        // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere above
        //glBindBufferARB(GL_ARRAY_BUFFER_ARB, nColorsVBO);
        //glColorPointer(4, GL_FLOAT, 0, NULL);

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nNormalsVBO);
        glNormalPointer(GL_FLOAT, 0, NULL);
    }
    else
    {
        glVertexPointer(3, GL_FLOAT, 0, _pOwner->getVertices(false));
        // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere above
        //glColorPointer(4, GL_FLOAT, 0, _pOwner->getMaterial(false).getColors());
        glNormalPointer(GL_FLOAT, 0, _pOwner->getNormals(false));
    }

    if ( _pOwner->getMaterial(false).isTextured() )
    {
        const TPRREuint iMaxTexLayer = _pOwner->getMaterial(false).isMultiTextured() ? 1 : 0;
        for (TPRREuint i = 0; i <= iMaxTexLayer; i++)
        {
            if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
                glClientActiveTextureARB(GL_TEXTURE0_ARB + i);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            if ( redirectToServer )
            {
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[i]);
                glTexCoordPointer(3, GL_FLOAT, 0, NULL);
            }
            else
                glTexCoordPointer(3, GL_FLOAT, 0, _pOwner->getMaterial(false).getTexcoords(i));
        }
    }

} // SetArrayPointers()


/**
    Resets vertex data pointers after drawing.
    Can be used for both vertex buffer objects and legacy vertex arrays.

    @param redirectToServer Specify true if geometry data is located in server memory.
*/
void PRREObject3D::PRREObject3DImpl::ResetArrayPointers(TPRREbool redirectToServer) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( redirectToServer )
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        if ( PRREObject3DManager::isVertexReferencingIndexed(vertexTransferMode) )
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }
    if ( _pOwner->getMaterial(false).isTextured() )
    {
        const TPRREuint iMaxTexLayer = _pOwner->getMaterial(false).isMultiTextured() ? 1 : 0;
        for (TPRREuint i = 0; i <= iMaxTexLayer; i++)
        {
            if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
                glClientActiveTextureARB(GL_TEXTURE0_ARB + i);

            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
} // ResetArrayPointers()


/**
    Loads the given texture into the texture mapping unit.

    @param tex  Texture to be loaded. If NULL, texturing will be disabled on the specified texture mapping unit.
    @param iTMU Into which TMU we want to load the texture. Currently it must be either 0 or 1.
*/
void PRREObject3D::PRREObject3DImpl::LoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    // TODO: the PR00FPS-style lasttex method is not present here currently ... later it should be implemented.
    // A general TMU manager should record which TMU holds which texture, so that can avoid unnecessary glBindTexture() calls.

    // currently we support only 2 textured layers ...
    if ( iTMU > 1 )
    {
        getConsole().EOLn("ERROR: LoadTextureIntoTMU(tex, %d)", iTMU);
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
} // LoadTextureIntoTMU()


/**
    Applies transformations to the current modelview matrix based on the given object.
*/
void PRREObject3D::PRREObject3DImpl::ApplyTransformations() const    
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
}


void PRREObject3D::PRREObject3DImpl::PrepareGLbeforeDraw(bool bLighting) const
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
}


void PRREObject3D::PRREObject3DImpl::DrawSW()
{
    if ( !bVisible )
        return;

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( (_pOwner->getCount() > 0) || pRefersto )
    {
        // Currently 3D objects are 2-level things: first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.

        ApplyTransformations();

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = pRefersto ? pRefersto : _pOwner;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(false);
    }

    // before trying to draw anything, see if we actually have anything to draw ...
    if ( (_pOwner->getVertexIndicesCount(false) == 0) && (_pOwner->getVerticesCount(false) == 0) )
        return;  // we return here if we are parent or we are cloned object

    // actual draw here
    // todo: wtf, we should finally decide if it is renderer's responsibility to actually render an object or object's responsibility?
}


/*
   PRREObject3D
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREObject3D::~PRREObject3D()
{
    delete p;
    p = NULL;
} // ~PRRETexture()


/**
    Gets the original object which was cloned to create this object.
    @return NULL if this is a non-cloned object, otherwise the pointer to the original object which was cloned to create this cloned object.
*/
PRREObject3D* PRREObject3D::getReferredObject() const
{
    return p->getReferredObject();
}


/**
    Gets vertex modifying habit.
    It applies to the entire geometry of the object, including all of its subobjects.
    Level-2 objects use the property of their 
    parent object.
*/
TPRRE_VERTEX_MODIFYING_HABIT PRREObject3D::getVertexModifyingHabit() const
{
    return p->getVertexModifyingHabit();
} // getVertexModifyingHabit()


/**
    Sets vertex modifying habit.
    It applies to the entire geometry of the object, including all of its subobjects.
    The setting is ignored for level-2 objects as it is applied by their level-1 parent object.
*/
void PRREObject3D::SetVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    p->SetVertexModifyingHabit(vmod);
} // SetVertexModifyingHabit()


/**
    Gets vertex referencing mode.
    It applies to the entire geometry of the object, including all of its subobjects.
    Level-2 objects use the property of their level-1 parent object.
*/
TPRRE_VERTEX_REFERENCING_MODE PRREObject3D::getVertexReferencingMode() const
{
    return p->getVertexReferencingMode();
} // getVertexReferencingMode()

 
/**
    Sets vertex referencing mode.
    It applies to the entire geometry of the object, including all of its subobjects.
    The setting is ignored for level-2 objects as it is applied by their level-1 parent object.
*/
void PRREObject3D::SetVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    p->SetVertexReferencingMode(vref);
} // SetVertexReferencingMode()


/**
    Gets vertex transfer mode.
    It applies to the entire geometry of the object, including all of its subobjects.
    Level-2 objects use the property of their level-1 parent object.
*/
TPRRE_VERTEX_TRANSFER_MODE PRREObject3D::getVertexTransferMode() const
{
    return p->getVertexTransferMode();
} // getVertexTransferMode()


/**
    Sets vertex transfer mode.
    Vertex transfer mode gets selected automatically by PPP.
    However, it can be also set manually for custom reasons.
    The setting may not happen if the selected transfer mode is not available for some reason, for
    example, if the selected mode is not supported on the current hardware.
    It applies to the entire geometry of the object, including all of its subobjects.
    The setting is ignored for level-2 objects as it is applied by their level-1 parent object.
*/
void PRREObject3D::SetVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    p->SetVertexTransferMode(vtrans);
} // SetVertexTransferMode()


/**
    Gets the pointer to transformed vertices.
    These are generated after rendering.

    @return Pointer to transformed vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's transformed vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRRE_TRANSFORMED_VERTEX* PRREObject3D::getTransformedVertices(TPRREbool implicitAccessSubobject)
{
    // TODO: add unit test for TPRREbool implicitAccessSubobject parameter!
    return p->getTransformedVertices(implicitAccessSubobject);
} // getVertices(false)


/**
    Gets the rotation angles.
    Rotation angle is currently ignored for level-2 objects, they are rotated by the same factor as their level-1 parent object using the same pivot point.
    @return Rotation angles vector.
*/
PRREVector& PRREObject3D::getAngleVec()
{
    return p->getAngleVec();
} // getAngleVec()


/**
    Gets the rotation angles.
    Rotation angle is currently ignored for level-2 objects, they are rotated by the same factor as their level-1 parent object using the same pivot point.
    @return Rotation angles vector.
*/
const PRREVector& PRREObject3D::getAngleVec() const
{
    return p->getAngleVec();
} // getAngleVec()


/**
    Gets the base sizes.
    Need to override Mesh3D method because if we are a cloned object, we need to return the size of the object we are referring to.
*/
const PRREVector& PRREObject3D::getSizeVec() const
{
    return (p->pRefersto == PGENULL) ? PRREMesh3D::getSizeVec() : p->pRefersto->getSizeVec();
}


/**
    Gets the real sizes considering the geometry size calculated from vertex data and the current scaling factor.
    @return Real sizes vector.
*/
PRREVector PRREObject3D::getScaledSizeVec() const
{
    return p->getScaledSizeVec();
} // getScaledSizeVec()


/**
    Gets the scaling factor.
    @return Scaling.
*/
const PRREVector& PRREObject3D::getScaling() const
{
    return p->getScaling();
} // getScaling()


/**
    Sets the scaling factor to given scalar.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
*/
void PRREObject3D::SetScaling(TPRREfloat value)
{
    p->SetScaling(value);
} // SetScaling()


/**
    Sets the scaling factor to given vector.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
*/
void PRREObject3D::SetScaling(const PRREVector& value)
{
    p->SetScaling(value);
} // SetScaling()


/**
    Scales by the given scalar value. 
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
*/
void PRREObject3D::Scale(TPRREfloat value)
{
    p->Scale(value);
} // Scale()


/**
    Scales by the given vector.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
*/
void PRREObject3D::Scale(const PRREVector& value)
{
    p->Scale(value);
} // Scale()


/**
    Gets the visibility state.
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
    @return True, if visible, false otherwise.
*/
TPRREbool PRREObject3D::isVisible() const
{
    return p->isVisible();
} // isVisible()


/**
    Sets the visibility state.
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
*/
void PRREObject3D::SetVisible(TPRREbool state)
{
    p->SetVisible(state);
} // SetVisible()


/**
    Sets the visibility state to true.
    Equivalent to SetVisible(true).
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
*/
void PRREObject3D::Show()
{
    p->Show();
} // Show()


/**
    Sets the visibility state to false.
    Equivalent to SetVisible(false).
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
*/
void PRREObject3D::Hide()
{
    p->Hide();
} // Hide()


/**
    Gets whether colliding is enabled.
    Legacy function unrelated to the purpose of this class, will need to be removed!
    @return True, if colliding is enabled, false otherwise.
*/
TPRREbool PRREObject3D::isColliding_TO_BE_REMOVED() const
{
    return p->isColliding_TO_BE_REMOVED();
}


/**
    Sets whether colliding is enabled.
    Legacy function unrelated to the purpose of this class, will need to be removed!
*/
void PRREObject3D::SetColliding_TO_BE_REMOVED(TPRREbool value)
{
    p->SetColliding_TO_BE_REMOVED(value);
}


/**
    Gets the rotation order.
    Rotation order is currently ignored for level-2 objects, they are rotated the same way as their level-1 parent object.
    @return Rotation order.
*/
TPRRE_ROTATION_ORDER PRREObject3D::getRotationOrder() const
{
    return p->getRotationOrder();
} // getRotationOrder()


/**
    Sets the rotation order.
    Rotation order is currently ignored for level-2 objects, they are rotated the same way as their level-1 parent object.
*/
void PRREObject3D::SetRotationOrder(TPRRE_ROTATION_ORDER value)
{
    p->SetRotationOrder(value);
} // SetRotationOrder()


/**
    Gets the lit state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if lit, false otherwise.
*/
TPRREbool PRREObject3D::isLit() const
{
    return p->isLit();
} // isLit()


/**
    Sets the lit state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetLit(TPRREbool value)
{
    p->SetLit(value);
} // SetLit()


/**
    Gets the double sided state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if double sided, false otherwise.
*/
TPRREbool PRREObject3D::isDoubleSided() const
{
    return p->isDoubleSided();
} // isDoubleSided()


/**
    Sets the double sided state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetDoubleSided(TPRREbool value)
{
    p->SetDoubleSided(value);
} // SetDoubleSided()


/**
    Gets the wireframed state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if wireframed, false otherwise.
*/
TPRREbool PRREObject3D::isWireframed() const
{
    return p->isWireframed();
} // isWireframed()


/**
    Sets the wireframed state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetWireframed(TPRREbool value)
{
    p->SetWireframed(value);
} // SetWireframed()


/**
    Gets the wireframed culling state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if culling is enabled in wireframed state, false otherwise.
*/
TPRREbool PRREObject3D::isWireframedCulled() const
{
    return p->isWireframedCulled();
} // isWireframedCulled()


/**
    Sets the wireframed culling state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetWireframedCulled(TPRREbool value)
{
    p->SetWireframedCulled(value);
} // SetWireframedCulled()


/**
    Gets whether we write to the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if we write to Z-Buffer while rendering, false otherwise.
*/
TPRREbool PRREObject3D::isAffectingZBuffer() const
{
    return p->isAffectingZBuffer();
} // isAffectingZBuffer()


/**
    Sets whether we write to the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetAffectingZBuffer(TPRREbool value)
{
    p->SetAffectingZBuffer(value);
} // SetAffectingZBuffer()


/**
    Gets whether we test against the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if we test against the Z-Buffer while rendering, false otherwise.
*/
TPRREbool PRREObject3D::isTestingAgainstZBuffer() const
{
    return p->isTestingAgainstZBuffer();
} // isTestingAgainstZBuffer()


/**
    Sets whether we test against the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetTestingAgainstZBuffer(TPRREbool value)
{
    p->SetTestingAgainstZBuffer(value);
} // SetTestingAgainstZBuffer()


/**
    Gets the sticked-to-screen state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if sticked to screen, false otherwise.
*/
TPRREbool PRREObject3D::isStickedToScreen() const
{
    return p->isStickedToScreen();
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
*/
void PRREObject3D::SetStickedToScreen(TPRREbool value)
{
    p->SetStickedToScreen(value);
} // SetStickedToScreen()


/**
    Gets the amount of allocated system memory.
    It includes the allocated Material size as well (getMaterial(false)).

    @return Amount if allocated system memory in Bytes.
*/
TPRREuint PRREObject3D::getUsedSystemMemory() const
{
    return PRREFiledManaged::getUsedSystemMemory() - sizeof(PRREFiledManaged) +
        PRREMesh3D::getUsedSystemMemory() - sizeof(PRREMesh3D) + 
        sizeof(*this) + p->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    Draws the object.
*/
void PRREObject3D::Draw(bool bLighting)
{
    p->Draw(bLighting);
} // Draw()


// ############################## PROTECTED ##############################


/**
    Only PRREObject3DManager creates it.

    @param prfmt                 Primitive format for the underlying Mesh3D.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
*/
PRREObject3D::PRREObject3D(
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory )
{
    PRREFiledManaged::getConsole().OLnOI("PRREObject3D() ...");
    p = new PRREObject3DImpl(this, vmod, vref, bForceUseClientMemory);
    PRREFiledManaged::getConsole().SOLnOO("Done!");
} // PRREObject3D()


PRREObject3D::PRREObject3D(const PRREObject3D& other)
    : PRREMesh3D(other.getPrimitiveFormat())
{
}


PRREObject3D& PRREObject3D::operator=(const PRREObject3D&)
{
    return *this;
}


// ############################### PRIVATE ###############################


